#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <stdexcept>

enum class SignalState { GREEN, YELLOW, RED };
enum class ApproachDirection { NORTH, SOUTH, EAST, WEST };

struct VehicleData {
    double latitude = 0.0;
    double longitude = 0.0;
    double speed = 0.0;
    double acceleration = 0.0;
    ApproachDirection approach = ApproachDirection::NORTH;
    int laneNumber = 0;
};

struct DriverBehavior {
    double reactionTime = 0.0;
    double attentiveness = 1.0;
    double aggressiveness = 0.0;
};

class ConflictPointAnalyzer {
private:
    const double TTC_UPPER_LIMIT = 1.5;

    struct ConflictEvent {
        VehicleData encroachingVehicle;
        VehicleData rightOfWayVehicle;
        SignalState trafficSignalState;
        
        struct ConflictPointLocation {
            double latitude = 0.0;
            double longitude = 0.0;
        } conflictPoint;

        double timeToCollision = 0.0;
    };

    std::vector<ConflictEvent> conflictEvents;

    double computeDistance(const VehicleData& v1, const VehicleData& v2) {
        return std::sqrt(std::pow(v1.latitude - v2.latitude, 2) + 
                         std::pow(v1.longitude - v2.longitude, 2));
    }

    ApproachDirection stringToApproachDirection(const std::string& dirStr) {
        if (dirStr == "NORTH") return ApproachDirection::NORTH;
        if (dirStr == "SOUTH") return ApproachDirection::SOUTH;
        if (dirStr == "EAST") return ApproachDirection::EAST;
        if (dirStr == "WEST") return ApproachDirection::WEST;
        throw std::invalid_argument("Invalid approach direction: " + dirStr);
    }

    SignalState stringToSignalState(const std::string& stateStr) {
        if (stateStr == "GREEN") return SignalState::GREEN;
        if (stateStr == "YELLOW") return SignalState::YELLOW;
        if (stateStr == "RED") return SignalState::RED;
        throw std::invalid_argument("Invalid signal state: " + stateStr);
    }

public:
    void readVehicleDataFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "ERROR: Could not open file: " << filename << std::endl;
            return;
        }

        std::string line;
        int lineCount = 0;

        while (std::getline(file, line)) {
            lineCount++;
            std::cout << "Processing line " << lineCount << ": " << line << std::endl;

            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;

            // Split line by comma
            while (std::getline(iss, token, ',')) {
                // Trim whitespace
                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                if (!token.empty()) {
                    tokens.push_back(token);
                }
            }

            // Debug: print tokens
            std::cout << "Tokens count: " << tokens.size() << std::endl;
            for (const auto& t : tokens) {
                std::cout << "Token: '" << t << "'" << std::endl;
            }

            // Check if we have enough tokens for a complete record
            if (tokens.size() < 13) {
                std::cerr << "Skipping invalid line " << lineCount 
                          << ": insufficient tokens" << std::endl;
                continue;
            }

            try {
                ConflictEvent event;

                // Parse encroaching vehicle data
                event.encroachingVehicle.latitude = std::stod(tokens[0]);
                event.encroachingVehicle.longitude = std::stod(tokens[1]);
                event.encroachingVehicle.speed = std::stod(tokens[2]);
                event.encroachingVehicle.acceleration = std::stod(tokens[3]);
                event.encroachingVehicle.approach = stringToApproachDirection(tokens[4]);
                event.encroachingVehicle.laneNumber = std::stoi(tokens[5]);

                // Parse right-of-way vehicle data
                event.rightOfWayVehicle.latitude = std::stod(tokens[6]);
                event.rightOfWayVehicle.longitude = std::stod(tokens[7]);
                event.rightOfWayVehicle.speed = std::stod(tokens[8]);
                event.rightOfWayVehicle.acceleration = std::stod(tokens[9]);
                event.rightOfWayVehicle.approach = stringToApproachDirection(tokens[10]);
                event.rightOfWayVehicle.laneNumber = std::stoi(tokens[11]);

                // Parse signal state
                event.trafficSignalState = stringToSignalState(tokens[12]);

                // Compute conflict point (simplified)
                event.conflictPoint.latitude = (event.encroachingVehicle.latitude + 
                                                 event.rightOfWayVehicle.latitude) / 2;
                event.conflictPoint.longitude = (event.encroachingVehicle.longitude + 
                                                  event.rightOfWayVehicle.longitude) / 2;

                // Compute Time-to-Collision
                event.timeToCollision = computeDistance(event.encroachingVehicle, 
                                                        event.rightOfWayVehicle) / 
                                        std::max(event.encroachingVehicle.speed, 
                                                 event.rightOfWayVehicle.speed);

                // Add to conflict events if TTC is below threshold
                if (event.timeToCollision < TTC_UPPER_LIMIT) {
                    conflictEvents.push_back(event);
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error processing line " << lineCount 
                          << ": " << e.what() << std::endl;
            }
        }

        std::cout << "Total conflict events found: " << conflictEvents.size() << std::endl;
    }

    void printConflictAnalysis() {
        if (conflictEvents.empty()) {
            std::cout << "No conflict events found." << std::endl;
            return;
        }

        std::cout << "Conflict Events Analysis:\n";
        for (const auto& event : conflictEvents) {
            std::cout << "Encroaching Vehicle:\n";
            std::cout << "  Latitude: " << event.encroachingVehicle.latitude 
                      << ", Longitude: " << event.encroachingVehicle.longitude << std::endl;
            std::cout << "  Speed: " << event.encroachingVehicle.speed 
                      << ", Approach: " << static_cast<int>(event.encroachingVehicle.approach) << std::endl;
            
            std::cout << "Right-of-Way Vehicle:\n";
            std::cout << "  Latitude: " << event.rightOfWayVehicle.latitude 
                      << ", Longitude: " << event.rightOfWayVehicle.longitude << std::endl;
            std::cout << "  Speed: " << event.rightOfWayVehicle.speed 
                      << ", Approach: " << static_cast<int>(event.rightOfWayVehicle.approach) << std::endl;
            
            std::cout << "Traffic Signal State: " 
                      << static_cast<int>(event.trafficSignalState) << std::endl;
            std::cout << "Time to Collision: " << event.timeToCollision << " s\n";
            std::cout << "Conflict Point: (" 
                      << event.conflictPoint.latitude << ", " 
                      << event.conflictPoint.longitude << ")\n\n";
        }
    }
};

int main() {
    ConflictPointAnalyzer analyzer;

    try {
        // Read vehicle data from a CSV file
        analyzer.readVehicleDataFromFile("vehiclesCLAUD1.csv");

        // Print analysis results
        analyzer.printConflictAnalysis();
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}