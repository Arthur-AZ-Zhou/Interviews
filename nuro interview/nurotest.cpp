#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

// Struct to store vehicle conflict event data
struct ConflictEvent {
    int vehicle_id;
    double conflict_x, conflict_y;  // Conflict point location
    double time_front_at_conflict;  // Time when front bumper reaches conflict point
    double speed;                   // Speed (m/s)
    double acceleration;             // Acceleration (m/s²)
    double length;                   // Vehicle length (m)
};

// Function to compute probability of collision based on TTC and DeltaS
double computeCollisionProbability(double ttc, double deltaS, double initialDR) {
    if (ttc <= 0) return 1.0; // Immediate collision
    if (ttc > 5.0) return 0.0; // Safe case

    double riskFactor = (1.0 / ttc) + (deltaS / 50.0) - (initialDR / 10.0);; // Simple risk model
    return min(1.0, max(0.0, riskFactor)); // Clamp probability between 0 and 1
}

// Function to compute surrogate safety measures with vehicle length adjustment
void computeSafetyMetrics(vector<ConflictEvent> &events) {
    // Print all generated vehicles (for debugging)
    cout << "\n=== GENERATED VEHICLE DATA ===\n";
    for (auto &v : events) {
        cout << "Vehicle " << v.vehicle_id << " -> Conflict Point: (" << v.conflict_x << ", " << v.conflict_y << "), "
             << "Time: " << v.time_front_at_conflict << "s, "
             << "Speed: " << v.speed << " m/s, "
             << "Acceleration: " << v.acceleration << " m/s², "
             << "Length: " << v.length << " m\n";
    }
    cout << "==================================\n\n";

    // Sort events by time_at_conflict
    sort(events.begin(), events.end(), [](const ConflictEvent &a, const ConflictEvent &b) {
        return a.time_front_at_conflict < b.time_front_at_conflict;
    });

    bool foundConflicts = false; // Track if any conflicts are found

    // Compare each vehicle with all others at the same conflict point
    for (int i = 0; i < events.size(); i++) {
        for (int j = i + 1; j < events.size(); j++) {
            ConflictEvent A = events[i];
            ConflictEvent B = events[j];

            // Ensure they are at the same conflict point
            if (A.conflict_x == B.conflict_x && A.conflict_y == B.conflict_y) {
                foundConflicts = true;

                // Compute adjusted departure time (consider vehicle length)
                double t_departure_A = A.time_front_at_conflict + (A.length / A.speed);

                // Compute PET using adjusted departure time
                double PET = B.time_front_at_conflict - t_departure_A;

                // Compute TTC (Time to Collision)
                double distanceBetween = fabs(A.speed * PET); // Rough estimate of separation
                double TTC = (distanceBetween > 0) ? distanceBetween / fabs(A.speed - B.speed) : 999.0; // Avoid divide-by-zero

                double deltaS = fabs(A.speed - B.speed);
                double maxS = max(A.speed, B.speed);
                double initialDR = -A.acceleration; // Assuming initial deceleration is negative acceleration

                double collisionProbability = computeCollisionProbability(TTC, deltaS, initialDR);

                // Print results
                cout << "🚗 **Conflict at (" << A.conflict_x << ", " << A.conflict_y << ")** 🚗\n";
                cout << "Vehicles " << A.vehicle_id << " & " << B.vehicle_id << ":\n";
                cout << "  PET: " << PET << " sec\n";
                cout << "  TTC: " << TTC << " sec\n";
                cout << "  DeltaS: " << deltaS << " m/s\n";
                cout << "  MaxS: " << maxS << " m/s\n";
                cout << "  Initial DR: " << initialDR << " m/s²\n";
                cout << "  Collision Probability: " << (collisionProbability * 100) << "%\n";
                cout << "---------------------------------------------\n";
            }
        }
    }

    // If no conflicts were found, notify the user
    if (!foundConflicts) {
        cout << "⚠️ No conflicts detected! Try running again for different random data.\n";
    }
}

// Function to read data from a space-separated text file
vector<ConflictEvent> readFromTextFile(const string &filename) {
    vector<ConflictEvent> simulationData;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return simulationData;
    }

    ConflictEvent event;
    while (file >> event.vehicle_id 
                >> event.conflict_x 
                >> event.conflict_y 
                >> event.time_front_at_conflict 
                >> event.speed 
                >> event.acceleration 
                >> event.length) {
        simulationData.push_back(event);
    }

    file.close();
    return simulationData;
}

// Function to read data from a CSV file
vector<ConflictEvent> readFromCSV(const string &filename) {
    vector<ConflictEvent> simulationData;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return simulationData;
    }

    string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        ConflictEvent event;
        char comma; // To ignore commas

        ss >> event.vehicle_id >> comma
           >> event.conflict_x >> comma
           >> event.conflict_y >> comma
           >> event.time_front_at_conflict >> comma
           >> event.speed >> comma
           >> event.acceleration >> comma
           >> event.length;

        simulationData.push_back(event);
    }

    file.close();
    return simulationData;
}

// Sample simulation data generator
vector<ConflictEvent> generateSimulationData(int numVehicles) {
    vector<ConflictEvent> simulationData;
    srand(time(0));

    for (int i = 0; i < numVehicles; i++) {
        double conflict_x = (rand() % 3) * 10.0; // Reduced randomness to increase collisions
        double conflict_y = (rand() % 3) * 10.0;
        double time_front_at_conflict = (rand() % 20) / 2.0 + i * 0.5; // Staggered times
        double speed = (rand() % 20) + 5.0; // Speeds between 5-25 m/s
        double acceleration = (rand() % 4) - 2.0; // Deceleration or acceleration (-2 to +2 m/s²)
        double length = (rand() % 5) + 3.0; // Vehicle length between 3-8 meters

        simulationData.push_back({i + 1, conflict_x, conflict_y, time_front_at_conflict, speed, acceleration, length});
    }

    return simulationData;
}

// Main function
int main() {
    vector<ConflictEvent> simulationData = generateSimulationData(10);
    computeSafetyMetrics(simulationData);
    return 0;
}
