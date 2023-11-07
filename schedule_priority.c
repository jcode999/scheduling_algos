#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"
#include "cpu.h"

struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;

void add(char *name, int priority, int burst, int arrival_time){
    Task* task = malloc(sizeof(Task));
    task->burst = burst;
    task->name = name;
    task->priority = priority;
    task->tid = 0000;
    task->arrival_time = arrival_time;
    
    insert(list_head_ptr,task);
}

void schedule(){
    printf("schedule function.\n");
    traverse(list_head);
    run(list_head->task,list_head->task->burst);
    
}
