#include <bits/stdc++.h>
using namespace std;

/**
 * Build a feature allows applications that use plaid to pass in data to make it ezier for consumers to find bank
 * 
 * Routing numbers: sequence of digits to identify bank (string/int?)
 *    - bank can have multiple, but digits can only point to 1 bank
 * 
 * Bank name: (string)
 *    - name of bank, BANK CAN GO BY DIFF NAMES
 * 
 * Bank ID(int): identify every single bank
 * 
 * Milestone 1:
 * - 1 to 1 mapping of routing number to related bank {
        "123": "Wells Fargo",
        "456": "Chase",
        "789": "Capital One",
        "555": "First State Bank",
        "999": "Wells Fargo",
     }
    - [
  # There are multiple common ways to write the name of this bank [
        ("Wells Fargo", 1),
        ("Wells", 1),

        ("Chase", 2),
        ("Capital One", 3),
        ("Bank of America", 4),

        # These are two different banks with the same name
        ("First State Bank", 5),
        ("First State Bank", 6),
        ]
 */

// BankTuple class to store name and ID
// class BankTuple {
// public:
//     string name;
//     int id;

//     BankTuple(string name, int id) : name(name), id(id) {}
// };

// some routing numbers don't have routing for and other routing numbers map to wrong bank ID
// some routing numbers are incomplete/wrong

// createRoutingNumberMapping combines a map from routing number to bank name with a list of relationships
// between bank names and bank IDs to create a single map with routing numbers as keys and a list of related Bank IDs as values.
unordered_map<string, vector<int>> createRoutingNumberMapping(const unordered_map<string, string>& rnToName, const vector<pair<string, int>>& nameToBankId) {
    unordered_map<string, vector<int>> rnToBankIds;
    unordered_map<string, vector<int>> nameToID;

    for (auto i : nameToBankId) {
        nameToID[i.first].push_back(i.second);
    }

    for (auto i : rnToName) {
        // cout << "i.first: " << i.first << ", nameToID[i.second]: " << nameToID[i.second] << endl;
        rnToBankIds[i.first].insert(rnToBankIds[i.first].end(), nameToID[i.second].begin(), nameToID[i.second].end());
    }

    return rnToBankIds;
}

unordered_map<string, vector<int>> newRNMap(const vector<unordered_map<string, string>>& rnToName, const vector<pair<string, int>>& nameToBankId) {
    unordered_map<string, vector<int>> rnToBankIds;
    unordered_map<string, vector<int>> nameToID;
    unordered_map<string, vector<int>> sortedResults; //routing number -> bank ids

    for (auto i : nameToBankId) {
        nameToID[i.first].push_back(i.second);
    }

    //process rnToName get a mapping RN : maxHeap(ID)

    for (auto i : rnToName) {
        for (auto j : i) { //run through the map
            string routingNum = j.first;
            string name = j.second;

            sortedResults[routingNum].insert(sortedResults[routingNum].end(), nameToID[name].begin(), nameToID[name].end());
        }
    }

    for (auto i : sortedResults) {
        //sum up each of the frequencies
        //sort using c++ built in library
        // RN - > list of possible IDs
    }

    return rnToBankIds;
}

int main() {
    unordered_map<string, string> rnToName = {
        {"123", "Wells Fargo"},
        {"456", "Chase"},
        {"789", "Capital One"},
        {"555", "First State Bank"},
        {"999", "Wells Fargo"},
    };

    vector<pair<string, int>> EXAMPLE_DATA = {
        {"Wells Fargo", 1},
        {"Wells", 1},
        {"Chase", 2},
        {"Capital One", 3},
        {"Bank of America", 4},
        {"First State Bank", 5},
        {"First State Bank", 6}
    };

    unordered_map<string, vector<int>> mapping = createRoutingNumberMapping(rnToName, EXAMPLE_DATA);

    // cout << "begin printing" << endl;

    for (auto i : mapping) {
        cout << i.first << " -> ";

        for (int j = 0; j < i.second.size(); j++) {
            cout << i.second[j] << " ";
        }

        cout << endl;
    }    
}