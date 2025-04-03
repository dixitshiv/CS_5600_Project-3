#include "../include/queue.h"
#include <stdlib.h>

void enqueue(JobQueue *q, Job *job)
{
    job->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = job;
    }
    else
    {
        q->rear->next = job;
        q->rear = job;
    }
}

Job *dequeue(JobQueue *q)
{
    if (q->front == NULL)
        return NULL;

    Job *job = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    job->next = NULL;
    return job;
}

int is_empty(JobQueue *q)
{
    return q->front == NULL;
}

// For Preemptive SJF: insert by remaining_time in ascending order
void insert_sorted_by_remaining_time(JobQueue *q, Job *job)
{
    job->next = NULL;

    if (q->front == NULL || job->remaining_time < q->front->remaining_time)
    {
        job->next = q->front;
        q->front = job;
        if (q->rear == NULL)
            q->rear = job;
        return;
    }

    Job *current = q->front;
    while (current->next != NULL && current->next->remaining_time <= job->remaining_time)
    {
        current = current->next;
    }

    job->next = current->next;
    current->next = job;

    if (job->next == NULL)
        q->rear = job;
}