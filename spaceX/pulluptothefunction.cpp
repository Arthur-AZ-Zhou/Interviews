#include <bits/stdc++.h>
using namespace std;

struct Rocket {
    int id;
    double capacity;     // max fuel it can hold
    double payloadWeight; // weight of payload
};

// ------------------ Solver Class ------------------
class FuelAllocator {
public:
    /**
     * @brief Assigns fuel to rockets to maximize number of launches
     * 
     * @param rockets list of rockets
     * @param totalFuel total fuel available at the station
     * @param factor multiplier: min fuel needed = payloadWeight * factor
     * @return vector<int> IDs of rockets that can launch
     */
    vector<int> maximizeLaunches(vector<Rocket>& rockets, double totalFuel, double factor) {
        vector<int> dp(totalFuel + 1, -1); //-1 is unreachable
        vector<int> parent(totalFuel + 1, -1); //which rocket lead to dp[i]
        vector<int> prevFuel(totalFuel + 1 , -1); //prev fuel state

        dp[0] = 0;

        for (auto& r : rockets) {
            int minFuel = ceil(r.payloadWeight * factor);

            if (r.capacity < minFuel) { //rocket cannot launch
                continue;
            }

            for (int i = totalFuel; minFuel <= i; i--) {
                if (dp[i] < dp[i - minFuel] + 1) {
                    dp[i] = dp[i - minFuel] + 1;
                    parent[i] = r.id;
                    prevFuel[i] = i - minFuel;
                }
            }
        }

        int bestFuel = max_element(dp.begin(), dp.end()) - dp.begin();

        vector<int> chosenRockets;
        for (int i = bestFuel; 0 < i && parent[i] != -1; i = prevFuel[i]) {
            chosenRockets.push_back(parent[i]);
        }

        return chosenRockets;
    }
};
