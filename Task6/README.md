# Task 6 – Deadlock Handling and Banker's Algorithm

## Objective

To study deadlock conditions, implement Banker's algorithm for deadlock avoidance, evaluate resource requests and compare different deadlock-handling strategies for a shared research-centre cluster.

## A. Four Necessary Conditions for Deadlock

Deadlock can occur when all four of the following conditions exist simultaneously:

1. **Mutual Exclusion:** A resource can be allocated to only one process at a time.
2. **Hold and Wait:** A process holds resources while waiting for additional resources.
3. **No Preemption:** Resources cannot be forcibly removed from a process.
4. **Circular Wait:** A circular chain exists in which every process waits for a resource held by another process.

### Resource-Allocation Graph

Consider three jobs and three resources:

- P1 holds a GPU and requests a licence.
- P2 holds a licence and requests storage.
- P3 holds storage and requests a GPU.

The circular dependency is:

```text
P1 → Licence → P2 → Storage → P3 → GPU → P1
```

This demonstrates circular wait and, with single-instance resources in the cycle, deadlock.

*Insert the resource-allocation graph diagram here.*

## B. Comparison of Deadlock-Handling Strategies

| Strategy | Description | Limitation |
|---|---|---|
| Prevention | Removes one of the four necessary deadlock conditions | Can restrict resource allocation |
| Avoidance | Checks whether each request maintains a safe state | Requires maximum resource demands in advance |
| Detection and recovery | Detects deadlocks and recovers by aborting or rolling back jobs | Detection and recovery consume resources and may waste work |
| Ignoring deadlock | Does not actively handle deadlock | Can leave jobs blocked until administrator intervention |

For the research centre, avoidance is appropriate when resource demands are predictable. Detection and recovery are useful when workloads are dynamic.

## C. Banker's Algorithm Implementation

The C program implements the safety algorithm and resource-request algorithm using the supplied five-process, three-resource dataset.

The safety algorithm calculates the remaining need of each process:

Need = Maximum − Allocation

It checks whether a process can finish using the currently available resources. If it can, its allocated resources are released into the work vector and the process is added to the safe sequence.

### Initial Safe Sequence

```text
initial state safe: P1 P3 P4 P0 P2
```

### Resource Requests

| Process | Request | Result |
|---|---|---|
| P1 | (1, 0, 2) | Granted |
| P4 | (3, 3, 0) | Must wait |
| P0 | (0, 2, 0) | Denied (unsafe) |

The P1 request is granted because the resulting state remains safe. P4 must wait because three GPUs are not available. P0's request is within the available resources but makes the state unsafe, so the temporary allocation is rolled back.

### Why P0's Request Is Unsafe

After granting P1's request, the available vector is (2, 3, 0). P0 requests (0, 2, 0), resulting in available resources of (2, 1, 0).

The remaining needs are:

| Process | Remaining Need |
|---|---|
| P0 | (7, 2, 3) |
| P1 | (0, 2, 0) |
| P2 | (6, 0, 0) |
| P3 | (0, 1, 1) |
| P4 | (4, 3, 1) |

No process can complete with the available resources. Therefore, no safe sequence exists, and the request is denied.

## D. Complexity Prediction

Let n be the number of processes and m be the number of resource types.

The safety algorithm may examine all n processes repeatedly. For each process, it checks m resource types.

Worst-case time complexity:

O(m × n²)

The resource-request algorithm performs a safety check after temporarily allocating resources, so its worst-case safety-check complexity is also O(m × n²).

## E. Performance Measurement

The program measures the safety algorithm for 10, 100 and 1,000 processes. The benchmark uses a deliberately constructed safe sequence that requires repeated scans, with one resource type held constant.

*Insert the actual terminal measurements here.*

The measured execution time is expected to increase as the number of processes increases. This is consistent with the predicted quadratic growth when the number of resource types is fixed.

## Alternative: Deadlock Detection and Recovery

An alternative approach is to allow resource allocation and periodically execute a deadlock-detection algorithm. If a deadlock is detected, the system can abort a selected job, roll it back to a checkpoint or release its resources through recovery.

This approach supports unpredictable workloads but may waste completed work and introduce detection overhead.

## F. Final Design Decision

Banker's algorithm is recommended when research jobs have known maximum resource demands. It checks whether resource allocation preserves a safe state before granting requests.

Its main limitation is the O(m × n²) safety-check cost and the need to know maximum resource requirements in advance.

For dynamic workloads, periodic detection and checkpoint-based recovery provide a more flexible alternative. The final design should therefore use Banker's avoidance for predictable resource-intensive jobs and retain recovery mechanisms for exceptional situations.

## Compilation and Execution

```bash
gcc -std=c11 -O2 -Wall -Wextra bankers_algorithm.c -o bankers_algorithm
./bankers_algorithm
```

## Conclusion

The experiment demonstrates how Banker's algorithm avoids unsafe resource allocation by checking for a safe sequence before granting requests. It also compares prevention, avoidance, detection and recovery, and ignoring deadlock. The final recommendation considers both resource safety and the computational cost of deadlock avoidance.
