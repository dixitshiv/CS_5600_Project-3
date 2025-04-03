#ifndef QUEUE_H
#define QUEUE_H

#include "job.h"

// A basic queue structure using linked list
typedef struct
{
    Job *front;
    Job *rear;
} JobQueue;

// Queue operations
void enqueue(JobQueue *q, Job *job);
Job *dequeue(JobQueue *q);
int is_empty(JobQueue *q);
void insert_sorted_by_remaining_time(JobQueue *q, Job *job); // For SJF

#endif // QUEUE_H