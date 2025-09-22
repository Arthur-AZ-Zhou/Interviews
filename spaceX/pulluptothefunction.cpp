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
        vector<int> dp(totalFuel, 0);

        for (auto& r : rockets) {
            int minFuel = ceil(r.payloadWeight * factor);

            if (r.capacity < minFuel) {
                continue;
            }

            for (int i = totalFuel; minFuel <= i; i--) {
                dp[i] = max(dp[i], dp[i - minFuel] + 1);
            }
        }

        for (int i = 0; i < dp.size(); i++) {

        }
    }
};
