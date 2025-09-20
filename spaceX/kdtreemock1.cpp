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

// ------------------ KD-Tree ------------------
struct KDNode {
    GroundStation station;
    KDNode* left;
    KDNode* right;
    int axis; // 0 = x, 1 = y
    KDNode(const GroundStation& s, int a) : station(s), left(nullptr), right(nullptr), axis(a) {}
};

class KDTree {
    KDNode* root = nullptr;

    KDNode* build(vector<GroundStation>& pts, int depth, int l, int r) {
        if (l > r) return nullptr;
        int axis = depth % 2;
        int mid = (l + r) / 2;

        auto cmp = [axis](const GroundStation& a, const GroundStation& b) {
            return axis == 0 ? a.x < b.x : a.y < b.y;
        };
        nth_element(pts.begin() + l, pts.begin() + mid, pts.begin() + r + 1, cmp);

        KDNode* node = new KDNode(pts[mid], axis);
        node->left  = build(pts, depth + 1, l, mid - 1);
        node->right = build(pts, depth + 1, mid + 1, r);
        return node;
    }

    void nearest(KDNode* node, double x, double y, GroundStation& best, double& bestDist) {
        if (!node) return;

        double d = distance(x, y, node->station.x, node->station.y);
        if (d < bestDist) {
            bestDist = d;
            best = node->station;
        }

        int axis = node->axis;
        double diff = (axis == 0 ? x - node->station.x : y - node->station.y);

        KDNode* first = diff < 0 ? node->left : node->right;
        KDNode* second = diff < 0 ? node->right : node->left;

        nearest(first, x, y, best, bestDist);
        if (fabs(diff) < bestDist) {
            nearest(second, x, y, best, bestDist);
        }
    }

public:
    KDTree(const vector<GroundStation>& stations) {
        if (!stations.empty()) {
            vector<GroundStation> pts = stations;
            root = build(pts, 0, 0, (int)pts.size() - 1);
        }
    }

    GroundStation nearestNeighbor(double x, double y) {
        GroundStation best = {-1, 0, 0};
        double bestDist = DBL_MAX;
        nearest(root, x, y, best, bestDist);
        return best;
    }
};

// ------------------ Matcher ------------------
class Matcher {
public:
    /**
     * @brief Assigns each satellite to nearest ground station using KDTree
     */
    vector<pair<int,int>> assignSatellitesToStations(
        const vector<Satellite>& sats,
        const vector<GroundStation>& stations)
    {
        vector<pair<int,int>> assignments;
        if (stations.empty()) return assignments;

        KDTree tree(stations);

        for (const Satellite& s : sats) {
            GroundStation g = tree.nearestNeighbor(s.x, s.y);
            assignments.push_back({s.id, g.id});
        }

        return assignments;
    }

    /**
     * @brief calculates fuel requirement for rocket based on start GCS to end Satellite, 
     *        if over capacity return -1.0
     */
    double computeFuelRequirement(const Rocket& rocket) {
        if (rocket.mpg <= 0) return -1.0; // avoid div by zero
        double dist = distance(rocket.startGCS.x, rocket.startGCS.y,
                               rocket.destinationSat.x, rocket.destinationSat.y);
        double fuelNeeded = dist / rocket.mpg;
        return (fuelNeeded <= rocket.capacity) ? fuelNeeded : -1.0;
    }
};

// ------------------ Example Main ------------------
int main() {
    vector<GroundStation> stations = {
        {1, 0, 0}, {2, 10, 10}, {3, -5, 4}
    };

    vector<Satellite> sats = {
        {101, 1, 1}, {102, 9, 9}, {103, -4, 3}
    };

    Matcher matcher;
    auto assignments = matcher.assignSatellitesToStations(sats, stations);

    for (auto [satId, gcsId] : assignments) {
        cout << "Satellite " << satId << " -> Ground Station " << gcsId << "\n";
    }

    Rocket r{stations[0], sats[1], 200, 100.0, 120.0, 0.5};
    cout << "Rocket fuel requirement: " << matcher.computeFuelRequirement(r) << "\n";
}
