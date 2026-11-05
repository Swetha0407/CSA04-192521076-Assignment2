# Task 2 – CPU Scheduling

## Objective

To implement and analyse Round Robin scheduling, compare different CPU scheduling policies, study the effect of time quantum and recommend a suitable scheduling policy for a video service.

## Workload

| Process | Arrival Time | Burst Time | Priority |
|---|---:|---:|---:|
| P1 | 0 | 8 | 1 |
| P2 | 1 | 4 | 2 |
| P3 | 2 | 9 | 3 |
| P4 | 3 | 5 | 4 |

**Assumption:** A lower priority number represents a higher priority. Therefore, P1 has the highest priority. Priority scheduling is non-preemptive. SJF is non-preemptive, while SRTF is preemptive.

## Performance Formulas

- Turnaround Time (TAT) = Completion Time − Arrival Time
- Waiting Time (WT) = Turnaround Time − Burst Time
- Response Time (RT) = First CPU Start Time − Arrival Time

## Scheduling Algorithm Comparison

| Algorithm | Average TAT | Average WT | Average RT |
|---|---:|---:|---:|
| FCFS | 15.25 | 8.75 | 8.75 |
| Non-preemptive SJF | 14.25 | 7.75 | 7.75 |
| SRTF | 13.00 | 6.50 | 4.25 |
| Non-preemptive Priority | 15.25 | 8.75 | 8.75 |
| Round Robin (q = 3) | 20.00 | 13.50 | 3.00 |

### Gantt Chart Sequences

- **FCFS:** P1 (0–8), P2 (8–12), P3 (12–21), P4 (21–26)
- **SJF:** P1 (0–8), P2 (8–12), P4 (12–17), P3 (17–26)
- **SRTF:** P1 (0–1), P2 (1–5), P4 (5–10), P1 (10–17), P3 (17–26)
- **Priority:** P1 (0–8), P2 (8–12), P3 (12–21), P4 (21–26)
- **Round Robin (q = 3):** P1 (0–3), P2 (3–6), P3 (6–9), P4 (9–12), P1 (12–15), P2 (15–16), P3 (16–19), P4 (19–21), P1 (21–23), P3 (23–26)

### Observations

SRTF produces the lowest average turnaround time (13.00 units) and waiting time (6.50 units) for this workload. Round Robin produces the lowest average response time (3.00 units), providing earlier CPU access to processes. However, its average turnaround and waiting times are higher.

## Round Robin Time-Quantum Experiment

| Quantum | Average TAT | Average WT | Average RT | Dispatches |
|---:|---:|---:|---:|---:|
| 1 | 19.00 | 12.50 | 0.75 | 26 |
| 3 | 20.00 | 13.50 | 3.00 | 10 |
| 5 | 17.50 | 11.00 | 5.50 | 6 |
| 20 | 15.25 | 8.75 | 8.75 | 4 |

A dispatch represents one CPU allocation to a process. A context switch occurs when the CPU changes from one process to another. Therefore, the number of context switches is not necessarily equal to the number of dispatches.

A smaller quantum improves initial response time but increases scheduling frequency. A larger quantum reduces dispatch frequency but can make processes wait longer for their first CPU allocation.

## Starvation and Ageing

Starvation occurs when a process waits indefinitely because other processes are repeatedly selected ahead of it. It can occur in Priority scheduling when higher-priority processes continuously arrive.

Ageing prevents starvation by gradually increasing the priority of processes that have waited for a long time, ensuring that low-priority processes eventually receive CPU time.

## Alternative Concept – Multilevel Feedback Queue (MLFQ)

MLFQ uses multiple ready queues with different priority levels and time quanta. New processes can enter a high-priority queue, while CPU-intensive processes that use their full time quantum may be moved to lower-priority queues.

This allows short interactive tasks to receive quicker CPU service while longer jobs continue executing in lower-priority queues. Ageing or periodic priority boosts can help prevent starvation.

For a video service, MLFQ is a useful alternative when short interactive operations and long video-encoding jobs run together. It can prioritise responsive tasks without permanently preventing longer jobs from executing.

## Complexity Comparison

The following costs represent straightforward implementations and may vary with the data structures used.

| Algorithm | Scheduling Decision Cost | Space |
|---|---|---|
| FCFS | O(1) queue operation | O(n) |
| SJF | O(n) selection | O(n) |
| SRTF | O(n) selection per decision | O(n) |
| Priority | O(n) selection | O(n) |
| Round Robin | O(1) queue operation | O(n) |
| MLFQ | O(1) queue operation per level | O(n) |

Here, n represents the number of processes. More advanced data structures, such as heaps, can reduce selection costs for some algorithms.

## Design Decision for the Video Service

Round Robin is recommended for an interactive video service because it provides fair CPU allocation and prevents a single long-running process from monopolising the processor. With a time quantum of 3, the workload achieves an average response time of 3.00 units.

Although SRTF gives lower average waiting and turnaround times in this test, it requires estimates of remaining execution times and may disadvantage longer jobs. MLFQ is a useful alternative for a mixed workload containing short interactive tasks and long video-encoding operations.

The results are based on the supplied four-process workload and should not be treated as measurements of a production video server.

## Conclusion

The experiment compares FCFS, SJF, SRTF, Priority and Round Robin scheduling. It also demonstrates how the Round Robin time quantum affects response, waiting and turnaround times. Round Robin is selected for its fairness and responsiveness, while MLFQ is considered a suitable alternative for mixed interactive and CPU-intensive workloads.

## Compilation and Execution

```bash
gcc -std=c11 -Wall -Wextra scheduling_comparison.c -o scheduling_comparison
./scheduling_comparison
```

The terminal output should be saved in the Task 2 evidence folder.
