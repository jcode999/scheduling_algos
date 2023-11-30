#include "list.h"
#include<stdio.h>
#include<stdlib.h>

int main(){
    struct node* head_node = NULL;
    struct node** head= &head_node;

    struct node* tail_node = NULL;
    struct node** tail = &tail_node;

    Task* temp = malloc(sizeof(Task));
    temp->name = "P1";
    insert_fifo(tail,head,temp);
    traverse((*head));
    q_delete(head,tail,temp);

    return 0;
}