#include <bits/stdc++.h>
using namespace std;

//PART 1==============================
/*
// from typing import List
// # `request_api_call` is called every time the caller wants to make an API call
// def request_api_call(cost: int) -> None:

// # `emit_api_calls` is called by an automated system, once per second.
// # It is guaranteed to be called once per second and will always be the last (or only) call in a given second.
// # `emit_api_calls` should return a list of Costs in the same order that they are requested.
// def emit_api_calls() -> List[int]:

// RATE_LIMIT=4

// # The request_api_call() function can be invoked multiple (from zero to infinite) times within a single second.
// # Calls to emit_api_calls() are made once every second, and will be the last (or only) call each second.
// request_api_call(2) # An API call with a cost of 2 is requested.
// request_api_call(2) # Another API call with a cost of 2 is requested, thus we have [2, 2].

// request_api_call(1) # Continuing, we now have API calls with costs [2, 2, 1].
// request_api_call(1) # [2, 2, 1, 1].
// emit_api_calls() # Returns [2, 2]. Only the first two API calls are processed due to the rate limit of 4.

// request_api_call(3) # In the next second, a call costing 3 is requested, so we have [1, 1, 3].
// emit_api_calls() # Returns [1, 1]. Requests from earlier are processed; the recent request is not, as it would exceed the rate limit.

// # If a time passes without any new request_api_calls, any pending API calls are processed in subsequent seconds.
// emit_api_calls() # Returns [3].
*/

//PART 2==============================
/*
RATE_LIMIT=5 # No more than 5 cost of calls
RATE_LIMIT_WINDOW=3 # In the past 3 seconds

request_api_call(2) 
emit_api_calls() # -> [2]
request_api_call(3)
request_api_call(4) 
request_api_call(5) 
emit_api_calls() # -> [3]. Emitting 3 is ok because 2+3 <= 5.
emit_api_calls() # -> []. We have emitted 5 cost of calls last three seconds, so can't fit a 4 cost call
emit_api_calls() # -> []. We have emitted 3 cost of calls in the last three seconds, so still can't fit a 4 cost call
emit_api_calls() # -> [4]. 3-cost call from time 1 is no longer in the rate limit window, so can afford the 4-cost call
emit_api_calls() # -> [] 
emit_api_calls() # -> [] 
emit_api_calls() # -> [5]
*/

/*
Rate limit = 5
limit window = 3

request_api_call(1)
request_api_call(1)
request_api_call(1)
request_api_call(1)
request_api_call(1)
emit_api_calls() -> {1 1 1 1 1}
emit_api_calls()
emit_api_calls()
request_api_call(5)
emit_api_calls() -> {5}
*/

/*
RATE_LIMIT=3
RATE_LIMIT_WINDOW=1 # (no windowing to keep the example simple)

# 4 requests come in at time 0
request_api_call(2) # Request "a"
request_api_call(2) # Request "b"
request_api_call(1) # Request "c"
request_api_call(1) # Request "d"

# Naive implementation
emit_api_calls() # -> [2] Request "a" was scheduled at T=0 and emitted at T=0, so latency here is 0
emit_api_calls() # -> [2,1] Request "b" and "c" were scheduled at T=0 and emitted at T=1, so latency here for both is 1
emit_api_calls() # -> [1] Request "d" was scheduled at T=0 and emitted at T=2, so latency here is 2
# average latency is (0 + 1 + 1 + 2) / 4 = 4/4

# With a different implementation
emit_api_calls() # ->[1,1] Request "c" and "d" were scheduled at T=0 and emitted at T=0, so latency here for both is 0
emit_api_calls() # ->[2]. Request "a" was scheduled at T=0 and emitted at T=1, so latency here is 1
emit_api_calls() # ->[2]. Request "b" was scheduled at T=0 and emitted at T=2, so latency here is 2
# average latency is (0 + 0 + 1 + 2) / 4 = 3/4
*/

/*
def request_api_call(cost: int) -> None:

def request_background_api_call(cost: int) -> None:

RATE_LIMIT=4
RATE_LIMIT_WINDOW=1 # (no windowing to keep the example simple)

request_api_call(3) 
request_background_api_call(2)
request_background_api_call(2)
emit_api_calls() # -> ([3], []) # prioritize the user-present api call
emit_api_calls() # -> ([], [2,2]) # then schedule the background calls
*/

const int RATE_LIMIT = 3;
const int RATE_LIMIT_WINDOW = 1;

class RateLimiter {
    public: 
    RateLimiter(int rL, int rLW) : rateLimit(rL), limitWindow(rLW), currentCost(0), windowSize(0) {};

    /**
     * @brief pushes cost to back of queueCost
     * @param cost that we are pushing
     */
    void requestApiCall(int cost) {
        if (rateLimit < cost) {
            cout << "REJECTED COST OF: " << cost << " FOR BEING GREATER THAN RATELIMIT" << endl;
            return;
        }

        queueCost.push(cost);
    }

    void requestBackgroundApiCall(int cost) {

    }

    /**
     * @brief: happens last, prints out until hit rateLimit
     * @return: list of costs that are within rateLimit
     */
    pair<vector<int>, vector<int>> emitApiCalls() {
        vector<int> apiCallsCosts;
        int oldTotalCost = 0;

        if (windowSize == limitWindow && !oldCosts.empty()) {
            currentCost -= oldCosts.front();
            oldCosts.pop();
        } else {
            windowSize++; //grow to limitWindow
        }

        while (!queueCost.empty() && currentCost < rateLimit && currentCost + queueCost.top() <= rateLimit) {
            currentCost += queueCost.top();
            apiCallsCosts.push_back(queueCost.top());
            oldTotalCost += queueCost.top();

            queueCost.pop();
        }

        oldCosts.push(oldTotalCost);

        // while (!queueCost.empty() && currentCost < rateLimit && currentCost + queueCost.front() <= rateLimit) {
        //     currentCost += queueCost.front();
        //     apiCallsCosts.push_back(queueCost.front());
        //     queueCost.pop();
        // }

        //PRINT EMITTED CALLS================
        cout << "{";
        for (int calls : apiCallsCosts) {
            cout << calls << " ";
        }
        cout << "}\n";

        return apiCallsCosts;
    }

    private: 
    priority_queue <int, vector<int>, greater<int>> queueCost; //minHeap
    queue<int> oldCosts;
    int rateLimit;
    int limitWindow;
    int internalTime;
    int currentCost;
    int windowSize;
};

int main() {
    RateLimiter testRateLimiter(RATE_LIMIT, RATE_LIMIT_WINDOW);



    // testRateLimiter.requestApiCall(2);
    // testRateLimiter.requestApiCall(2);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);

    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();

    // testRateLimiter.requestApiCall(2);

    // testRateLimiter.emitApiCalls();

    // testRateLimiter.requestApiCall(3);
    // testRateLimiter.requestApiCall(4);
    // testRateLimiter.requestApiCall(5);

    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();

    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);

    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();

    // testRateLimiter.requestApiCall(5);

    // testRateLimiter.emitApiCalls();

    // testRateLimiter.requestApiCall(2);
    // testRateLimiter.requestApiCall(2);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.requestApiCall(1);
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.requestApiCall(3);
    // testRateLimiter.emitApiCalls();
    // testRateLimiter.emitApiCalls();
}