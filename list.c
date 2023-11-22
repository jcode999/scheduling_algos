/**
 * Various list operations
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}
void insert_fifo(struct node** tail,struct node** head,Task *newTask){
    
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    if(*head == NULL){
        
        *head = newNode;
        *tail = newNode;
        
        return;
    }

    (*tail)->next = newNode;
    *tail = newNode;
    

}

// delete the selected task from the list
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d] [%d] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst,temp->task->completion_time,temp->task->turn_around_time,temp->task->wait_time);
        temp = temp->next;
    }
    
}

// free the list
void clean_up(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        free(temp);
        temp = temp->next;
    }
    
}

void sort_arrival_time(struct node* head,struct node* array[]){
    struct node* current_min = head;
    struct node* current_item = head;
    Task* temp = NULL;
    int copy_index = 0;

    while(current_item!= NULL){
        while(current_item->next!=NULL){
            if(current_item->next->task->arrival_time < current_min->task->arrival_time){
                temp = current_min->task;
                current_min->task = current_item->next->task;
                current_item->next->task = temp;
            }
            current_item = current_item->next;
        }
        array[copy_index] = current_min;
        current_item = current_min->next;
        current_min = current_item;
        copy_index ++;

    }
    
}

void sort_priority(struct node* head,struct node* array[]){
    struct node* current_min = head;
    struct node* current_item = head;
    Task* temp = NULL;
    int copy_index = 0;

    while(current_item!= NULL){
        while(current_item->next!=NULL){
            if(current_item->next->task->priority < current_min->task->priority){
                temp = current_min->task;
                current_min->task = current_item->next->task;
                current_item->next->task = temp;
            }
            current_item = current_item->next;
        }
        array[copy_index] = current_min;
        current_item = current_min->next;
        current_min = current_item;
        copy_index ++;

    }
    


}
