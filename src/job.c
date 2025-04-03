// #include "../include/job.h"
// #include <stdio.h>
// #include <stdlib.h>

// int read_job_file(const char *filename, Job ***job_list_ptr)
// {
//     FILE *file = fopen(filename, "r");
//     if (!file)
//     {
//         perror("Error opening job file");
//         exit(1);
//     }

//     int capacity = 10;
//     int count = 0;
//     Job **jobs = malloc(sizeof(Job *) * capacity);

//     int pid, arrival, service;
//     while (fscanf(file, "%d %d %d", &pid, &arrival, &service) == 3)
//     {
//         if (count >= capacity)
//         {
//             capacity *= 2;
//             jobs = realloc(jobs, sizeof(Job *) * capacity);
//         }

//         Job *job = malloc(sizeof(Job));
//         job->pid = pid;
//         job->arrival_time = arrival;
//         job->service_time = service;
//         job->remaining_time = service;
//         job->priority = 0;
//         job->queue_level = 0;
//         job->current_queue_level = 0;
//         job->time_slice_used = 0;
//         job->state = NEW;
//         job->start_time = -1;
//         job->end_time = -1;
//         job->time_in_ready = 0;
//         job->time_in_running = 0;
//         job->time_in_io = 0;
//         job->next = NULL;

//         jobs[count++] = job;
//     }

//     fclose(file);
//     *job_list_ptr = jobs;
//     return count;
// }

#include "../include/job.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_job_file(const char *filename, Job ***job_list_ptr)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening job file");
        exit(1);
    }

    int capacity = 10;
    int count = 0;
    Job **jobs = malloc(sizeof(Job *) * capacity);
    
    char line[256];
    
    while (fgets(line, sizeof(line), file))
    {

        int pid, arrival, service, priority;
        char *token;
        
      
        token = strtok(line, ":");
        if (token == NULL) continue;
        pid = atoi(token);
        
        token = strtok(NULL, ":");
        if (token == NULL) continue;
        arrival = atoi(token);
        
        token = strtok(NULL, ":");
        if (token == NULL) continue;
        service = atoi(token);
        
        token = strtok(NULL, ":");
        priority = (token != NULL) ? atoi(token) : 0;
        
        if (count >= capacity)
        {
            capacity *= 2;
            jobs = realloc(jobs, sizeof(Job *) * capacity);
        }

        Job *job = malloc(sizeof(Job));
        job->pid = pid;
        job->arrival_time = arrival;
        job->service_time = service;
        job->remaining_time = service;
        job->priority = priority;
        job->queue_level = 0;
        job->current_queue_level = 0;
        job->time_slice_used = 0;
        job->state = NEW;
        job->start_time = -1;
        job->end_time = -1;
        job->time_in_ready = 0;
        job->time_in_running = 0;
        job->time_in_io = 0;
        job->next = NULL;

        jobs[count++] = job;
    }

    fclose(file);
    *job_list_ptr = jobs;
    return count;
}