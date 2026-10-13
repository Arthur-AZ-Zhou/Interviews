#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/*

*/

class RateLimiter {
private:
    unordered_map<string, deque<pair<long long, int>>> slidingWindow; // map to deque of {timestamps, cost} per user
    unordered_map<string, int> userCurrentCostMap; //userId -> current cost they're using
    int maxReqs;
    long long window;

public:
    RateLimiter(int maxRequests, long long windowSeconds) : maxReqs(maxRequests), window(windowSeconds) {}

    bool allowRequest(const string& userId, long long timestamp, int cost = 1) {
        if (userId.empty() || timestamp < 0 || cost <= 0) {
            return false;
        }

        if (slidingWindow[userId].empty()) {
            if (cost <= maxReqs) {
                slidingWindow[userId].push_back({timestamp, cost});
                userCurrentCostMap[userId] += cost;
                return true;
            } else {
                return false;
            }
        }

        while (!slidingWindow.empty() && slidingWindow[userId].front().first + window <= timestamp) {
            userCurrentCostMap[userId] -= slidingWindow[userId].front().second;
            slidingWindow[userId].pop_front();
        }

        if (cost <= maxReqs - userCurrentCostMap[userId]) {
            slidingWindow[userId].push_back({timestamp, cost});
            userCurrentCostMap[userId] += cost;
            return true;
        } else {
            return false;
        }
    }

    /*
    bool allowRequest(const string& userId, long long timestamp) {
        if (userId.empty() || timestamp < 0) {
            return false;
        }

        if (slidingWindow[userId].empty()) {
            slidingWindow[userId].push_back(timestamp);
            return true;
        }

        if (timestamp < slidingWindow[userId].front() + window) { //within timeframe
            if (slidingWindow[userId].size() < maxReqs) {
                slidingWindow[userId].push_back(timestamp);
            } else {
                return false;
            }
        } else {
            while (slidingWindow[userId].front() + window <= timestamp) {
                slidingWindow[userId].pop_front();
            }

            if (slidingWindow[userId].size() < maxReqs) {
                slidingWindow[userId].push_back(timestamp);
            } else {
                return false;
            }

        }

        return true;
    }
    */
};

int main() {
    RateLimiter limiter(5, 10);

    cout << limiter.allowRequest("alice", 1, 3) << endl;  // 1, total = 3
    cout << limiter.allowRequest("alice", 2, 2) << endl;  // 1, total = 5
    cout << limiter.allowRequest("alice", 3, 1) << endl;  // 0, would become 6

    cout << limiter.allowRequest("bob", 3, 5) << endl;    // 1
    cout << limiter.allowRequest("bob", 4) << endl;       // 0, default cost 1

    cout << limiter.allowRequest("alice", 11, 3) << endl; // 1
    // Request at t=1 has expired, so total goes from 5 to 2, then to 5.

    cout << limiter.allowRequest("alice", 12, 2) << endl; // 1
    // Request at t=2 has now expired.

    cout << limiter.allowRequest("alice", 13, 0) << endl; // 0
    cout << limiter.allowRequest("alice", 13, 6) << endl; // 0
}