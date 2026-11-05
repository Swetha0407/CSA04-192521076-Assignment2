#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5
#define ITEMS 20
#define PRODUCERS 4
#define CONSUMERS 4
#define REPEATS 100
#define ITEMS_PER_PRODUCER (ITEMS / PRODUCERS)
#define ITEMS_PER_CONSUMER (ITEMS / CONSUMERS)

int buffer[SIZE];
int in = 0, out = 0;
int consumed_sum = 0;
int consumed_count = 0;

sem_t empty, full;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = id * ITEMS_PER_PRODUCER + i + 1;

        sem_wait(&empty);
        pthread_mutex_lock(&lock);

        buffer[in] = item;
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&lock);
        sem_post(&full);
    }

    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;

    for (int i = 0; i < ITEMS_PER_CONSUMER; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&lock);

        int item = buffer[out];
        out = (out + 1) % SIZE;

        consumed_sum += item;
        consumed_count++;

        pthread_mutex_unlock(&lock);
        sem_post(&empty);
    }

    return NULL;
}

int main(void)
{
    int successful_runs = 0;

    for (int run = 1; run <= REPEATS; run++) {
        pthread_t p[PRODUCERS], c[CONSUMERS];
        int producer_ids[PRODUCERS];

        in = 0;
        out = 0;
        consumed_sum = 0;
        consumed_count = 0;

        sem_init(&empty, 0, SIZE);
        sem_init(&full, 0, 0);

        for (int i = 0; i < PRODUCERS; i++) {
            producer_ids[i] = i;
            pthread_create(&p[i], NULL, producer, &producer_ids[i]);
        }

        for (int i = 0; i < CONSUMERS; i++) {
            pthread_create(&c[i], NULL, consumer, NULL);
        }

        for (int i = 0; i < PRODUCERS; i++)
            pthread_join(p[i], NULL);

        for (int i = 0; i < CONSUMERS; i++)
            pthread_join(c[i], NULL);

        if (consumed_count == ITEMS && consumed_sum == 210)
            successful_runs++;
        else
            printf("Run %d FAILED: count=%d sum=%d\n",
                   run, consumed_count, consumed_sum);

        sem_destroy(&empty);
        sem_destroy(&full);
    }

    printf("Total runs: %d\n", REPEATS);
    printf("Successful runs: %d\n", successful_runs);
    printf("Expected items: %d, Expected sum: 210\n", ITEMS);

    pthread_mutex_destroy(&lock);

    return successful_runs == REPEATS ? 0 : 1;
}