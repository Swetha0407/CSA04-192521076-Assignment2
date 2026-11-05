# Task 1 – Processes and Process States

## Objective
To understand process creation, process states and Process Control Blocks (PCB), and demonstrate parent-child process creation using the `fork()` system call in Linux.

## Concepts Covered
- Five-state process model
- Process Control Block (PCB)
- Process creation using `fork()`
- Parent and child process identification
- Process observation using `ps` and `/proc`

## Implementation
The C program uses `fork()` to create a child process. The parent and child display their respective process IDs and parent process IDs. The parent uses `waitpid()` to wait for the child to complete execution.

## Compilation and Execution

```bash
gcc -std=c11 -Wall -Wextra process_demo.c -o process_demo
./process_demo
```

## Process Observation

The `ps` command is used to inspect running processes, while the `/proc` filesystem provides information about process status and attributes.

## Evaluation
A process-per-student approach offers isolation but introduces memory and process-management overhead when handling 2,000 simultaneous examination sessions. A fixed worker-process pool is recommended to control resource consumption while supporting concurrent request processing.

## Conclusion
The experiment successfully demonstrates process creation, parent-child relationships and process termination in Linux. The evaluation highlights the importance of selecting an appropriate process-management strategy for high-concurrency systems.