# CSA04 – Operating Systems
## Assignment 2: Operating System Concepts and Practical Implementations

**Student:** Swetha S  
**Register Number:** 192521076  
**Course:** CSA04 – Operating Systems

---

## Project Overview

This repository contains the practical implementations and analysis completed for Operating Systems Assignment 2. The assignment explores fundamental operating-system concepts through C programming, process management, CPU scheduling, multithreading, inter-process communication, synchronisation and deadlock handling.

The project combines theoretical analysis with executable programs and experimental observations to demonstrate how operating systems manage processes, coordinate concurrent activities, allocate resources and maintain system reliability.

## Objectives

- Understand process creation, execution and termination.
- Implement and evaluate CPU scheduling algorithms.
- Explore multithreading and race conditions.
- Compare inter-process communication techniques.
- Implement synchronisation using semaphores and mutexes.
- Understand critical sections and concurrency problems.
- Implement Banker's algorithm for deadlock avoidance.
- Analyse performance and justify operating-system design decisions.

---

## Repository Structure

```text
CSA04-192521076-Assignment2/
│
├── Task01/
│   ├── process_demo.c
│   └── Evidence/
│
├── Task02/
│   ├── round_robin.c
│   ├── README.md
│   └── Evidence/
│
├── Task03/
│   ├── camera_threads.c
│   ├── race_condition.c
│   ├── README.md
│   └── Evidence/
│
├── Task04/
│   ├── pipe_ipc.c
│   ├── shared_memory_ipc.c
│   ├── README.md
│   └── Evidence/
│
├── Task05/
│   ├── producer_consumer.c
│   ├── sync_benchmark.c
│   ├── README.md
│   └── Evidence/
│
├── Task06/
│   ├── bankers_algorithm.c
│   ├── README.md
│   └── Evidence/
│
└── README.md
```

*The repository contains the source files, task documentation and execution evidence for the completed experiments.*

---

## Task Details

### Task 1 – Process Creation and Management

**Concepts:** Process creation, parent-child relationships, process identifiers and process termination.

Implemented a C program using the `fork()` system call to create a child process. The program demonstrates parent and child execution, displays their process IDs and illustrates process completion using `wait()`/`waitpid()`.

**Key concepts demonstrated:**
- Process creation using `fork()`.
- Parent and child process identification.
- Process execution and termination.
- Synchronisation between parent and child processes.

### Task 2 – CPU Scheduling

**Concepts:** Round Robin scheduling, time quantum, turnaround time and waiting time.

Implemented the Round Robin CPU scheduling algorithm using four processes with different arrival and burst times and a time quantum of 3.

The program generates a Gantt chart and calculates turnaround time (TAT), waiting time (WT), average turnaround time and average waiting time.

**Calculated results:**

| Process | Turnaround Time | Waiting Time |
|---|---:|---:|
| P1 | 23 | 15 |
| P2 | 15 | 11 |
| P3 | 24 | 15 |
| P4 | 18 | 13 |

- Average Turnaround Time: **20.00**
- Average Waiting Time: **13.50**

The scheduling decision is evaluated in the context of responsiveness and fairness for a video-service workload.

### Task 3 – Multithreading and Race Conditions

**Concepts:** Threads, concurrency, shared resources and race conditions.

Implemented multithreading experiments to demonstrate concurrent execution and the problems that can occur when multiple threads access shared data.

The task includes a camera-thread simulation and a race-condition demonstration.

**Key concepts demonstrated:**
- Thread creation and execution using POSIX threads.
- Concurrent processing.
- Shared-variable access.
- Race-condition behaviour.
- Importance of synchronisation in multithreaded applications.

### Task 4 – Inter-Process Communication

**Concepts:** Pipes, shared memory and process communication.

Implemented two IPC approaches to demonstrate communication between processes:

1. **Pipe-based communication:** Transfers weather-sensor readings between a producer and a consumer process.
2. **Shared-memory communication:** Uses `mmap()` and semaphores to coordinate data transfer between processes.

The experiments investigate data transfer at different sizes: 1 MB, 10 MB and 100 MB.

**Comparison:**

| Feature | Pipes | Shared Memory |
|---|---|---|
| Communication | Kernel-managed channel | Shared memory region |
| Synchronisation | Managed through pipe operations | Requires explicit synchronisation |
| Data transfer | Kernel-buffered | Direct access to shared mapping |
| Complexity | Relatively simple | Requires coordination |

The measured results are documented in the corresponding task report.

### Task 5 – Synchronisation: Critical Sections, Semaphores and Monitors

**Concepts:** Mutual exclusion, semaphores, mutexes, producer-consumer problem and synchronisation performance.

Implemented a bounded-buffer producer-consumer system using four producers, four consumers and a buffer of size five.

The experiment transfers 20 items and verifies the expected sum of 210 over 100 repetitions.

Additional benchmarking compares:
- Atomic operations using `atomic_fetch_add()`.
- Spinlocks using `atomic_flag`.
- Mutex-based synchronisation.

The task also examines Peterson's algorithm, the readers-writers problem, the dining-philosophers problem and monitor-based synchronisation.

**Key objective:** Demonstrate correct coordination between concurrent threads while preventing race conditions and inconsistent shared data.

### Task 6 – Deadlock Handling and Banker's Algorithm

**Concepts:** Deadlock conditions, resource-allocation graphs, safe states and deadlock avoidance.

Implemented Banker's algorithm in C using the supplied five-process and three-resource dataset.

The program includes:
- Safety algorithm to identify a safe sequence.
- Resource-request algorithm to evaluate allocation requests.
- Temporary allocation and rollback when a request produces an unsafe state.
- Performance measurements for 10, 100 and 1,000 processes.

**Initial safe sequence:**

```text
P1 → P3 → P4 → P0 → P2
```

The experiment evaluates resource requests and demonstrates how Banker's algorithm prevents unsafe resource allocation.

The task also compares deadlock prevention, avoidance, detection and recovery, and the strategy of ignoring deadlocks.

---

## Technologies and Tools Used

| Technology | Purpose |
|---|---|
| C | Implementation of operating-system algorithms |
| GCC | Compilation of C programs |
| POSIX Threads (pthreads) | Multithreading and synchronisation |
| Linux / Ubuntu (WSL) | Execution and testing environment |
| System calls | Process management and IPC |
| POSIX semaphores | Process and thread synchronisation |
| Git and GitHub | Version control and project management |
| VS Code | Source-code development |

---

## Compilation and Execution

The programs were developed and tested using GCC in Ubuntu through Windows Subsystem for Linux (WSL).

General compilation format:

```bash
gcc -std=c11 -Wall -Wextra filename.c -o output
./output
```

For programs using POSIX threads and semaphores:

```bash
gcc -std=c11 -Wall -Wextra -pthread filename.c -o output
./output
```

For performance-oriented experiments:

```bash
gcc -std=c11 -O2 -Wall -Wextra -pthread filename.c -o output
./output
```

Navigate to the relevant task directory before compiling and executing its program.

---

## Experimental Evidence

Each task includes an `Evidence` directory containing screenshots of relevant program execution and experimental results.

The evidence demonstrates:
- Successful compilation and execution.
- Process and thread behaviour.
- Scheduling calculations.
- IPC communication.
- Synchronisation correctness.
- Deadlock-avoidance decisions.
- Performance measurements.

Refer to the individual task directories for source code, detailed explanations and execution evidence.

---

## Learning Outcomes

Through this assignment, the following operating-system concepts were explored practically:

1. Understanding how processes are created and managed.
2. Evaluating CPU scheduling behaviour using calculated performance metrics.
3. Understanding concurrency and the challenges of shared-resource access.
4. Comparing IPC techniques for communication between processes.
5. Applying synchronisation mechanisms to maintain data consistency.
6. Identifying deadlock conditions and implementing safe resource allocation.

## Conclusion

This assignment provided practical experience with fundamental operating-system mechanisms through C programming and experimental evaluation. The six tasks demonstrate how processes and threads are managed, how communication and synchronisation are achieved, and how operating systems handle resource allocation and deadlocks.

The combination of implementation, testing, performance analysis and design decisions helped establish a practical understanding of operating-system concepts and their application in real-world computing environments.

---

**Course:** CSA04 – Operating Systems  
**Assignment:** Assignment 2  
**Student Register Number:** 192521076
