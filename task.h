/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int arrival_time;
    int completion_time;
    int turn_around_time;
    int wait_time;
    int remaining_burst_time;
 } Task;

#endif
