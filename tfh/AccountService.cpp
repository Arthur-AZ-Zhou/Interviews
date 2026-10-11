#include <bits/stdc++.h>
#include <iostream>
using namespace std;

struct TransferInfo {
    string sender;
    string recipient;
    long long amt;
    bool status;
};

class AccountService {
private:
    unordered_map<string, long long> acctBalanceMap; //acctId -> balance in cents
    unordered_map<string, TransferInfo> requestMap; //reqId -> transfer info
    unordered_map<string, long long> acctTransferMap; //acctId -> total $$ transferred

public:
    AccountService() {}

    bool createAccount(const string& accountId) {
        if (accountId.empty() || acctBalanceMap.find(accountId) != acctBalanceMap.end()) {
            return false;
        }

        acctBalanceMap[accountId] = 0;
        return true;
    }

    bool deposit(const string& accountId, long long amount) {
        if (acctBalanceMap.find(accountId) == acctBalanceMap.end() || amount <= 0) {
            return false;
        }

        acctBalanceMap[accountId] += amount;
        return true;
    }

    bool transfer(
        const string& requestId,
        const string& fromAccount,
        const string& toAccount,
        long long amount
    ) {
        // Phase 1: Validate the idempotency key
        if (requestId.empty()) {
            return false;
        }

        // Phase 2: Handle a previously processed request
        auto requestIt = requestMap.find(requestId);

        if (requestIt != requestMap.end()) {
            const TransferInfo& previous = requestIt->second;

            bool sameRequest =
                previous.sender == fromAccount &&
                previous.recipient == toAccount &&
                previous.amt == amount;

            if (!sameRequest) {
                return false;
            }

            return previous.status;
        }

        // Phase 3: Validate a brand-new transfer
        auto fromIt = acctBalanceMap.find(fromAccount);
        auto toIt = acctBalanceMap.find(toAccount);

        if (fromIt == acctBalanceMap.end() ||
            toIt == acctBalanceMap.end() ||
            fromAccount == toAccount ||
            amount <= 0 ||
            fromIt->second < amount) {

            requestMap[requestId] = {
                fromAccount,
                toAccount,
                amount,
                false
            };

            return false;
        }

        // Phase 4: Execute and remember the successful transfer
        fromIt->second -= amount;
        toIt->second += amount;

        requestMap[requestId] = {
            fromAccount,
            toAccount,
            amount,
            true
        };
        acctTransferMap[fromAccount] += amount;

        return true;
    }

    long long getBalance(const string& accountId) {
        if (acctBalanceMap.find(accountId) == acctBalanceMap.end()) {
            return -1;
        }

        return acctBalanceMap[accountId];
    }

    vector<pair<string, long long>> getTopSenders(int k) const {
        if (k <= 0) {
            return {};
        }

        priority_queue<pair<long long, string>> maxHeap; //amt, then tiebreak by acctId

        for (const auto& i : acctTransferMap) {
            maxHeap.push({i.second, i.first});
        }

        vector<pair<string, long long>> topSenders;
        int numSenders = 0;

        while (!maxHeap.empty() && numSenders < k) {
            topSenders.push_back({maxHeap.top().second, maxHeap.top().first});

            numSenders++;
            maxHeap.pop();
        }

        return topSenders; 
    }
};

int main() {
    AccountService service;

    service.createAccount("alice");
    service.createAccount("bob");
    service.createAccount("charlie");
    service.createAccount("dave");

    service.deposit("alice", 1000);
    service.deposit("bob", 500);
    service.deposit("charlie", 500);

    service.transfer("req-1", "alice", "bob", 300);
    service.transfer("req-1", "alice", "bob", 300); // retry: don't count again
    service.transfer("req-2", "alice", "charlie", 200);

    service.transfer("req-3", "bob", "charlie", 400);
    service.transfer("req-4", "charlie", "alice", 400);

    service.transfer("req-5", "dave", "alice", 100); // fails

    cout << "time to get top senders" << endl;

    auto top = service.getTopSenders(3);

    for (const auto& [accountId, totalSent] : top) {
        cout << accountId << ": " << totalSent << endl;
    }
}