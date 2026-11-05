#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define ITERATIONS 1000000

atomic_long atomic_counter = 0;
long spin_counter = 0;
atomic_flag spinlock = ATOMIC_FLAG_INIT;

long mutex_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void *atomic_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++)
        atomic_fetch_add(&atomic_counter, 1);

    return NULL;
}

void *spin_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        while (atomic_flag_test_and_set(&spinlock))
            ;

        spin_counter++;

        atomic_flag_clear(&spinlock);
    }

    return NULL;
}

void *mutex_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        mutex_counter++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void run_test(const char *name,
              void *(*worker)(void *), int thread_count)
{
    pthread_t threads[16];

    atomic_store(&atomic_counter, 0);
    spin_counter = 0;
    mutex_counter = 0;
    atomic_flag_clear(&spinlock);

    double start = get_time();

    for (int i = 0; i < thread_count; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    double elapsed = get_time() - start;

    printf("%-12s %2d threads: %.6f seconds\n",
           name, thread_count, elapsed);
}

int main(void)
{
    int counts[] = {1, 4, 16};

    printf("Synchronisation Benchmark\n");
    printf("Iterations per thread: %d\n\n", ITERATIONS);

    for (int i = 0; i < 3; i++)
        run_test("Atomic", atomic_worker, counts[i]);

    for (int i = 0; i < 3; i++)
        run_test("Spinlock", spin_worker, counts[i]);

    for (int i = 0; i < 3; i++)
        run_test("Mutex", mutex_worker, counts[i]);

    printf("\nExpected counter for each test: %d\n",
           ITERATIONS * 16);

    pthread_mutex_destroy(&mutex);

    return 0;
}