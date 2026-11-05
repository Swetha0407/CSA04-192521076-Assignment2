# Task 3 – Threads

## Objective

To evaluate multithreaded processing using POSIX threads for a traffic-camera number-plate recognition system involving 64 cameras.

## Implementation

A C program was developed using POSIX threads to simulate camera-frame processing. The program compares sequential execution, one thread per camera, and fixed thread pools containing 4, 8 and 16 threads.

The execution time is measured using `clock_gettime()` and speed-up is calculated relative to sequential processing.

## Thread Resource Sharing

Threads belonging to the same process share code, global variables, heap memory and open files. However, each thread maintains its own stack, registers, program counter and thread-local storage.

## Thread Models

- **Many-to-one:** Multiple user threads map to one kernel thread.
- **One-to-one:** Each user thread maps to a separate kernel thread.
- **Many-to-many:** Multiple user threads are mapped to multiple kernel threads.

Linux POSIX threads generally follow the one-to-one model.

## Performance Evaluation

The experiment measures execution time for sequential processing, 64 individual threads, and pools of 4, 8 and 16 threads.

Speed-up is calculated using:

Speed-up = Sequential Execution Time / Parallel Execution Time

Increasing the number of threads can improve parallelism, but performance gains eventually become limited by CPU cores, scheduling overhead, memory bandwidth and workload distribution.

*Insert actual execution measurements and screenshots here.*

## Race Condition

A shared counter was updated by multiple threads without synchronisation. This demonstrates a data race, where concurrent updates can interfere with one another. A mutex can protect the critical section and ensure correct updates.

## Alternative Concept – Multiple Processes

If camera frames were processed using multiple processes, each process would have a separate address space. This provides better isolation but increases memory consumption and requires inter-process communication to combine results.

Threads share memory and can exchange results directly, making them more suitable for this workload.

## Complexity Analysis

For 64 cameras with equal processing work W:

- Sequential processing: O(64 × W)
- Parallel processing: approximately O((64 / T) × W) with T workers, assuming balanced work and sufficient processing resources.
- Thread creation and joining add overhead.
- Memory usage includes shared process memory and a separate stack for each thread.

## Design Decision

A fixed thread pool is recommended for the traffic-camera system. It allows multiple camera frames to be processed concurrently while controlling the number of active threads. This reduces thread creation overhead and avoids the resource consumption associated with creating an unlimited number of threads.

## Conclusion

The experiment demonstrates how POSIX threads can improve camera-frame processing through concurrency. It also highlights the importance of selecting an appropriate thread-pool size and protecting shared data against race conditions.