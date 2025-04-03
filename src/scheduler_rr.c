// #include "../include/job.h"
// #include "../include/queue.h"
// #include "../include/io.h"
// #include "../include/statistics.h"
// #include <stdio.h>

// #define TIME_SLICE 2

// void round_robin_scheduler(Job **job_list, int job_count)
// {
//     int clock = 0;
//     int completed_jobs = 0;
//     Job *running_job = NULL;
//     int time_used = 0;

//     JobQueue ready_queue = {NULL, NULL};
//     JobQueue io_queue = {NULL, NULL};

//     while (clock < 500 && completed_jobs < job_count)
//     {
//         printf("Clock: %d | Completed: %d/%d\n", clock, completed_jobs, job_count);

//         // Move arriving jobs to ready queue
//         for (int i = 0; i < job_count; i++)
//         {
//             Job *job = job_list[i];
//             if (job->arrival_time == clock && job->state == NEW)
//             {
//                 job->state = READY;
//                 enqueue(&ready_queue, job);
//                 printf("Job %d arrived at clock %d\n", job->pid, clock);
//             }
//         }

//         // Handle I/O completion
//         Job *prev = NULL;
//         Job *curr = io_queue.front;

//         while (curr != NULL)
//         {
//             if (IO_complete())
//             {
//                 Job *completed_io = curr;
//                 completed_io->state = READY;
//                 enqueue(&ready_queue, completed_io);
//                 printf("Job %d completed I/O at clock %d\n", completed_io->pid, clock);

//                 // Remove from io_queue
//                 if (prev == NULL)
//                 {
//                     io_queue.front = curr->next;
//                 }
//                 else
//                 {
//                     prev->next = curr->next;
//                 }
//                 if (curr == io_queue.rear)
//                 {
//                     io_queue.rear = prev;
//                 }

//                 Job *temp = curr->next;
//                 curr->next = NULL;
//                 curr = temp;
//                 continue;
//             }

//             prev = curr;
//             curr = curr->next;
//         }

//         // Start new job if needed
//         if (running_job == NULL && ready_queue.front != NULL)
//         {
//             running_job = dequeue(&ready_queue);
//             if (running_job->start_time == -1)
//                 running_job->start_time = clock;
//             running_job->state = RUNNING;
//             time_used = 0;
//             printf("Job %d started running at clock %d\n", running_job->pid, clock);
//         }

//         // Run current job
//         if (running_job != NULL)
//         {
//             running_job->remaining_time--;
//             running_job->time_in_running++;
//             time_used++;
//             printf("Job %d running... Remaining: %d\n", running_job->pid, running_job->remaining_time);

//             // Completion check
//             if (running_job->remaining_time <= 0)
//             {
//                 running_job->state = FINISHED;
//                 running_job->end_time = clock;
//                 completed_jobs++;
//                 printf("Job %d finished at clock %d\n", running_job->pid, clock);
//                 running_job = NULL;
//                 time_used = 0;
//             }
//             else if (IO_request())
//             {
//                 running_job->state = WAITING_IO;
//                 enqueue(&io_queue, running_job);
//                 printf("Job %d requested I/O at clock %d\n", running_job->pid, clock);
//                 running_job = NULL;
//                 time_used = 0;
//             }
//             else if (time_used >= TIME_SLICE)
//             {
//                 running_job->state = READY;
//                 enqueue(&ready_queue, running_job);
//                 printf("Job %d time slice expired at clock %d\n", running_job->pid, clock);
//                 running_job = NULL;
//                 time_used = 0;
//             }
//         }

//         // Track time for waiting jobs
//         Job *temp = ready_queue.front;
//         while (temp != NULL)
//         {
//             temp->time_in_ready++;
//             temp = temp->next;
//         }

//         temp = io_queue.front;
//         while (temp != NULL)
//         {
//             temp->time_in_io++;
//             temp = temp->next;
//         }

//         clock++;
//     }

//     printf("Simulation ended at clock %d\n", clock);
//     print_statistics(job_list, job_count, clock);
// }

#include "../include/job.h"
#include "../include/queue.h"
#include "../include/io.h"
#include "../include/statistics.h"
#include <stdio.h>
#include <stdlib.h>

#define TIME_SLICE 2

void round_robin_scheduler(Job **job_list, int job_count)
{
    int clock = 0;
    int completed_jobs = 0;
    Job *running_job = NULL;
    int time_slice_used = 0;

 
    JobQueue ready_queue = {NULL, NULL};
    JobQueue io_queue = {NULL, NULL};
    

    os_srand(1);

    while (completed_jobs < job_count)
    {
        
        for (int i = 0; i < job_count; i++)
        {
            Job *job = job_list[i];
            if (job->arrival_time == clock && job->state == NEW)
            {
                job->state = READY;
                enqueue(&ready_queue, job);
                printf("Clock %d: Job %d arrived and added to ready queue\n", 
                       clock, job->pid);
            }
        }


        if (io_queue.front != NULL)
        {
            Job *prev = NULL;
            Job *curr = io_queue.front;

            while (curr != NULL)
            {
           
                if (IO_complete())
                {
                    printf("Clock %d: Job %d completed I/O\n", clock, curr->pid);
                    
               
                    Job *completed_io = curr;
                    
                    if (prev == NULL)
                    {
                        io_queue.front = curr->next;
                    }
                    else
                    {
                        prev->next = curr->next;
                    }
                    
                    if (curr == io_queue.rear)
                    {
                        io_queue.rear = prev;
                    }
                    
                    Job *next = curr->next;
                    curr->next = NULL;
                    
               
                    completed_io->state = READY;
                    enqueue(&ready_queue, completed_io);
                    
                    curr = next;
                    continue;
                }
                
                prev = curr;
                curr = curr->next;
            }
        }

        if (running_job == NULL && !is_empty(&ready_queue))
        {
            running_job = dequeue(&ready_queue);
            running_job->state = RUNNING;
            time_slice_used = 0;
            
   
            if (running_job->start_time == -1)
            {
                running_job->start_time = clock;
            }
            
            printf("Clock %d: Job %d started running (remaining: %d)\n", 
                   clock, running_job->pid, running_job->remaining_time);
        }

   
        if (running_job != NULL)
        {
          
            running_job->remaining_time--;
            running_job->time_in_running++;
            time_slice_used++;
            
            printf("Clock %d: Job %d running (remaining: %d, time slice: %d/%d)\n", 
                   clock, running_job->pid, running_job->remaining_time, 
                   time_slice_used, TIME_SLICE);
            
        
            if (running_job->remaining_time == 0)
            {
                printf("Clock %d: Job %d completed execution\n", 
                       clock, running_job->pid);
                
                running_job->state = FINISHED;
                running_job->end_time = clock;
                completed_jobs++;
                running_job = NULL;
                time_slice_used = 0;
            }
     
            else if (IO_request())
            {
                printf("Clock %d: Job %d requested I/O\n", 
                       clock, running_job->pid);
                
                running_job->state = WAITING_IO;
                enqueue(&io_queue, running_job);
                running_job = NULL;
                time_slice_used = 0;
            }
       
            else if (time_slice_used >= TIME_SLICE)
            {
                printf("Clock %d: Job %d time slice expired\n", 
                       clock, running_job->pid);
                
                running_job->state = READY;
                enqueue(&ready_queue, running_job);
                running_job = NULL;
                time_slice_used = 0;
            }
        }

        Job *temp = ready_queue.front;
        while (temp != NULL)
        {
            temp->time_in_ready++;
            temp = temp->next;
        }

        temp = io_queue.front;
        while (temp != NULL)
        {
            temp->time_in_io++;
            temp = temp->next;
        }

       
        clock++;
        
       
        if (clock > 10000)
        {
            printf("Warning: Exceeded maximum simulation time (10000 ticks).\n");
            break;
        }
    }

    printf("\nSimulation completed at clock tick %d\n", clock);
    print_statistics(job_list, job_count, clock);
}