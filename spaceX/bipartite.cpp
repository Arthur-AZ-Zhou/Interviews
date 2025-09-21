#include <bits/stdc++.h>
using namespace std;

struct GroundStation {
    int id;
    double x, y;
};

struct Satellite {
    int id;
    double x, y;
};

struct Rocket {
    int id;
    double fuelCapacity;
    double mpg;
    GroundStation start;
};

double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// ------------------ Matcher Class ------------------
class Matcher {
public:
    /**
     * @brief Assign rockets to satellites such that every satellite is covered
     * 
     * @param rockets list of rockets
     * @param sats list of satellites
     * @return vector<pair<int,int>> rocketId -> satelliteId
     */
    vector<pair<int,int>> assignRockets(const vector<Rocket>& rockets, const vector<Satellite>& sats) {
        // Step 1: Build adjacency graph (rocket -> satellites it can reach)
        vector<vector<int>> adj(rockets.size());

        for (int i = 0; i < rockets.size(); i++) {
            for (int j = 0; j < sats.size(); j++) {
                double dist = distance(rockets[i].start.x, rockets[i].start.y, sats[j].x, sats[j].y);
                double fuelNeeded = dist / rockets[i].mpg;

                if (fuelNeeded <= rockets[i].fuelCapacity) {
                    adj[i].push_back(j); // rocket i can reach satellite j
                }
            }
        }

        // Step 2: Implement bipartite matching
        // matchSat[j] = rocket assigned to satellite j (or -1)
        vector<int> matchSat(sats.size(), -1);

        // Try to assign each rocket using DFS
        for (int i = 0; i < rockets.size(); i++) {
            vector<bool> visited(sats.size(), false);

            if (!dfs(i, adj, matchSat, visited)) {
                return {}; // impossible to assign every satellite
            }
        }

        // Step 3: Collect results
        vector<pair<int,int>> result;
        for (int j = 0; j < sats.size(); j++) {
            if (matchSat[j] != -1) {
                result.push_back({rockets[matchSat[j]].id, sats[j].id});
            }
        }
        return result;
    }

private:
    bool dfs(int rocket, const vector<vector<int>>& adj, vector<int>& matchSat, vector<bool>& visited) {
        for (int sat : adj[rocket]) {
            if (visited[sat]) {
                continue;
            }
            visited[sat] = true;

            if (matchSat[sat] == -1 || dfs(matchSat[sat], adj, matchSat, visited)) {
                matchSat[sat] = rocket;
                return true;
            }
        }
        return false;
    }
};

// ------------------ Example ------------------
int main() {
    vector<GroundStation> gcs = {{1, 0, 0}, {2, 10, 10}};
    vector<Satellite> sats = {{101, 5, 0}, {102, 12, 8}};
    vector<Rocket> rockets = {
        {201, 10, 1.0, gcs[0]},
        {202, 20, 1.0, gcs[1]}
    };

    Matcher matcher;
    auto result = matcher.assignRockets(rockets, sats);

    if (result.empty()) {
        cout << "No full assignment possible.\n";
    } else {
        for (auto [r, s] : result) {
            cout << "Rocket " << r << " -> Satellite " << s << "\n";
        }
    }
}
