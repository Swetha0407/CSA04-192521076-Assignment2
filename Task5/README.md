# Task 5 – Synchronisation: Critical Sections, Semaphores and Monitors

## Objective

To implement and evaluate synchronisation techniques using semaphores and mutexes, understand critical-section requirements, and compare atomic operations, spinlocks and mutexes.

## 1. Critical-Section Requirements

A correct solution to the critical-section problem must satisfy three requirements:

- **Mutual exclusion:** Only one process or thread can execute the protected critical section at a time.
- **Progress:** If the critical section is free, the selection of the next process must not be postponed indefinitely.
- **Bounded waiting:** Every process requesting entry must eventually get an opportunity to enter.

## 2. Peterson's Solution

Peterson's algorithm is a software-based synchronisation technique for two processes. It uses two intent flags and a turn variable to coordinate entry into the critical section.

It satisfies mutual exclusion, progress and bounded waiting under its assumed memory model. However, a straightforward implementation may fail on modern multiprocessors because of compiler and CPU memory reordering. It is also limited to two participants.

## 3. Bounded-Buffer Implementation

The experiment uses a circular buffer with a size of 5, four producers and four consumers. A total of 20 items are transferred.

The implementation uses:

| Synchronisation variable | Initial value | Purpose |
|---|---:|---|
| `empty` | 5 | Counts available buffer slots |
| `full` | 0 | Counts occupied buffer slots |
| `lock` | Unlocked | Protects shared buffer data |

Producers wait for an empty slot before inserting an item. Consumers wait for a filled slot before removing an item. A mutex protects the buffer indices and shared sum.

### Expected result

- Total items consumed: 20
- Expected sum: 210
- Number of test repetitions: 100
- Expected successful runs: 100

*Insert the actual terminal output screenshot here.*

## 4. Synchronisation Correctness

The semaphore operations prevent producers from inserting into a full buffer and consumers from removing items from an empty buffer. The mutex protects shared data from simultaneous modification.

The program verifies that all 20 items are consumed and that their sum is 210 in every repetition.

Removing the mutex can introduce a race condition because multiple threads may modify shared variables simultaneously. Incorrect results may occur even when semaphore operations control buffer availability, because the semaphore alone does not protect every shared variable.

*Insert any additional experiment output here if performed.*

## 5. Alternative Synchronisation Methods

The shared counter was also evaluated using three methods:

- **Atomic operations:** `atomic_fetch_add()` performs an indivisible counter update.
- **Spinlock:** A thread repeatedly checks whether an atomic lock is available.
- **Mutex:** A thread acquires a lock before updating the shared counter.

The benchmark tests 1, 4 and 16 threads, each performing one million updates.

*Insert the actual benchmark output and measured timings here.*

### Performance comparison

| Method | Advantage | Limitation |
|---|---|---|
| Atomic operation | Efficient for simple shared updates | Not suitable for complex multi-step critical sections |
| Spinlock | Avoids sleeping for very short waits | Consumes CPU while waiting |
| Mutex | Allows waiting threads to block | Has locking and scheduling overhead |

A mutex is generally suitable for longer critical sections, while spinlocks are more appropriate for very short waits. Atomic operations are useful for simple independent updates.

## 6. Readers-Writers Problem

The readers-writers problem allows multiple readers to access shared data simultaneously, but writers require exclusive access. Synchronisation prevents readers from observing partially updated data and prevents multiple writers from modifying the resource together.

The solution must also consider starvation and fairness.

## 7. Dining-Philosophers Problem

The dining-philosophers problem demonstrates deadlock when multiple processes compete for limited resources. If each philosopher holds one fork while waiting for another, a circular wait can occur.

Deadlock can be prevented by imposing a fixed resource-acquisition order, limiting the number of competing philosophers, or using a waiter or monitor.

## 8. Monitors and Condition Variables

A monitor combines shared data and the operations that access it into a synchronised unit. Only one thread can execute within the monitor at a time.

Condition variables allow threads to wait until a required condition becomes true. In the bounded-buffer problem, producers wait while the buffer is full and consumers wait while it is empty. This makes the synchronisation logic easier to organise and maintain.

## 9. Ticket-Booking System Design Decision

Three locking approaches were considered:

- **Global lock:** Simple, but all booking operations are serialised.
- **Train-level lock:** Allows different trains to be booked concurrently, but bookings within the same train are serialised.
- **Coach-level lock:** Allows bookings in different coaches of the same train to proceed concurrently.

A coach-level lock is recommended because it provides a balance between mutual exclusion and booking throughput. Each coach can be protected independently, while operations on the same coach remain synchronised. For transactions involving multiple coaches, locks should be acquired in a consistent order to avoid deadlocks.

## 10. Complexity Analysis

Let n represent the number of shared-counter updates.

| Method | Time complexity for n updates | Additional space |
|---|---|---|
| Atomic operations | O(n) | O(1) |
| Spinlock | O(n) without contention; waiting can increase under contention | O(1) |
| Mutex | O(n) excluding scheduling overhead | O(1) |
| Semaphore-based buffer | O(n) for n items, excluding scheduling overhead | O(B) for a buffer of size B |

The actual execution time depends on the number of threads, processor cores, contention and synchronisation overhead.

## Conclusion

The experiment demonstrates how semaphores and mutexes coordinate producer-consumer operations and protect shared resources. Repeating the bounded-buffer test validates the correctness of the implementation. The alternative benchmark illustrates the trade-offs between atomic operations, spinlocks and mutexes. For the ticket-booking scenario, coach-level locking is recommended to maintain correctness while allowing independent bookings to proceed concurrently.

## Compilation and Execution

```bash
gcc -std=c11 -Wall -Wextra -pthread producer_consumer.c -o producer_consumer
./producer_consumer

gcc -std=c11 -O2 -Wall -Wextra -pthread sync_benchmark.c -o sync_benchmark
./sync_benchmark
```
