#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>

enum class SignalState { GREEN, YELLOW, RED };
enum class ApproachDirection { NORTH, SOUTH, EAST, WEST };
enum class MovementType { LEFT, RIGHT, DIAGONAL, STRAIGHT };

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

class RearEndConflictAnalyzer {
private:
    struct ConflictEvent {
        // Timing parameters
        double t1 = 0.0;  // Current time step
        double t2 = 0.0;  // Initial detection time
        double t_arrival = 0.0;
        double t_previous = 0.0;

        // Vehicle data
        VehicleData leadingVehicle;
        VehicleData followingVehicle;
        
        // Movement and behavior details
        MovementType leadingVehicleMovement;
        DriverBehavior leadingDriverBehavior;
        DriverBehavior followingDriverBehavior;

        // Scenario context
        SignalState trafficSignalState;

        // Conflict line location
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
        bool hasUnexpectedBraking = false;
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
        // Detect significant deceleration
        for (size_t i = 1; i < speedProfile.size(); ++i) {
            if ((speedProfile[i-1] - speedProfile[i]) > 2.0) {  // Significant speed drop
                return true;
            }
        }
        return false;
    }

    bool isNormalSignalReaction(SignalState signalState, 
                                const std::vector<double>& speedProfile) {
        // Check if deceleration matches expected signal state reaction
        if (signalState == SignalState::YELLOW || signalState == SignalState::RED) {
            return hasDecelerationEvent(speedProfile);
        }
        return false;
    }

public:
    void analyzeRearEndConflict(
        const VehicleData& leadingVehicle, 
        const VehicleData& followingVehicle,
        MovementType leadingVehicleMovement,
        SignalState signalState,
        const std::vector<double>& leadingSpeedProfile,
        const std::vector<double>& followingSpeedProfile
    ) {
        ConflictEvent event;

        // Record initial vehicle and scenario details
        event.leadingVehicle = leadingVehicle;
        event.followingVehicle = followingVehicle;
        event.leadingVehicleMovement = leadingVehicleMovement;
        event.trafficSignalState = signalState;

        // Compute conflict line starting point (simplified)
        event.conflictLine.startLatitude = (leadingVehicle.latitude + followingVehicle.latitude) / 2;
        event.conflictLine.startLongitude = (leadingVehicle.longitude + followingVehicle.longitude) / 2;

        // Time-to-Collision computation
        double totalDistance = computeDistance(leadingVehicle, followingVehicle);
        event.timeToCollision = totalDistance / 
            std::max(leadingVehicle.speed, followingVehicle.speed);

        // Deceleration analysis
        bool hasDeceleration = hasDecelerationEvent(followingSpeedProfile);
        bool isNormalDeceleration = isNormalSignalReaction(signalState, followingSpeedProfile);
        
        event.hasUnexpectedBraking = hasDeceleration && !isNormalDeceleration;

        // Speed differential and maximum speed computations
        std::vector<double> speedDifferentials;
        for (size_t i = 0; i < leadingSpeedProfile.size(); ++i) {
            speedDifferentials.push_back(
                std::abs(leadingSpeedProfile[i] - followingSpeedProfile[i])
            );
        }

        event.deltaSpeed = *std::max_element(speedDifferentials.begin(), speedDifferentials.end());
        event.maxSpeed = std::max(
            *std::max_element(leadingSpeedProfile.begin(), leadingSpeedProfile.end()),
            *std::max_element(followingSpeedProfile.begin(), followingSpeedProfile.end())
        );

        // Validate conflict event based on TTC
        if (event.timeToCollision < TTC_UPPER_LIMIT) {
            conflictEvents.push_back(event);
        }
    }

    void printConflictAnalysis() {
        std::cout << "Rear-End Conflict Line Analysis:\n";
        for (const auto& event : conflictEvents) {
            std::cout << "Leading Vehicle ID: " << event.leadingVehicle.vehicleId << std::endl;
            std::cout << "Following Vehicle ID: " << event.followingVehicle.vehicleId << std::endl;
            std::cout << "Leading Vehicle Movement: " << static_cast<int>(event.leadingVehicleMovement) << std::endl;
            std::cout << "Traffic Signal State: " << static_cast<int>(event.trafficSignalState) << std::endl;
            std::cout << "Time to Collision: " << event.timeToCollision << " s\n";
            std::cout << "Max Speed: " << event.maxSpeed << " m/s\n";
            std::cout << "Speed Differential: " << event.deltaSpeed << " m/s\n";
            std::cout << "Unexpected Braking: " << (event.hasUnexpectedBraking ? "Yes" : "No") << std::endl;
            std::cout << "Conflict Line Start: (" 
                      << event.conflictLine.startLatitude << ", " 
                      << event.conflictLine.startLongitude << ")\n\n";
        }
    }
};

int main() {
    RearEndConflictAnalyzer analyzer;

    // Example usage
    VehicleData leadingVehicle, followingVehicle;
    
    // Populate vehicle data
    leadingVehicle.vehicleId = "LEAD001";
    leadingVehicle.latitude = 37.7749;
    leadingVehicle.longitude = -122.4194;
    leadingVehicle.speed = 10.5;
    leadingVehicle.approach = ApproachDirection::NORTH;
    leadingVehicle.laneNumber = 1;

    followingVehicle.vehicleId = "FOLLOW001";
    followingVehicle.latitude = 37.7750;
    followingVehicle.longitude = -122.4195;
    followingVehicle.speed = 15.0;
    followingVehicle.approach = ApproachDirection::NORTH;
    followingVehicle.laneNumber = 1;

    // Example speed profiles (simplified)
    std::vector<double> leadingSpeedProfile = {10.5, 10.2, 9.8, 9.5};
    std::vector<double> followingSpeedProfile = {15.0, 14.8, 14.5, 12.0};

    // Analyze rear-end conflict
    analyzer.analyzeRearEndConflict(
        leadingVehicle, 
        followingVehicle, 
        MovementType::LEFT,
        SignalState::GREEN,
        leadingSpeedProfile,
        followingSpeedProfile
    );

    // Print analysis results
    analyzer.printConflictAnalysis();

    return 0;
}