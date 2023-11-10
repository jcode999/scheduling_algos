/**
 * list data structure containing the tasks in the system
 */

#include "task.h"

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);
void sort_arrival_time(struct node *head,struct node* array[]);
void sort_priority(struct node *head,struct node* array[]);
