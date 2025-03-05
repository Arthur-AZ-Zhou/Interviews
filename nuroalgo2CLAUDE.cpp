#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>

enum class SignalState { GREEN, YELLOW, RED };
enum class ApproachDirection { NORTH, SOUTH, EAST, WEST };
enum class ConflictType { LEFT_TURN_SAME_DIRECTION, RIGHT_TURN_SAME_LANE, LANE_CHANGE }; //0, 1, 2 respectively

struct VehicleData {
    std::string vehicleId;
    double latitude = 0.0;
    double longitude = 0.0;
    double speed = 0.0;
    double acceleration = 0.0;
    double mass = 0.0;
    ApproachDirection approach = ApproachDirection::NORTH;
    int laneNumber = 0;
};

struct DriverBehavior {
    double reactionTime = 0.0;
    double attentiveness = 1.0;
    double aggressiveness = 0.0;
};

class MergingConflictAnalyzer {
private:
    struct ConflictEvent {
        // Timing parameters
        double t1 = 0.0;  // Current time step
        double t2 = 0.0;  // Initial detection time
        double t_arrival = 0.0;
        double t_previous = 0.0;

        // Vehicle data
        VehicleData encroachingVehicle;
        VehicleData rightOfWayVehicle;
        DriverBehavior encroachingDriverBehavior;
        DriverBehavior rightOfWayDriverBehavior;

        // Conflict details
        ConflictType conflictType;
        SignalState trafficSignalState;

        // Conflict point location
        struct ConflictLineLocation {
            double startLatitude = 0.0;
            double startLongitude = 0.0;
            double endLatitude = 0.0;
            double endLongitude = 0.0;
        } conflictLine;

        // Computed metrics
        double timeToCollision = std::numeric_limits<double>::max();
        double postEncroachmentTime = std::numeric_limits<double>::max();
        double maxSpeed = 0.0;
        double deltaSpeed = 0.0;
    };

    // Configuration parameters
    const double TTC_UPPER_LIMIT = 1.5;  // Time-to-Collision upper limit
    const double MAX_CONFLICT_DISTANCE = 100.0;  // meters

    std::vector<ConflictEvent> conflictEvents;

    // Helper methods
    double computeDistance(const VehicleData& v1, const VehicleData& v2) {
        return std::sqrt(std::pow(v1.latitude - v2.latitude, 2) + 
                         std::pow(v1.longitude - v2.longitude, 2));
    }

    bool hasDecelerationEvent(const std::vector<double>& speedProfile) {
        // Check if speed profile indicates deceleration
        return std::adjacent_find(speedProfile.begin(), speedProfile.end(), 
            [](double a, double b) { return a > b; }) != speedProfile.end();
    }

public:
    void analyzeConflictLine(
        const VehicleData& encroachingVehicle, 
        const VehicleData& rightOfWayVehicle,
        ConflictType conflictType,
        SignalState signalState,
        const std::vector<double>& encroachingSpeedProfile,
        const std::vector<double>& rowSpeedProfile
    ) {
        ConflictEvent event;

        // Record initial vehicle and scenario details
        event.encroachingVehicle = encroachingVehicle;
        event.rightOfWayVehicle = rightOfWayVehicle;
        event.conflictType = conflictType;
        event.trafficSignalState = signalState;

        // Compute conflict line starting point (simplified)
        event.conflictLine.startLatitude = (encroachingVehicle.latitude + rightOfWayVehicle.latitude) / 2;
        event.conflictLine.startLongitude = (encroachingVehicle.longitude + rightOfWayVehicle.longitude) / 2;

        // Time-to-Collision computation
        double totalDistance = computeDistance(encroachingVehicle, rightOfWayVehicle);
        event.timeToCollision = totalDistance / 
            std::max(encroachingVehicle.speed, rightOfWayVehicle.speed);

        // Deceleration analysis
        bool hasDeceleration = hasDecelerationEvent(rowSpeedProfile);

        // Speed differential and maximum speed computations
        std::vector<double> speedDifferentials;
        for (size_t i = 0; i < encroachingSpeedProfile.size(); ++i) {
            speedDifferentials.push_back(
                std::abs(encroachingSpeedProfile[i] - rowSpeedProfile[i])
            );
        }

        event.deltaSpeed = *std::max_element(speedDifferentials.begin(), speedDifferentials.end());
        event.maxSpeed = std::max(
            *std::max_element(encroachingSpeedProfile.begin(), encroachingSpeedProfile.end()),
            *std::max_element(rowSpeedProfile.begin(), rowSpeedProfile.end())
        );

        // Validate conflict event based on TTC
        if (event.timeToCollision < TTC_UPPER_LIMIT) {
            conflictEvents.push_back(event);
        }
    }

    void printConflictAnalysis() {
        std::cout << "Merging Flows Conflict Line Analysis:\n";
        for (const auto& event : conflictEvents) {
            std::cout << "Conflict Type: " << static_cast<int>(event.conflictType) << std::endl;
            std::cout << "Encroaching Vehicle ID: " << event.encroachingVehicle.vehicleId << std::endl;
            std::cout << "Right-of-Way Vehicle ID: " << event.rightOfWayVehicle.vehicleId << std::endl;
            std::cout << "Traffic Signal State: " << static_cast<int>(event.trafficSignalState) << std::endl;
            std::cout << "Time to Collision: " << event.timeToCollision << " s\n";
            std::cout << "Max Speed: " << event.maxSpeed << " m/s\n";
            std::cout << "Speed Differential: " << event.deltaSpeed << " m/s\n";
            std::cout << "Conflict Line Start: (" 
                      << event.conflictLine.startLatitude << ", " 
                      << event.conflictLine.startLongitude << ")\n\n";
        }
    }
};

int main() {
    MergingConflictAnalyzer analyzer;

    // Example usage
    VehicleData encroachingVehicle, rightOfWayVehicle;
    
    // Populate vehicle data
    encroachingVehicle.vehicleId = "ENC001";
    encroachingVehicle.latitude = 37.7749;
    encroachingVehicle.longitude = -122.4194;
    encroachingVehicle.speed = 10.5;
    encroachingVehicle.approach = ApproachDirection::NORTH;
    encroachingVehicle.laneNumber = 1;

    rightOfWayVehicle.vehicleId = "ROW001";
    rightOfWayVehicle.latitude = 37.7750;
    rightOfWayVehicle.longitude = -122.4195;
    rightOfWayVehicle.speed = 15.0;
    rightOfWayVehicle.approach = ApproachDirection::SOUTH;
    rightOfWayVehicle.laneNumber = 2;

    // Example speed profiles (simplified)
    std::vector<double> encroachingSpeedProfile = {10.5, 10.2, 9.8, 9.5};
    std::vector<double> rowSpeedProfile = {15.0, 14.8, 14.5, 14.2};

    // Analyze conflict line
    analyzer.analyzeConflictLine(
        encroachingVehicle, 
        rightOfWayVehicle, 
        ConflictType::LEFT_TURN_SAME_DIRECTION,
        SignalState::GREEN,
        encroachingSpeedProfile,
        rowSpeedProfile
    );

    // Print analysis results
    analyzer.printConflictAnalysis();

    return 0;
}