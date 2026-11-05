#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define CAMERAS 64
#define WORK 500000

typedef struct {
    int camera_id;
    long result;
} Camera;

typedef struct {
    Camera *cameras;
    int start;
    int end;
} PoolTask;

long process_frame(int camera_id)
{
    long result = 0;

    for (int i = 0; i < WORK; i++) {
        result += (i * 31L + camera_id) % 97;
    }

    return result;
}

void *camera_worker(void *arg)
{
    Camera *cam = (Camera *)arg;
    cam->result = process_frame(cam->camera_id);
    return NULL;
}

void *pool_worker(void *arg)
{
    PoolTask *task = (PoolTask *)arg;

    for (int i = task->start; i < task->end; i++) {
        task->cameras[i].result =
            process_frame(task->cameras[i].camera_id);
    }

    return NULL;
}

double get_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void)
{
    Camera cameras[CAMERAS];
    pthread_t threads[CAMERAS];

    for (int i = 0; i < CAMERAS; i++) {
        cameras[i].camera_id = i + 1;
        cameras[i].result = 0;
    }

    /* Sequential processing */
    double start = get_time();

    for (int i = 0; i < CAMERAS; i++) {
        cameras[i].result =
            process_frame(cameras[i].camera_id);
    }

    double sequential_time = get_time() - start;

    printf("Sequential time: %.6f seconds\n",
           sequential_time);

    /* One thread per camera */
    start = get_time();

    for (int i = 0; i < CAMERAS; i++) {
        pthread_create(&threads[i], NULL,
                       camera_worker, &cameras[i]);
    }

    for (int i = 0; i < CAMERAS; i++) {
        pthread_join(threads[i], NULL);
    }

    double individual_time = get_time() - start;

    printf("64 threads time: %.6f seconds | Speed-up: %.2fx\n",
           individual_time,
           sequential_time / individual_time);

    /* Thread pools */
    int pool_sizes[] = {4, 8, 16};

    for (int p = 0; p < 3; p++) {
        int pool_size = pool_sizes[p];
        PoolTask tasks[16];

        start = get_time();

        int chunk = CAMERAS / pool_size;

        for (int i = 0; i < pool_size; i++) {
            tasks[i].cameras = cameras;
            tasks[i].start = i * chunk;
            tasks[i].end = (i == pool_size - 1)
                               ? CAMERAS
                               : (i + 1) * chunk;

            pthread_create(&threads[i], NULL,
                           pool_worker, &tasks[i]);
        }

        for (int i = 0; i < pool_size; i++) {
            pthread_join(threads[i], NULL);
        }

        double elapsed = get_time() - start;

        printf("%2d-thread pool time: %.6f seconds | Speed-up: %.2fx\n",
               pool_size, elapsed,
               sequential_time / elapsed);
    }

    return 0;
}