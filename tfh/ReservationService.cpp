#include <bits/stdc++.h>
#include <iostream>

using namespace std;

/*
addRoom creates a room.
Room IDs must be nonempty and unique.
Reservation IDs must be nonempty and globally unique.
The room must already exist.
A valid interval requires startTime >= 0 and startTime < endTime.
Reservations use half-open intervals: [startTime, endTime).
Reservations for the same room cannot overlap.
Touching reservations are allowed:- [10,20) and [20,30) do not overlap.

Reservations may be submitted in any chronological order.
A failed booking must not mutate state.
Aim for O(logN) booking time, where N is the number of reservations in that room.
*/

class ReservationService {
private:
    unordered_map<string, map<long long, long long>> roomBookings;
    unordered_set<string> reservationIds; //globally unique reservation IDs

public:
    ReservationService() {}

    bool addRoom(const string& roomId) {
        if (roomId.empty() || roomBookings.find(roomId) != roomBookings.end()) {
            return false;
        }

        roomBookings[roomId] = {};
        return true;
    }

    bool book(
        const string& reservationId, 
        const string& roomId, 
        long long startTime, 
        long long endTime
    ) {
        if (reservationId.empty() ||
            reservationIds.find(reservationId) != reservationIds.end() ||
            startTime < 0 ||
            startTime >= endTime
        ) {
            return false;
        }

        auto roomIt = roomBookings.find(roomId);

        if (roomIt == roomBookings.end()) {
            return false;
        }

        auto& bookings = roomIt->second;

        // First reservation whose start time is >= the new start time.
        auto next = bookings.lower_bound(startTime);

        // Check overlap with the next reservation.
        if (next != bookings.end() && next->first < endTime) {
            return false;
        }

        // Check overlap with the previous reservation.
        if (next != bookings.begin()) {
            auto previous = prev(next);

            if (previous->second > startTime) {
                return false;
            }
        }

        bookings[startTime] = endTime;
        reservationIds.insert(reservationId);

        return true;
    }
};

int main() {
    ReservationService service;

    cout << service.addRoom("room-a") << endl; // 1
    cout << service.addRoom("room-b") << endl; // 1
    cout << service.addRoom("room-a") << endl; // 0
    cout << service.addRoom("") << endl;       // 0

    cout << service.book("r1", "room-a", 10, 20) << endl; // 1
    cout << service.book("r2", "room-a", 20, 30) << endl; // 1
    cout << service.book("r3", "room-a", 5, 10) << endl;  // 1

    cout << service.book("r4", "room-a", 15, 25) << endl; // 0
    cout << service.book("r5", "room-a", 9, 11) << endl;  // 0

    cout << service.book("r6", "room-b", 15, 25) << endl; // 1
    cout << service.book("r1", "room-b", 30, 40) << endl; // 0
    cout << service.book("r7", "unknown", 30, 40) << endl; // 0
    cout << service.book("r8", "room-a", 40, 40) << endl; // 0
}