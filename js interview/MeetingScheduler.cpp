#include <bits/stdc++.h>
using namespace std;

class MeetingScheduler {
    /**
     * Meeting scheduler class, individual one for each day
     */
    public:
        MeetingScheduler(vector<pair<int, int>>& dailyMeetings) : dailyMeetings(dailyMeetings), numRequiredRooms(0) {
            /**
             * @brief Constructor to schedule meetings
             * 
             * @param dailyMeetings: a vector of pairs containing start and end times 
             */
        }

        void minNumMeetingRooms() {
            /**
             * @brief Schedules meetings in the minimum number of rooms
             */

            for (auto& [startInterval, endInterval] : dailyMeetings) {
                timeline.push_back({startInterval, 0});
                timeline.push_back({endInterval, 1});
            }

            sort(timeline.begin(), timeline.end());
            int numActiveRooms = 0;

            for (auto& [timestamp, indicator] : timeline) {
                if (indicator == 0) {
                    numActiveRooms++;
                    numRequiredRooms = max(numRequiredRooms, numActiveRooms);
                } else {
                    numActiveRooms--;
                }
            }
        }

        int getNumRequiredRooms() {
            /**
             * @brief returns numRequiredRooms
             * 
             * @returns numRequiredRooms: number of required rooms
             */
            return numRequiredRooms;
        }

    private:
        vector<pair<int, int>> dailyMeetings;
        vector<pair<int, int>> timeline; // first is time, second is 0 for start, 1 for end
        int numRequiredRooms;
};

int main() {
    vector<pair<int, int>> mondayMeetings = {
        {0, 30},
        {5, 10},
        {15, 20},
    };

    vector<pair<int, int>> tuesdayMeetings = {
        {7, 10},
        {2, 4},
    };

    MeetingScheduler mondayScheduler(mondayMeetings);
    MeetingScheduler tuesdayScheduler(tuesdayMeetings);

    mondayScheduler.minNumMeetingRooms();
    tuesdayScheduler.minNumMeetingRooms();

    cout << "Number of rooms for Monday: " << mondayScheduler.getNumRequiredRooms() << endl;
    cout << "Number of rooms for Tuesday: " << tuesdayScheduler.getNumRequiredRooms() << endl;
}