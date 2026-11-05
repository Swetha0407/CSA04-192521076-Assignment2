# Task 2 – CPU Scheduling

## Objective
To implement and analyse Round Robin CPU scheduling using process arrival times, burst times and a fixed time quantum.

## Algorithm
Round Robin scheduling assigns each process a fixed CPU time quantum. If a process is not completed within its allocated time, it is placed at the end of the ready queue.

## Input
- Processes: P1, P2, P3, P4
- Arrival times: 0, 1, 2, 3
- Burst times: 8, 4, 9, 5
- Time quantum: 3 units

## Implementation
The C program maintains a ready queue, executes processes for up to three time units, and calculates completion time, turnaround time and waiting time.

## Results
- Average Turnaround Time: 20.00 units
- Average Waiting Time: 13.50 units

## Design Decision
Round Robin is recommended for the video service because it provides fair CPU allocation and regular execution opportunities for multiple active processes. Although the calculated averages may not be the lowest among scheduling policies, its fairness and responsiveness make it suitable for interactive workloads.

## Conclusion
The experiment successfully implements Round Robin scheduling and calculates the average turnaround and waiting times for four processes.