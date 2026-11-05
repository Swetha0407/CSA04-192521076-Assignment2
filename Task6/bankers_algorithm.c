#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define P 5
#define R 3

int avail[R] = {3, 3, 2};

int maxm[P][R] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};

int alloc[P][R] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

/* Banker's safety algorithm */
int is_safe(int seq[P])
{
    int work[R];
    int finish[P] = {0};
    int count = 0;

    memcpy(work, avail, sizeof(work));

    while (count < P) {
        int found = 0;

        for (int p = 0; p < P; p++) {
            if (finish[p])
                continue;

            int possible = 1;

            /* TODO 1 completed */
            for (int r = 0; r < R; r++) {
                int need = maxm[p][r] - alloc[p][r];

                if (need > work[r]) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                for (int r = 0; r < R; r++)
                    work[r] += alloc[p][r];

                finish[p] = 1;
                seq[count++] = p;
                found = 1;
            }
        }

        if (!found)
            return 0;
    }

    return 1;
}

/* Resource-request algorithm */
void request(int p, const int req[R])
{
    for (int r = 0; r < R; r++) {
        int need = maxm[p][r] - alloc[p][r];

        if (req[r] > need || req[r] > avail[r]) {
            printf("P%d request: must wait (not available)\n", p);
            return;
        }
    }

    /* Temporarily allocate requested resources */
    for (int r = 0; r < R; r++) {
        avail[r] -= req[r];
        alloc[p][r] += req[r];
    }

    int seq[P];

    if (is_safe(seq)) {
        printf("P%d request: granted, safe sequence", p);

        for (int i = 0; i < P; i++)
            printf(" P%d", seq[i]);

        printf("\n");
    } else {
        /* Roll back unsafe allocation */
        for (int r = 0; r < R; r++) {
            avail[r] += req[r];
            alloc[p][r] -= req[r];
        }

        printf("P%d request: denied (unsafe)\n", p);
    }
}

/* Benchmark a worst-case safe sequence */
double benchmark(int n)
{
    int *available = malloc(sizeof(int));
    int *allocation = malloc(n * sizeof(int));
    int *need = malloc(n * sizeof(int));
    int *finish = malloc(n * sizeof(int));
    int *sequence = malloc(n * sizeof(int));

    if (!available || !allocation || !need ||
        !finish || !sequence) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /*
       One resource type.
       Only the last process can finish initially.
       Each completed process releases one unit,
       allowing the previous process to finish.
    */
    *available = 1;

    for (int i = 0; i < n; i++) {
        allocation[i] = 1;
        need[i] = n - i;
    }

    double total_time = 0.0;
    const int repetitions = 5;

    for (int run = 0; run < repetitions; run++) {
        memset(finish, 0, n * sizeof(int));

        int work = *available;
        int count = 0;

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        while (count < n) {
            int found = 0;

            for (int p = 0; p < n; p++) {
                if (!finish[p] && need[p] <= work) {
                    work += allocation[p];
                    finish[p] = 1;
                    sequence[count++] = p;
                    found = 1;
                }
            }

            if (!found)
                break;
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        total_time += (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec) / 1e9;
    }

    if (sequence[n - 1] < 0)
        fprintf(stderr, "Unexpected benchmark result\n");

    free(available);
    free(allocation);
    free(need);
    free(finish);
    free(sequence);

    return total_time / repetitions;
}

int main(void)
{
    int seq[P];

    printf("BANKER'S ALGORITHM\n\n");

    if (is_safe(seq)) {
        printf("Initial state safe:");

        for (int i = 0; i < P; i++)
            printf(" P%d", seq[i]);

        printf("\n");
    } else {
        printf("Initial state unsafe\n");
    }

    request(1, (int[R]){1, 0, 2});
    request(4, (int[R]){3, 3, 0});
    request(0, (int[R]){0, 2, 0});

    printf("\nPERFORMANCE MEASUREMENT\n");
    printf("Processes    Average time (seconds)\n");

    int sizes[] = {10, 100, 1000};

    for (int i = 0; i < 3; i++) {
        double elapsed = benchmark(sizes[i]);

        printf("%-12d %.9f\n", sizes[i], elapsed);
    }

    return 0;
}