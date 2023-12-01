#include "schedulers.h"
#include "list.h"
#include "cpu.h"
#include <stdlib.h>
#include <stddef.h> 
#include<stdio.h>

//keep data from file in this list
struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;

void add(char *name, int priority, int burst, int arrival_time){
    Task* task = malloc(sizeof(Task));
    task->burst = burst;
    task->name = name;
    task->priority = priority;
    task->tid = 0000; //todo: figure this out
    task->arrival_time = arrival_time;
    task->completion_time = -1;//initially not determined 
    task->wait_time = -1;//initially not determined 
    task->turn_around_time = -1;//initially not determined 
    task->remaining_burst_time = burst;
    insert(list_head_ptr,task);
}

void schedule(int num_processes) {
    struct node* current = list_head;
    while (current != NULL) {
        run(current->task, current->task->burst);
        current = current->next;
    }
}


