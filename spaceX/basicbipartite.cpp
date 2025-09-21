#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief A DFS-based recursive function to find a matching for an applicant.
 * @param adj The adjacency list representing the bipartite graph. adj[u] contains the jobs applicant 'u' is interested in.
 * @param applicant The current applicant (vertex from the left set) we are trying to match.
 * @param seen A vector to keep track of jobs visited in the current DFS traversal.
 * @param matches A vector where matches[j] stores the applicant matched to job 'j'.
 * @return true if a match is found for the applicant, false otherwise.
 */
bool findMatch(const vector<vector<int>>& adj, int applicant, vector<bool>& seen, vector<int>& matches) {
    // Try every job the current applicant is interested in.
    for (int job : adj[applicant]) {
        // If this job has not been considered in the current DFS run
        if (!seen[job]) {
            // Mark the job as considered
            seen[job] = true;

            // If the job is available OR the applicant currently holding the job
            // can be matched with a different job, then we have a match.
            if (matches[job] < 0 || findMatch(adj, matches[job], seen, matches)) {
                matches[job] = applicant;
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Finds the maximum number of matchings in a bipartite graph.
 * @param adj The adjacency list of the graph.
 * @param numJobs The total number of jobs (vertices in the right set).
 * @return The count of maximum possible matchings.
 */
int maxBipartiteMatching(const vector<vector<int>>& adj, int numJobs) {
    int numApplicants = adj.size();

    // matches[j] stores the applicant assigned to job 'j'. Initialize all to -1 (unassigned).
    vector<int> matches(numJobs, -1);

    int result = 0;
    // Iterate through all applicants to find a match for each.
    for (int applicant = 0; applicant < numApplicants; ++applicant) {
        // For each applicant, we need a fresh 'seen' array for the DFS.
        vector<bool> seen(numJobs, false);

        // If a match can be found for the current applicant
        if (findMatch(adj, applicant, seen, matches)) {
            result++;
        }
    }

    // Optional: Print the final matches
    cout << "Final Job Assignments (Job -> Applicant):" << endl;
    for (int i = 0; i < numJobs; ++i) {
        if (matches[i] != -1) {
            cout << "  Job " << i << " -> Applicant " << matches[i] << endl;
        }
    }
    cout << "------------------------------------------" << endl;

    return result;
}

// Driver Code
int main() {
    // The graph is represented as an adjacency list.
    // The index of the outer vector represents the applicant.
    // The inner vector contains the jobs they are interested in.
    int numApplicants = 6;
    int numJobs = 6;

    vector<vector<int>> bipartiteGraph(numApplicants);
    bipartiteGraph[0] = {1, 2};
    bipartiteGraph[1] = {0, 3};
    bipartiteGraph[2] = {2};
    bipartiteGraph[3] = {2, 3};
    bipartiteGraph[4] = {}; // Applicant 4 is interested in no jobs
    bipartiteGraph[5] = {5};

    cout << "Maximum number of applicants that can get a job is "
              << maxBipartiteMatching(bipartiteGraph, numJobs) << endl;

    return 0;
}

