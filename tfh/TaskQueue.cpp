#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class TaskQueue {
private:
    unordered_map<int, queue<string>> taskMap; // prio -> list of tasks
    priority_queue<int> priorityHeap; // what prios we have
    unordered_map<string, int> tasksInQueue; // taskId -> prio, check for dupes
    // unordered_map<string, unordered_set<int>> cancelledTasks; //taskId -> {prio numbers}

public:
    TaskQueue() {}

    bool addTask(const string& taskId, int priority) {
        if (tasksInQueue.find(taskId) != tasksInQueue.end() || taskId == "" || priority < 0) {
            return false;
        }

        tasksInQueue[taskId] = priority;
        if (taskMap.find(priority) == taskMap.end()) { // need to push prio to heap if not there alr
            priorityHeap.push(priority);
        }
        taskMap[priority].push(taskId);
        
        return true;
    }  

    string popNext() {
        if (priorityHeap.empty()) {
            return "";
        }

        int prioNum;
        string nextTask = "";

        while (!priorityHeap.empty()) {
            prioNum = priorityHeap.top();
            nextTask = taskMap[prioNum].front();

            cout << "nextTask: " << nextTask << endl;

            taskMap[prioNum].pop();

            if (tasksInQueue.find(nextTask) == tasksInQueue.end() || tasksInQueue[nextTask] != prioNum) { //found cancelled
                cout << "found cancelled task: " << nextTask << ": " << prioNum << endl;
                if (taskMap[prioNum].empty()) {
                    priorityHeap.pop();
                    cout << "new pq top: " << priorityHeap.top() << endl;
                }

            } else {
                tasksInQueue.erase(nextTask);

                if (taskMap[prioNum].empty()) {
                    taskMap.erase(prioNum);
                    priorityHeap.pop();
                }

                break;
            }
        }

        return nextTask;
    }

    bool cancelTask(const string& taskId) {
        /*
        The problem is we can't remove tasks from taskMap since that will take too long
        */
        if (tasksInQueue.find(taskId) == tasksInQueue.end()) {
            return false;
        }

        cout << "erased: " << taskId << endl;
        tasksInQueue.erase(taskId);

        return true;
    }
};

int main() {
    TaskQueue queue;

    // queue.addTask("generate-report", 2);
    // queue.addTask("send-email", 1);
    // queue.addTask("security-scan", 3);
    // queue.addTask("backup", 2);

    // cout << queue.popNext() << endl; // security-scan
    // cout << queue.popNext() << endl; // generate-report
    // cout << queue.popNext() << endl; // backup
    // cout << queue.popNext() << endl; // send-email
    // cout << queue.popNext() << endl; // ""

    // cout << queue.addTask("job-1", 5) << endl;  // true
    // cout << queue.addTask("job-1", 10) << endl; // false
    // cout << queue.popNext() << endl;            // job-1
    // cout << queue.addTask("job-1", 10) << endl; // true

    // queue.addTask("A", 10);
    // queue.cancelTask("A");
    // queue.addTask("A", 1);
    // queue.addTask("B", 5);

    // cout << queue.popNext() << endl; // B
    // cout << queue.popNext() << endl; // A
    // cout << queue.popNext() << endl; // ""

    queue.addTask("A", 10);
    queue.addTask("X", 10);
    queue.cancelTask("A");
    queue.addTask("A", 10);

    cout << queue.popNext() << endl; // Should be X
    cout << queue.popNext() << endl; // Should be A
}