#include <bits/stdc++.h>
using namespace std;

// ------------------ Entity Definitions ------------------
struct GroundStation {
    int id;
    double x, y; // Position
};

struct Satellite {
    int id;
    double x, y; // Position
};

struct Rocket {
    GroundStation startGCS;
    Satellite destinationSat;
    int id;
    double fuel;
    double capacity;
    double mpg; // miles per gallon of fuel
};

// ------------------ Utility Functions ------------------
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// ------------------ System / Matching Class ------------------
class Matcher {
public:
    /**
     * @brief Assigns each satellite to nearest ground station
     * 
     * @param sats satellites 
     * @param stations GCS
     * @returns pair of satellite ID to GCS ID
     */
    vector<pair<int,int>> assignSatellitesToStations(const vector<Satellite>& sats, const vector<GroundStation>& stations) {
        if (stations.empty() || sats.empty()) {
            return {};
        }

        vector<pair<int, int>> sattellitePairing;

        for (const Satellite& s : sats) {
            double tempAnswer = DBL_MAX;
            pair<int, int> tempAssign;

            for (const GroundStation& gcs : stations) {
                if (distance(s.x, s.y, gcs.x, gcs.y) < tempAnswer) {
                    tempAnswer = distance(s.x, s.y, gcs.x, gcs.y);
                    tempAssign = {s.id, gcs.id};
                }
            }

            sattellitePairing.push_back(tempAssign);
        }

        return sattellitePairing;
    }

    /**
     * @brief calculates fuel requirement for rocket based on start GCS to end Sattelite, if over capacity return -1.0
     * 
     * @param rocket rocket to calculate fuel requirement
     * @return min amt of fuel to reach destination, -1.0 if not possible w/ capacity
     */
    double computeFuelRequirement(const Rocket& rocket) {
        double fuelNeeded;

        double dist = distance(rocket.startGCS.x, rocket.startGCS.y, rocket.destinationSat.x, rocket.destinationSat.y);
        fuelNeeded = dist / rocket.mpg;

        return (fuelNeeded <= rocket.capacity)? fuelNeeded : -1.0;
    }
};
