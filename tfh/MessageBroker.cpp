#include <bits/stdc++.h>
#include <iostream>

using namespace std;

struct Message {
    string messageId;
    string topic;
    string payload;
};

/*
poll returns and removes that consumer’s oldest pending message.
Return nullopt if the consumer is unknown or has no pending messages.
*/

class MessageBroker {
private:
    unordered_map<string, unordered_set<string>> consumerSubMap; //consumerId -> current subscriptions
    unordered_map<string, unordered_set<string>> subConsumerMap; //sub topics -> consumerIds
    unordered_map<string, queue<Message>> consumerMQ; //consumerId -> message queue
    unordered_map<string, vector<Message>> consumerDLQ; //consumerId -> their dlq
    unordered_map<string, Message> inFlightMap; //consumerId -> in flight message
    unordered_map<string, unordered_map<string, int>> consumerAttempts; //consumerId -> messageId -> num tries
    unordered_set<string> messageIDSet; //ensure messageID global unique
    int maxAttempts;

public:
    MessageBroker(int maxDeliveryAttempts) {
        if (maxDeliveryAttempts <= 0) {
            throw invalid_argument("maxDeliveryAttempts must be positive");
        }

        maxAttempts = maxDeliveryAttempts;
    }

    bool subscribe(const string& consumerId, const string& topic) {
        if (consumerId.empty() || topic.empty()) {
            return false;
        }

        if (consumerSubMap.find(consumerId) != consumerSubMap.end() && consumerSubMap[consumerId].find(topic) != consumerSubMap[consumerId].end()) {
            return false;
        }

        consumerSubMap[consumerId].insert(topic);
        subConsumerMap[topic].insert(consumerId);
        if (consumerMQ.find(consumerId) == consumerMQ.end()) {
            consumerMQ[consumerId] = {};
        }

        return true;
    }

    bool publish(const Message& message) {
        if (message.messageId.empty() 
            || message.payload.empty()
            || message.topic.empty()
            || messageIDSet.find(message.messageId) != messageIDSet.end()
        ) {
            return false;
        }

        messageIDSet.insert(message.messageId);
        for (const auto& i : subConsumerMap[message.topic]) {
            consumerMQ[i].push(message);
        }

        return true;
    }

    optional<Message> poll(const string& consumerId) {
        if (consumerSubMap.find(consumerId) == consumerSubMap.end() 
            || (consumerMQ[consumerId].empty() 
                && inFlightMap.find(consumerId) == inFlightMap.end())
        ) {
            return nullopt;
        }

        if (inFlightMap.find(consumerId) != inFlightMap.end()) {
            return inFlightMap[consumerId];
        }

        Message m = consumerMQ[consumerId].front();
        consumerMQ[consumerId].pop();
        inFlightMap[consumerId] = m;
        consumerAttempts[consumerId][m.messageId]++;
        return m;
    }

    bool acknowledge(const string& consumerId, const string& messageId) {
        if (consumerMQ.find(consumerId) == consumerMQ.end() 
            || inFlightMap.find(consumerId) == inFlightMap.end() 
            || inFlightMap[consumerId].messageId != messageId
        ) {
            return false;
        }

        inFlightMap.erase(consumerId);
        consumerAttempts[consumerId].erase(messageId);
        return true;
    }

    bool reject(const string& consumerId, const string& messageId) {
        if (consumerId.empty() 
            || messageId.empty() 
            || consumerMQ.find(consumerId) == consumerMQ.end() 
        ) {
            return false;
        }

        auto inFlightIt = inFlightMap.find(consumerId);

        if (inFlightIt == inFlightMap.end() || inFlightIt->second.messageId != messageId) {
            return false;
        }

        const Message message = inFlightIt->second;
        int attempts = consumerAttempts[consumerId][messageId];

        if (attempts < maxAttempts) {
            consumerMQ[consumerId].push(message);
        } else {
            consumerDLQ[consumerId].push_back(message);
            consumerAttempts[consumerId].erase(messageId);
        }

        inFlightMap.erase(inFlightIt);
        return true;
    }

    vector<Message> getDeadLetters(const string& consumerId) const {
        if (consumerDLQ.find(consumerId) == consumerDLQ.end()) {
            return {};
        }

        return consumerDLQ.at(consumerId);
    }
};

int main() {
    MessageBroker broker(3);

    broker.subscribe("alice", "security");
    broker.subscribe("bob", "security");

    broker.publish({"m1", "security", "login failed"});

    // Alice fails all three attempts.
    cout << broker.poll("alice")->messageId << endl;       // m1, attempt 1
    cout << broker.reject("alice", "m1") << endl;          // 1

    cout << broker.poll("alice")->messageId << endl;       // m1, attempt 2
    cout << broker.reject("alice", "m1") << endl;          // 1

    cout << broker.poll("alice")->messageId << endl;       // m1, attempt 3
    cout << broker.reject("alice", "m1") << endl;          // 1, dead-lettered

    cout << broker.poll("alice").has_value() << endl;      // 0

    auto aliceDeadLetters = broker.getDeadLetters("alice");
    cout << aliceDeadLetters.size() << endl;               // 1
    cout << aliceDeadLetters[0].messageId << endl;         // m1

    // Bob processes his independent copy successfully.
    cout << broker.poll("bob")->messageId << endl;         // m1
    cout << broker.acknowledge("bob", "m1") << endl;       // 1
    cout << broker.getDeadLetters("bob").size() << endl;   // 0

    cout << broker.reject("alice", "m1") << endl;          // 0
    cout << broker.reject("unknown", "m1") << endl;        // 0
    
    //regression test
    cout << broker.reject("alice", "m1") << endl;     // 0
    cout << broker.poll("alice").has_value() << endl; // must remain 0
}