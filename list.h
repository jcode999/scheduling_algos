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
void sort_arrival_time(struct node *head);
void sort_priority(struct node *head,struct node* array[]);
void clean_up(struct node* head);
void insert_fifo(struct node **tail,struct node **head, Task *task);
struct node* get_tail(struct node* head);
struct node* q_delete(struct node **head,struct node **tail, Task *task);