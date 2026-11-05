#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 65536

typedef struct {
    sem_t empty;
    sem_t full;
    size_t bytes;
    unsigned char buffer[BUFFER_SIZE];
} SharedData;

double get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void)
{
    size_t sizes[] = {
        1 * 1024 * 1024,
        10 * 1024 * 1024,
        100 * 1024 * 1024
    };

    SharedData *shared = mmap(NULL, sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    sem_init(&shared->empty, 1, 1);
    sem_init(&shared->full, 1, 0);

    printf("Shared Memory IPC Benchmark\n");

    for (int s = 0; s < 3; s++) {
        size_t total = sizes[s];
        size_t transferred = 0;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        double start = get_time();

        if (pid == 0) {
            size_t received = 0;
            unsigned long checksum = 0;

            while (received < total) {
                sem_wait(&shared->full);

                size_t n = shared->bytes;

                for (size_t i = 0; i < n; i++)
                    checksum += shared->buffer[i];

                received += n;
                sem_post(&shared->empty);
            }

            printf("Child received %zu bytes, checksum = %lu\n",
                   received, checksum);
            fflush(stdout);
            _exit(0);
        }

        while (transferred < total) {
            size_t n = total - transferred;

            if (n > BUFFER_SIZE)
                n = BUFFER_SIZE;

            sem_wait(&shared->empty);

            for (size_t i = 0; i < n; i++)
                shared->buffer[i] = (unsigned char)((transferred + i) % 256);

            shared->bytes = n;
            transferred += n;

            sem_post(&shared->full);
        }

        waitpid(pid, NULL, 0);

        double elapsed = get_time() - start;

        printf("Transferred %zu MB in %.6f seconds\n\n",
               total / (1024 * 1024), elapsed);
    }

    sem_destroy(&shared->empty);
    sem_destroy(&shared->full);
    munmap(shared, sizeof(SharedData));

    return 0;
}