// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "../include/job.h"
// #include "../include/io.h"
// #include "../include/statistics.h"

// // Declare scheduler prototypes
// void sjf_scheduler(Job **job_list, int job_count);
// void round_robin_scheduler(Job **job_list, int job_count);
// void mlfq_scheduler(Job **job_list, int job_count);
// int read_job_file(const char *filename, Job ***job_list_ptr); // you’ll define this

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         printf("Usage: %s <job_file> <scheduler_type>\n", argv[0]);
//         return 1;
//     }

//     Job **job_list;
//     int job_count = read_job_file(argv[1], &job_list);

//     if (strcmp(argv[2], "sjf") == 0)
//         sjf_scheduler(job_list, job_count);
//     else if (strcmp(argv[2], "rr") == 0)
//         round_robin_scheduler(job_list, job_count);
//     else if (strcmp(argv[2], "mlfq") == 0)
//         mlfq_scheduler(job_list, job_count);
//     else
//         printf("Unknown scheduler type: %s\n", argv[2]);

//     return 0;

//     // if (argc != 2)
//     // {
//     //     printf("Usage: %s <job_file>\n", argv[0]);
//     //     return 1;
//     // }

//     // Job **job_list;
//     // int job_count = read_job_file(argv[1], &job_list);

//     // printf("Read %d jobs from file:\n", job_count);
//     // for (int i = 0; i < job_count; i++)
//     // {
//     //     Job *job = job_list[i];
//     //     printf("Job %d: Arrival = %d, Service = %d\n",
//     //            job->pid, job->arrival_time, job->service_time);
//     // }

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/job.h"
#include "../include/io.h"
#include "../include/statistics.h"

void sjf_scheduler(Job **job_list, int job_count);
void round_robin_scheduler(Job **job_list, int job_count);
void mlfq_scheduler(Job **job_list, int job_count);

void print_usage(const char *program_name)
{
    printf("Usage: %s <job_file> <scheduler_type>\n", program_name);
    printf("  <job_file>: file containing job information in format 'PID:ArrivalTime:ServiceTime:Priority'\n");
    printf("  <scheduler_type>: one of 'sjf', 'rr', or 'mlfq'\n");
    printf("Example: %s jobs.txt sjf\n", program_name);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    Job **job_list;
    int job_count = read_job_file(argv[1], &job_list);
    
    if (job_count <= 0)
    {
        printf("Error: No jobs read from file '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Read %d jobs from file:\n", job_count);
    for (int i = 0; i < job_count; i++)
    {
        Job *job = job_list[i];
        printf("Job %d: PID=%d, Arrival=%d, Service=%d, Priority=%d\n",
               i+1, job->pid, job->arrival_time, job->service_time, job->priority);
    }
    
    printf("\n");

    if (strcmp(argv[2], "sjf") == 0)
    {
        printf("Running Preemptive Shortest Job First scheduler...\n\n");
        sjf_scheduler(job_list, job_count);
    }
    else if (strcmp(argv[2], "rr") == 0)
    {
        printf("Running Round Robin scheduler...\n\n");
        round_robin_scheduler(job_list, job_count);
    }
    else if (strcmp(argv[2], "mlfq") == 0)
    {
        printf("Running Multi-level Feedback Queue scheduler...\n\n");
        mlfq_scheduler(job_list, job_count);
    }
    else
    {
        printf("Error: Unknown scheduler type '%s'\n", argv[2]);
        print_usage(argv[0]);
        return 1;
    }
    
    for (int i = 0; i < job_count; i++)
    {
        free(job_list[i]);
    }
    free(job_list);

    return 0;
}