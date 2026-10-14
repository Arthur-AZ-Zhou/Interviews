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
    unordered_set<string> messageIDSet; //ensure messageID global unique

public:
    MessageBroker() {}

    bool subscribe(
        const string& consumerId,
        const string& topic
    ) {
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
        if (consumerSubMap.find(consumerId) == consumerSubMap.end() || consumerMQ[consumerId].empty()) {
            return nullopt;
        }

        Message m = consumerMQ[consumerId].front();
        consumerMQ[consumerId].pop();
        return m;
    }
};

int main() {
    MessageBroker broker;

    cout << broker.subscribe("alice", "security") << endl; // 1
    cout << broker.subscribe("bob", "security") << endl;   // 1
    cout << broker.subscribe("alice", "deploys") << endl;  // 1
    cout << broker.subscribe("alice", "security") << endl; // 0

    cout << broker.publish({
        "m1", "security", "login failed"
    }) << endl; // 1

    cout << broker.publish({
        "m2", "deploys", "version 2 deployed"
    }) << endl; // 1

    cout << broker.publish({
        "m1", "security", "different payload"
    }) << endl; // 0: duplicate message ID

    auto alice1 = broker.poll("alice");
    cout << alice1.value().messageId << endl; // m1

    auto alice2 = broker.poll("alice");
    cout << alice2.value().messageId << endl; // m2

    cout << broker.poll("alice").has_value() << endl; // 0

    auto bob1 = broker.poll("bob");
    cout << bob1.value().messageId << endl; // m1

    cout << broker.subscribe("charlie", "security") << endl; // 1
    cout << broker.poll("charlie").has_value() << endl;      // 0

    cout << broker.publish({
        "m3", "security", "account locked"
    }) << endl; // 1

    cout << broker.poll("charlie").value().messageId << endl; // m3

    cout << broker.publish({
        "", "security", "invalid"
    }) << endl; // 0
}