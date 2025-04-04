// Anusha to work

// #include "../include/job.h"
// #include "../include/queue.h"
// #include "../include/io.h"
// #include "../include/statistics.h"
// #include <stdio.h>

// void mlfq_scheduler(Job **job_list, int job_count)
// {
//     printf("MLFQ scheduler is not yet implemented.\n");
//     printf("Please use 'sjf' or 'rr' options for now.\n");

//     // Mark all jobs as finished with dummy times to avoid issues in statistics
//     for (int i = 0; i < job_count; i++) {
//         if (job_list[i]->start_time == -1)
//             job_list[i]->start_time = 0;
//         job_list[i]->end_time = 1;
//         job_list[i]->state = FINISHED;
//     }
// }

#include "../include/job.h"
#include "../include/queue.h"
#include "../include/io.h"
#include "../include/statistics.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_QUEUES 3
#define BOOST_INTERVAL 20

void boost_all_jobs(JobQueue mlfq[])
{
    for (int i = 1; i < NUM_QUEUES; i++)
    {
        Job *curr = mlfq[i].front;
        while (curr)
        {
            Job *next = curr->next;
            curr->next = NULL;
            curr->queue_level = 0;
            enqueue(&mlfq[0], curr);
            curr = next;
        }
        mlfq[i].front = mlfq[i].rear = NULL;
    }
}

void mlfq_scheduler(Job **job_list, int job_count)
{
    int clock = 0;
    int completed_jobs = 0;
    Job *running_job = NULL;
    int time_slice_used = 0;

    JobQueue mlfq[NUM_QUEUES];
    for (int i = 0; i < NUM_QUEUES; i++)
    {
        mlfq[i].front = mlfq[i].rear = NULL;
    }

    JobQueue io_queue = {NULL, NULL};
    os_srand(1);

    while (completed_jobs < job_count)
    {
        // Rule 5: Priority Boost
        if (clock > 0 && clock % BOOST_INTERVAL == 0)
        {
            printf("Clock %d: Priority Boosting all jobs\n", clock);
            boost_all_jobs(mlfq);
        }

        // Handle job arrivals (Rule 3)
        for (int i = 0; i < job_count; i++)
        {
            Job *job = job_list[i];
            if (job->arrival_time == clock && job->state == NEW)
            {
                job->state = READY;
                job->queue_level = 0;
                enqueue(&mlfq[0], job);
                printf("Clock %d: Job %d arrived and added to Q%d\n", clock, job->pid, 0);
            }
        }

        // Handle I/O completion
        Job *prev = NULL, *curr = io_queue.front;
        while (curr != NULL)
        {
            if (IO_complete())
            {
                printf("Clock %d: Job %d completed I/O\n", clock, curr->pid);
                Job *done = curr;
                if (prev)
                    prev->next = curr->next;
                else
                    io_queue.front = curr->next;
                if (curr == io_queue.rear)
                    io_queue.rear = prev;
                curr = curr->next;
                done->next = NULL;
                done->state = READY;
                enqueue(&mlfq[done->queue_level], done);
                continue;
            }
            prev = curr;
            curr = curr->next;
        }

        // Schedule next job (Rules 1 & 2)
        if (!running_job)
        {
            for (int i = 0; i < NUM_QUEUES; i++)
            {
                if (!is_empty(&mlfq[i]))
                {
                    running_job = dequeue(&mlfq[i]);
                    running_job->state = RUNNING;
                    time_slice_used = 0;
                    if (running_job->start_time == -1)
                        running_job->start_time = clock;
                    printf("Clock %d: Job %d running from Q%d\n", clock, running_job->pid, running_job->queue_level);
                    break;
                }
            }
        }

        // Run the job if any
        if (running_job)
        {
            running_job->remaining_time--;
            running_job->time_in_running++;
            time_slice_used++;

            int max_time_slice = 1 << running_job->queue_level;

            printf("Clock %d: Job %d running (remaining %d, slice %d/%d)\n",
                   clock, running_job->pid, running_job->remaining_time,
                   time_slice_used, max_time_slice);

            if (running_job->remaining_time == 0)
            {
                printf("Clock %d: Job %d finished\n", clock, running_job->pid);
                running_job->state = FINISHED;
                running_job->end_time = clock;
                running_job = NULL;
                completed_jobs++;
                time_slice_used = 0;
            }
            else if (IO_request())
            {
                printf("Clock %d: Job %d requested I/O\n", clock, running_job->pid);
                running_job->state = WAITING_IO;
                enqueue(&io_queue, running_job);
                // Rule 4b: No demotion
                running_job = NULL;
                time_slice_used = 0;
            }
            else if (time_slice_used >= max_time_slice)
            {
                // Rule 4a: Demotion
                if (running_job->queue_level < NUM_QUEUES - 1)
                    running_job->queue_level++;
                printf("Clock %d: Job %d time slice expired, demoted to Q%d\n",
                       clock, running_job->pid, running_job->queue_level);
                running_job->state = READY;
                enqueue(&mlfq[running_job->queue_level], running_job);
                running_job = NULL;
                time_slice_used = 0;
            }
        }

        // Time accounting
        for (int i = 0; i < NUM_QUEUES; i++)
        {
            Job *temp = mlfq[i].front;
            while (temp)
            {
                temp->time_in_ready++;
                temp = temp->next;
            }
        }

        Job *io_temp = io_queue.front;
        while (io_temp)
        {
            io_temp->time_in_io++;
            io_temp = io_temp->next;
        }

        clock++;
        if (clock > 10000)
        {
            printf("Exceeded simulation max time.\n");
            break;
        }
    }

    printf("\nSimulation completed at clock tick %d\n", clock);
    print_statistics(job_list, job_count, clock);
}
