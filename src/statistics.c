#include "../include/statistics.h"
#include <stdio.h>
#include <limits.h>

void print_statistics(Job **job_list, int job_count, int total_time)
{
    int total_turnaround = 0;
    int total_waiting = 0;
    int total_running = 0;
    int total_io = 0;

    int min_turnaround = INT_MAX;
    int max_turnaround = INT_MIN;

    printf("\n===== Job Statistics =====\n");
    for (int i = 0; i < job_count; i++)
    {
        Job *job = job_list[i];
        int turnaround_time = job->end_time - job->arrival_time;
        int waiting_time = job->time_in_ready;

        total_turnaround += turnaround_time;
        total_waiting += waiting_time;
        total_running += job->time_in_running;
        total_io += job->time_in_io;

        if (turnaround_time < min_turnaround)
            min_turnaround = turnaround_time;
        if (turnaround_time > max_turnaround)
            max_turnaround = turnaround_time;

        printf("Job %d:\n", job->pid);
        printf("  Arrival Time: %d\n", job->arrival_time);
        printf("  Service Time: %d\n", job->service_time);
        printf("  Start Time:   %d\n", job->start_time);
        printf("  End Time:     %d\n", job->end_time);
        printf("  Turnaround Time: %d\n", turnaround_time);
        printf("  Time in READY:   %d\n", job->time_in_ready);
        printf("  Time in RUNNING: %d\n", job->time_in_running);
        printf("  Time in I/O:     %d\n\n", job->time_in_io);
    }

    printf("===== Summary =====\n");
    printf("Total Jobs Run: %d\n", job_count);
    printf("Total Elapsed Time: %d\n", total_time);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / job_count);
    printf("Average Waiting Time:    %.2f\n", (float)total_waiting / job_count);
    printf("Longest Job Turnaround:  %d\n", max_turnaround);
    printf("Shortest Job Turnaround: %d\n", min_turnaround);
    printf("Total CPU Time Used:     %d\n", total_running);
    printf("Total I/O Time:          %d\n", total_io);
    printf("CPU Utilization:         %.2f%%\n", (total_running * 100.0) / total_time);
}