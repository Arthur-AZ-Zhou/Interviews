#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>

struct Vehicle {
    int id;
    int lane_number;
    int approach_number;
    double speed;  // m/s
    double latitude;
    double longitude;
    double acceleration;
    bool has_right_of_way;
};

struct ConflictEvent {
    int encroaching_vehicle_id;
    int right_of_way_vehicle_id;
    double conflict_lat;
    double conflict_lon;
    double t1, t2, t3, t4, t5;  // Time steps
    double ttc, pet;
    double max_speed_encroaching, max_speed_row, max_speed_diff;
    bool valid_conflict;
};

class ConflictPointAnalyzer {
private:
    std::vector<ConflictEvent> events;
    double ttc_upper_limit = 1.5; // Threshold for valid conflict

public:
    void recordConflictPoint(const Vehicle& encroaching, const Vehicle& right_of_way, double time_step) {
        ConflictEvent event;
        event.encroaching_vehicle_id = encroaching.id;
        event.right_of_way_vehicle_id = right_of_way.id;
        event.conflict_lat = (encroaching.latitude + right_of_way.latitude) / 2;
        event.conflict_lon = (encroaching.longitude + right_of_way.longitude) / 2;
        event.t1 = time_step;

        // Compute projected arrival time of the right-of-way vehicle
        event.t2 = time_step;
        event.t3 = time_step + (encroaching.speed > 0 ? 1.0 / encroaching.speed : 0); // Approximate clearance time
        event.t5 = time_step + (right_of_way.speed > 0 ? 1.0 / right_of_way.speed : 0);

        event.ttc = event.t3 - event.t2;
        event.pet = event.t5 - event.t2;

        event.max_speed_encroaching = encroaching.speed;
        event.max_speed_row = right_of_way.speed;
        event.max_speed_diff = std::abs(encroaching.speed - right_of_way.speed);

        event.valid_conflict = (event.ttc < ttc_upper_limit);

        if (event.valid_conflict) {
            events.push_back(event);
            std::cout << "Conflict event recorded! TTC: " << event.ttc << "s, PET: " << event.pet << "s\n";
        }
    }

    void displayResults() {
        for (const auto& e : events) {
            std::cout << "Conflict Event (Encroaching Vehicle " << e.encroaching_vehicle_id
                      << " vs Right-of-Way Vehicle " << e.right_of_way_vehicle_id << ")\n";
            std::cout << "Conflict Point: (" << e.conflict_lat << ", " << e.conflict_lon << ")\n";
            std::cout << "TTC: " << e.ttc << "s, PET: " << e.pet << "s\n";
            std::cout << "Valid Conflict: " << (e.valid_conflict ? "Yes" : "No") << "\n";
        }
    }
};

// Function to read vehicles from a space-separated file
std::vector<Vehicle> readVehiclesFromFile(const std::string& filename) {
    std::vector<Vehicle> vehicles;
    std::ifstream file(filename);
    
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return vehicles;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Vehicle v;
        int right_of_way_flag;
        if (!(iss >> v.id >> v.lane_number >> v.approach_number >> v.speed 
                >> v.latitude >> v.longitude >> v.acceleration >> right_of_way_flag)) {
            std::cerr << "Error: Invalid line format: " << line << std::endl;
            continue;
        }
        v.has_right_of_way = (right_of_way_flag == 1);
        vehicles.push_back(v);
    }

    return vehicles;
}

int main() {
    ConflictPointAnalyzer analyzer;

    // Read vehicle data from file
    std::vector<Vehicle> vehicles = readVehiclesFromFile("vehicles.txt");

    // Ensure there are vehicles for conflict analysis
    if (vehicles.size() < 2) {
        std::cerr << "Error: Not enough vehicles to analyze conflicts." << std::endl;
        return 1;
    }

    // Process potential conflicts between encroaching and right-of-way vehicles
    for (size_t i = 0; i < vehicles.size(); ++i) {
        for (size_t j = 0; j < vehicles.size(); ++j) {
            if (i != j && vehicles[i].has_right_of_way == false && vehicles[j].has_right_of_way == true) {
                analyzer.recordConflictPoint(vehicles[i], vehicles[j], 5.0);  // Assume time_step = 5.0
            }
        }
    }

    analyzer.displayResults();
    return 0;
}
