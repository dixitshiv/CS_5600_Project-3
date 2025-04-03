#ifndef JOB_H
#define JOB_H

// To track where job is in its lifecycle
typedef enum
{
    NEW,
    READY,
    RUNNING,
    WAITING_IO,
    FINISHED
} JobState;

// Represents a process with all data/stats
typedef struct Job
{
    int pid;            // Process ID
    int arrival_time;   // Clock tick when the job arrives
    int service_time;   // Total amount of CPU time the job needs to complete
    int remaining_time; // Amount of CPU time left for the job to finish
    int priority;       // The job's initial priority (Not used in SJF/RR)
    int queue_level;

    // Current state
    JobState state;

    // Statistics tracking
    int time_in_ready;   // Total time the job spent in the ready queue
    int time_in_running; // Total time the job spent running on the CPU
    int time_in_io;      // Total time the job spent waiting for I/O
    int start_time;      // When the job first started running
    int end_time;        // When the job completed

    // Round Robin support
    int time_slice_used;

    // MLFQ support
    int current_queue_level;

    // Linked list pointer for queues
    struct Job *next;
} Job;

int read_job_file(const char *filename, Job ***job_list_ptr);

#endif // JOB_H