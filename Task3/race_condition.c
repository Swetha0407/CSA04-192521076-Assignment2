#include <stdio.h>
#include <pthread.h>

#define THREADS 4
#define ITERATIONS 1000000

long counter = 0;

void *increment(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        counter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Expected counter: %d\n",
           THREADS * ITERATIONS);

    printf("Actual counter: %ld\n", counter);

    return 0;
}