#include <stdio.h>

#define N 4
#define MAX_SEGMENTS 256

typedef struct {
    int process, start, end;
} Segment;

typedef struct {
    Segment segments[MAX_SEGMENTS];
    int count, dispatches;
    int finish[N], first_start[N];
} Result;

const char *name[N] = {"P1", "P2", "P3", "P4"};
const int arrival[N] = {0, 1, 2, 3};
const int burst[N] = {8, 4, 9, 5};

void init_result(Result *r)
{
    r->count = 0;
    r->dispatches = 0;

    for (int i = 0; i < N; i++) {
        r->finish[i] = -1;
        r->first_start[i] = -1;
    }
}

void add_segment(Result *r, int p, int start, int end)
{
    r->dispatches++;

    if (r->count > 0 &&
        r->segments[r->count - 1].process == p &&
        r->segments[r->count - 1].end == start) {
        r->segments[r->count - 1].end = end;
    } else if (r->count < MAX_SEGMENTS) {
        r->segments[r->count].process = p;
        r->segments[r->count].start = start;
        r->segments[r->count].end = end;
        r->count++;
    }

    if (r->first_start[p] == -1)
        r->first_start[p] = start;

    r->finish[p] = end;
}

void print_result(const char *title, const Result *r)
{
    double total_tat = 0, total_wt = 0, total_rt = 0;

    printf("\n%s\nGantt:", title);

    for (int i = 0; i < r->count; i++) {
        printf(" %s(%d-%d)",
               name[r->segments[i].process],
               r->segments[i].start,
               r->segments[i].end);
    }

    printf("\nProcess  CT  TAT  WT  RT\n");

    for (int i = 0; i < N; i++) {
        int tat = r->finish[i] - arrival[i];
        int wt = tat - burst[i];
        int rt = r->first_start[i] - arrival[i];

        total_tat += tat;
        total_wt += wt;
        total_rt += rt;

        printf("%-7s %2d %4d %3d %3d\n",
               name[i], r->finish[i], tat, wt, rt);
    }

    printf("Average TAT = %.2f, Average WT = %.2f, Average RT = %.2f\n",
           total_tat / N, total_wt / N, total_rt / N);

    printf("Dispatches = %d\n", r->dispatches);
}

/* mode 0 = FCFS, mode 1 = SJF, mode 2 = Priority */
void run_nonpreemptive(Result *r, int mode)
{
    int completed[N] = {0};
    int time = 0, done = 0;

    init_result(r);

    while (done < N) {
        int chosen = -1;

        for (int i = 0; i < N; i++) {
            if (!completed[i] && arrival[i] <= time) {
                if (chosen == -1) {
                    chosen = i;
                } else if (mode == 1 && burst[i] < burst[chosen]) {
                    chosen = i;
                } else if (mode == 2 && i < chosen) {
                    chosen = i;
                }
            }
        }

        if (chosen == -1) {
            time++;
            continue;
        }

        add_segment(r, chosen, time, time + burst[chosen]);
        time += burst[chosen];
        completed[chosen] = 1;
        done++;
    }
}

void run_srtf(Result *r)
{
    int remaining[N];
    int time = 0, done = 0;
    int current = -1, segment_start = 0;

    init_result(r);

    for (int i = 0; i < N; i++)
        remaining[i] = burst[i];

    while (done < N) {
        int chosen = -1;

        for (int i = 0; i < N; i++) {
            if (arrival[i] <= time && remaining[i] > 0) {
                if (chosen == -1 ||
                    remaining[i] < remaining[chosen] ||
                    (remaining[i] == remaining[chosen] && i < chosen)) {
                    chosen = i;
                }
            }
        }

        if (chosen != current) {
            if (current != -1)
                add_segment(r, current, segment_start, time);

            current = chosen;
            segment_start = time;
        }

        if (chosen == -1) {
            time++;
            continue;
        }

        remaining[chosen]--;
        time++;

        if (remaining[chosen] == 0) {
            add_segment(r, chosen, segment_start, time);
            current = -1;
            done++;
        }
    }
}

void run_round_robin(Result *r, int quantum)
{
    int remaining[N], added[N] = {0};
    int queue[512], head = 0, tail = 0;
    int time = 0, done = 0;

    init_result(r);

    for (int i = 0; i < N; i++)
        remaining[i] = burst[i];

    queue[tail++] = 0;
    added[0] = 1;

    while (done < N) {
        if (head == tail) {
            for (int i = 0; i < N; i++) {
                if (!added[i] && arrival[i] <= time) {
                    queue[tail++] = i;
                    added[i] = 1;
                }
            }

            if (head == tail) {
                time++;
                continue;
            }
        }

        int p = queue[head++];
        int run = remaining[p] < quantum ? remaining[p] : quantum;
        int start = time;

        time += run;
        remaining[p] -= run;

        add_segment(r, p, start, time);

        /* Add newly arrived processes before re-queuing p. */
        for (int i = 0; i < N; i++) {
            if (!added[i] && arrival[i] <= time) {
                queue[tail++] = i;
                added[i] = 1;
            }
        }

        if (remaining[p] > 0)
            queue[tail++] = p;
        else
            done++;
    }
}

void print_summary(const char *label, const Result *r)
{
    double tat = 0, wt = 0, rt = 0;

    for (int i = 0; i < N; i++) {
        int t = r->finish[i] - arrival[i];

        tat += t;
        wt += t - burst[i];
        rt += r->first_start[i] - arrival[i];
    }

    printf("%-10s %8.2f %8.2f %8.2f %10d\n",
           label, tat / N, wt / N, rt / N, r->dispatches);
}

int main(void)
{
    Result r;

    printf("Scheduling comparison for four processes\n");
    printf("Priority assumption: P1 > P2 > P3 > P4\n");

    run_nonpreemptive(&r, 0);
    print_result("FCFS", &r);

    run_nonpreemptive(&r, 1);
    print_result("Non-preemptive SJF", &r);

    run_srtf(&r);
    print_result("SRTF", &r);

    run_nonpreemptive(&r, 2);
    print_result("Non-preemptive Priority", &r);

    run_round_robin(&r, 3);
    print_result("Round Robin (q=3)", &r);

    printf("\nRound Robin quantum experiment\n");
    printf("%-10s %8s %8s %8s %10s\n",
           "Quantum", "Avg TAT", "Avg WT", "Avg RT", "Dispatches");

    const int quanta[] = {1, 3, 5, 20};

    for (int i = 0; i < 4; i++) {
        char label[32];

        run_round_robin(&r, quanta[i]);
        snprintf(label, sizeof(label), "%d", quanta[i]);
        print_summary(label, &r);
    }

    return 0;
}