#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    char id;
    int arrival_time, burst_time, priority, remaining_time, queue_level;
};

bool compareArrival(Process a, Process b) {
    return a.arrival_time < b.arrival_time;
}

void FCFS(queue<Process>& q, int& current_time, vector<int>& completion_time) {
    while (!q.empty()) {
        Process proc = q.front();
        q.pop();

        if (current_time < proc.arrival_time) current_time = proc.arrival_time;

        current_time += proc.burst_time;
        completion_time[proc.id - 'A'] = current_time;
    }
}

void RoundRobin(queue<Process>& q, int& current_time, vector<int>& completion_time, int time_quantum) {
    while (!q.empty()) {
        Process proc = q.front();
        q.pop();

        if (current_time < proc.arrival_time) current_time = proc.arrival_time;

        if (proc.remaining_time > time_quantum) {
            proc.remaining_time -= time_quantum;
            current_time += time_quantum;
            q.push(proc);
        } else {
            current_time += proc.remaining_time;
            completion_time[proc.id - 'A'] = current_time;
        }
    }
}

int main() {
    int n, time_quantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for process " << char('A' + i) << ":\n";
        processes[i].id = 'A' + i;
        cout << "Arrival time: ";
        cin >> processes[i].arrival_time;
        cout << "Burst time: ";
        cin >> processes[i].burst_time;

        cout << "Priority number (any positive integer): ";
        cin >> processes[i].priority;

        cout << "Queue Level (1: High, 2: Medium, 3: Low): ";
        cin >> processes[i].queue_level;

        processes[i].remaining_time = processes[i].burst_time;
    }

    cout << "\nEnter the time quantum for Round Robin scheduling: ";
    cin >> time_quantum;

    sort(processes.begin(), processes.end(), compareArrival);

    queue<Process> high, medium, low;
    vector<int> completion_time(n, 0);
    int current_time = 0;

    for (auto& proc : processes) {
        if (proc.queue_level == 1) {
            high.push(proc);
        }
        else if (proc.queue_level == 2) {
            medium.push(proc);
        }
        else {
            low.push(proc);
        }
    }

    FCFS(high, current_time, completion_time);
    RoundRobin(medium, current_time, completion_time, time_quantum);
    FCFS(low, current_time, completion_time);

    printf("\n%-6s %-10s %-10s %-8s %-8s %-10s %-10s %-10s\n",
           "P", "AT", "BT", "PRIO", "QL", "CT", "TAT", "WT");

    int total_turnaround = 0, total_waiting = 0;

    for (auto& proc : processes) {
        int turnaround_time = completion_time[proc.id - 'A'] - proc.arrival_time;
        int waiting_time = turnaround_time - proc.burst_time;
        total_turnaround += turnaround_time;
        total_waiting += waiting_time;

        printf("%-6c %-10d %-10d %-8d %-8d %-10d %-10d %-10d\n", 
               proc.id, 
               proc.arrival_time, 
               proc.burst_time, 
               proc.priority, 
               proc.queue_level, 
               completion_time[proc.id - 'A'],
               turnaround_time,
               waiting_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", (double)total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", (double)total_waiting / n);

    return 0;
}
