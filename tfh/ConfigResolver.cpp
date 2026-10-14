#include <bits/stdc++.h>
#include <optional>

using namespace std;

class ConfigResolver {
private:
    optional<string> resolveHelper(
        const unordered_map<string, string>& config,
        const string& key,
        unordered_set<string>& resolving
    ) {
        auto configIt = config.find(key);

        if (configIt == config.end()) {
            return nullopt;
        }

        // This key is already in the current recursion path.
        if (resolving.find(key) != resolving.end()) {
            return nullopt;
        }

        resolving.insert(key);

        const string& value = configIt->second;
        string resolved;
        size_t i = 0;

        while (i < value.length()) {
            // Ordinary character, or '$' not followed by '{'.
            if (value[i] != '$' || i + 1 >= value.length() || value[i + 1] != '{') {
                resolved += value[i];
                i++;
                continue;
            }

            size_t closeBrace = value.find('}', i + 2);

            if (closeBrace == string::npos) {
                resolving.erase(key);
                return nullopt;
            }

            string referencedKey = value.substr(
                i + 2,
                closeBrace - (i + 2)
            );

            if (referencedKey.empty()) {
                resolving.erase(key);
                return nullopt;
            }

            optional<string> referencedValue = resolveHelper(config, referencedKey, resolving);

            if (!referencedValue.has_value()) {
                resolving.erase(key);
                return nullopt;
            }

            resolved += referencedValue.value();
            i = closeBrace + 1;
        }

        // Remove it because this recursion branch is finished.
        resolving.erase(key);
        return resolved;
    }

public:
    optional<string> resolveValue(
        const unordered_map<string, string>& config,
        const string& key
    ) {
        unordered_set<string> resolving;
        return resolveHelper(config, key, resolving);
    }
};

int main() {
    ConfigResolver resolver;

    unordered_map<string, string> config = {
        {"A", "${B}"},
        {"B", "${C}"},
        {"C", "${A}"},
        {"HOST", "localhost"},
        {"PORT", "8080"},
        {"URL", "http://${HOST}:${PORT}/api"},
        {"PAIR", "${HOST}:${HOST}"},
        {"SELF", "${SELF}"},
        {"BROKEN", "${HOST"}
    };

    cout << resolver.resolveValue(config, "A").value_or("nullopt")
         << endl; // nullopt: indirect cycle

    cout << resolver.resolveValue(config, "PAIR").value_or("nullopt")
         << endl; // localhost:localhost

    cout << resolver.resolveValue(config, "URL").value_or("nullopt")
         << endl; // http://localhost:8080/api

    cout << resolver.resolveValue(config, "SELF").value_or("nullopt")
         << endl; // nullopt: direct cycle

    cout << resolver.resolveValue(config, "BROKEN").value_or("nullopt")
         << endl; // nullopt: missing closing brace

    cout << resolver.resolveValue(config, "MISSING").value_or("nullopt")
         << endl; // nullopt
}