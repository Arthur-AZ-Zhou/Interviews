#include <bits/stdc++.h>
#include <iostream>
#include <optional>

using namespace std;

class ConfigResolver {
private:
    unordered_set<string> visited; 

public:
    ConfigResolver() {}

    optional<string> resolveValue(const unordered_map<string, string>& config, const string& key) {
        if (config.find(key) == config.end()) {
            return nullopt;
        }

        string resolvedStr = "";
        int i = 0;

        const string& value = config.at(key);

        while (i < value.length()) {
            if (value[i] != '$' || value.length() <= i + 1 || value[i + 1] != '{') {
                resolvedStr += value[i];
                i++;
                continue;
            }

            int closeBrace = value.find('}', i + 2); // first instance of } in string

            if (closeBrace == string::npos) {
                return nullopt;
            }

            string newKey = value.substr(i + 2, closeBrace - i - 2);

            cout << "newKey detected: " << newKey << endl;

            if (visited.find(newKey) != visited.end()) {
                return nullopt;
            }

            cout << "made it past here" << endl;
            visited.insert(newKey);

            cout << "visited set: =============" << endl; 
            for (auto i : visited) {
                cout << i << " ";
            }
            cout << "\nend of visited set ===========" << endl;

            optional<string> configVal = resolveValue(config, newKey);
            cout << "found configVal: " << configVal.value_or("nullopt") << endl;

            if (!configVal.has_value()) {
                return nullopt;
            }

            resolvedStr += configVal.value();
            i = closeBrace + 1;
            visited.erase(newKey);
        }

        return resolvedStr;
    }

};

int main() {
    // unordered_map<string, string> config = {
    //     {"HOST", "localhost"},
    //     {"PORT", "8080"},
    //     {"URL", "http://${HOST}:${PORT}/api"}
    // };

    ConfigResolver CR;

    // cout << CR.resolveValue(config, "HOST").value_or("nullopt") << endl;
    // cout << CR.resolveValue(config, "MISSING").value_or("nullopt") << endl;
    // cout << CR.resolveValue(config, "URL").value_or("nullopt") << endl;

    unordered_map<string, string> config = {
        {"A", "${B}"},
        {"B", "${C}"},
        {"C", "${A}"},
        {"HOST", "localhost"},
        {"PAIR", "${HOST}:${HOST}"},
    };

    cout << CR.resolveValue(config, "A").value_or("nullopt") << endl;
    cout << CR.resolveValue(config, "PAIR").value_or("nullopt") << endl;
}