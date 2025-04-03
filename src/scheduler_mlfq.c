// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>

// #define MAX_PROCESSES 100
// #define MAX_QUEUES 3
// #define TIME_SLICE_BASE 2
// #define BOOST_INTERVAL 20

// typedef struct
// {
//     int pid;
//     int burst_time;
//     int remaining_time;
//     int queue_level;
//     int time_in_system;
//     bool active;
// } Process;

// typedef struct
// {
//     Process *processes[MAX_PROCESSES];
//     int front, rear;
// } Queue;

// Queue queues[MAX_QUEUES];
// int current_time = 0;

// void init_queue(Queue *q)
// {
//     q->front = q->rear = 0;
// }

// bool is_empty(Queue *q)
// {
//     return q->front == q->rear;
// }

// void enqueue(Queue *q, Process *p)
// {
//     q->processes[q->rear++] = p;
// }

// Process *dequeue(Queue *q)
// {
//     return is_empty(q) ? NULL : q->processes[q->front++];
// }

// void boost_all_processes()
// {
//     printf("Boosting all processes to highest priority.\n");
//     for (int i = 1; i < MAX_QUEUES; i++)
//     {
//         while (!is_empty(&queues[i]))
//         {
//             Process *p = dequeue(&queues[i]);
//             p->queue_level = 0;
//             enqueue(&queues[0], p);
//         }
//     }
// }

// void run_scheduler()
// {
//     while (true)
//     {
//         bool all_done = true;

//         if (current_time > 0 && current_time % BOOST_INTERVAL == 0)
//         {
//             boost_all_processes();
//         }

//         for (int i = 0; i < MAX_QUEUES; i++)
//         {
//             if (!is_empty(&queues[i]))
//             {
//                 Process *p = dequeue(&queues[i]);
//                 int time_slice = TIME_SLICE_BASE << i; // Longer slices at lower priorities
//                 int actual_run_time = (p->remaining_time < time_slice) ? p->remaining_time : time_slice;

//                 printf("Time %d: Running Process %d from Queue %d for %d units\n", current_time, p->pid, i, actual_run_time);
//                 current_time += actual_run_time;
//                 p->time_in_system += actual_run_time;
//                 p->remaining_time -= actual_run_time;

//                 if (p->remaining_time == 0)
//                 {
//                     printf("Process %d completed.\n", p->pid);
//                     p->active = false;
//                 }
//                 else
//                 {
//                     if (actual_run_time == time_slice && i < MAX_QUEUES - 1)
//                     {
//                         p->queue_level++; // demote
//                         enqueue(&queues[p->queue_level], p);
//                     }
//                     else
//                     {
//                         enqueue(&queues[i], p); // stays in same queue
//                     }
//                 }
//                 all_done = false;
//                 break; // run one process at a time
//             }
//         }

//         if (all_done)
//             break;
//     }
// }

// void add_process(int pid, int burst_time)
// {
//     Process *p = (Process *)malloc(sizeof(Process));
//     p->pid = pid;
//     p->burst_time = burst_time;
//     p->remaining_time = burst_time;
//     p->queue_level = 0;
//     p->time_in_system = 0;
//     p->active = true;

//     enqueue(&queues[0], p); // start at highest priority
//     printf("Process %d added with burst time %d\n", pid, burst_time);
// }

// int main()
// {
//     for (int i = 0; i < MAX_QUEUES; i++)
//         init_queue(&queues[i]);

//     add_process(1, 5);
//     add_process(2, 12);
//     add_process(3, 7);

//     run_scheduler();
//     return 0;
// }

//Anusha to work

#include "../include/job.h"
#include "../include/queue.h"
#include "../include/io.h"
#include "../include/statistics.h"
#include <stdio.h>

void mlfq_scheduler(Job **job_list, int job_count)
{
    printf("MLFQ scheduler is not yet implemented.\n");
    printf("Please use 'sjf' or 'rr' options for now.\n");
    
    // Mark all jobs as finished with dummy times to avoid issues in statistics
    for (int i = 0; i < job_count; i++) {
        if (job_list[i]->start_time == -1)
            job_list[i]->start_time = 0;
        job_list[i]->end_time = 1;
        job_list[i]->state = FINISHED;
    }
}