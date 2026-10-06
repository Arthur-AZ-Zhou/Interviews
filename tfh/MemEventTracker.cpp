#include <bits/stdc++.h>
#include <iostream>

using namespace std;

struct Event {
    string userId;
    string eventType;
    long long timestamp;
};

struct UserSummaryData {
    int totalEvents;
    long long firstTimestamp;
    long long lastTimestamp;
    unordered_map<string, int> eventTypeCounts;
};

class EventTracker {
private:
    unordered_map<string, UserSummaryData> userSummaryMap; //userId/name -> event

public: 
    EventTracker() {}

    bool recordEvent(const Event& event) { //returns false if timestamp neg
        if (event.timestamp < 0) {
            return false;
        }

        if (userSummaryMap.find(event.userId) == userSummaryMap.end()) {
            userSummaryMap[event.userId] = {1, event.timestamp, event.timestamp, {}};
            userSummaryMap[event.userId].eventTypeCounts[event.eventType]++;
        } else { //exists in map
            userSummaryMap[event.userId].totalEvents++;
            userSummaryMap[event.userId].firstTimestamp = min(userSummaryMap[event.userId].firstTimestamp, event.timestamp);
            userSummaryMap[event.userId].lastTimestamp = max(userSummaryMap[event.userId].lastTimestamp, event.timestamp);
            userSummaryMap[event.userId].eventTypeCounts[event.eventType]++;
        }

        return true;
    }

    string getUserSummary(const string& userId) {
        auto it = userSummaryMap.find(userId);

        if (it == userSummaryMap.end()) {
            return "No data found for user: " + userId;
        }

        const UserSummaryData& data = it->second;

        string returnStr;
        returnStr += "========================================\n";
        returnStr += "           " + userId + " Stats\n";
        returnStr += "========================================\n";
        returnStr += "Total Events   : " + to_string(data.totalEvents) + "\n";
        returnStr += "First Event    : " + to_string(data.firstTimestamp) + "\n";
        returnStr += "Last Event     : " + to_string(data.lastTimestamp) + "\n";
        returnStr += "\nEvent Type Counts:\n";
        returnStr += "----------------------------------------\n";

        for (const auto& [eventType, count] : data.eventTypeCounts) {
            returnStr += "  " + eventType + " : " + to_string(count) + "\n";
        }

        returnStr += "========================================\n";

        return returnStr;
    }

};

int main() {
    
}