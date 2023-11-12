#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"
#include "cpu.h"

struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;




//running processes at time 'time' returns pointer to the list
void get_processes(int time,int num_processes,struct node* head, struct node* process_list[]){
    struct node *temp;
    temp = head;
    int l_index = 0;
    //[todo: implement other efficient searching algorithm]
    while (temp != NULL) {
        if(temp->task->arrival_time == time){
            process_list[l_index] = temp;
            l_index++;
        }
        temp = temp->next; 
    }
}

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
    
    insert(list_head_ptr,task);
}

void q_sort(struct node* queue[],int num_processes,int queue_head_index,int queue_tail_index){
    
    // printf("queue head: %d , queue tail: %d\n",queue_head_index,queue_tail_index);
    printf("finding the process with highest priority\n");
    if(queue[queue_head_index]!=NULL){
        // printf("Not Null\n");
        for(int i = queue_head_index;i<queue_tail_index;i++){
            
                struct node* current_min = queue[i];
                if(queue[i+1]!=NULL){
                for(int j = i+1;j <queue_tail_index;j++){
                    
                        struct node* current_item = queue[j];
                        if(current_min->task->priority > current_item->task->priority){
                            struct node* temp = current_min;
                            queue[i] = current_item;
                            queue[j] = temp;
                        
                        }
                    }
                }
            }
        // printf("resturning:(head index: %d) %s\n",queue_head_index,queue[queue_head_index]->task->name);
        return;
    }
    printf("Empty Queue \n");
    
    
    
}


void schedule(int num_processes){

    //get a copy of the list as an array
    //struct node* copy_array[num_processes];
    //this function fills the array with sorted list
    //sort_arrival_time(list_head,copy_array);

    struct node* current_running_process = NULL;

    //processes in ready queue waiting for cpu
    struct node* priority_queue[num_processes];
    for (int i=0; i<num_processes; i++){
        priority_queue[i] = NULL;
    }
    
    //start time
    int time = 0;

    //points to the head of the queue / (item with the highest priority)
    int queue_head_index = 0;

    int queue_tail_index = 0;
    while(1){
        
     /*info*/   printf("Time: %d\n",time);
                            printf("Processes in queue: ");
                            for (int i=queue_head_index; i<queue_tail_index; i++){
                                
                                if(priority_queue[i] != NULL){
                                    
                                    printf("[%s],",priority_queue[i]->task->name);
                                }
                                
                            }
     /*info*/   printf("\n");
    

    //check if currently running process has completed its execution
    if(current_running_process!=NULL){
        current_running_process->task->burst = current_running_process->task->burst - 1;
        if(current_running_process->task->burst<=0){
            printf("Current Running process %s finished its execution\n",current_running_process->task->name);
           
                priority_queue[queue_head_index] = NULL; //remove from the queue
                queue_head_index ++; //change pointer to the queue head

                if(queue_head_index==num_processes){
                    printf("All Process Finished..\n");
                    break;
                }
            
        }
    }
    
    //get processes ready at time t
    struct node* new_processes[num_processes];
    for(int i =0;i<num_processes;i++){
        new_processes[i] = NULL;
    }
    get_processes(time,num_processes,list_head,new_processes);
    
    //add new processes to the queue
    if(new_processes[0]!=NULL){
        
        printf("Adding following processes to the queue: ");
        for(int i = 0;i<num_processes;i++){
            if(new_processes[i]!=NULL){
            printf("[%s],",new_processes[i]->task->name);
            priority_queue[queue_tail_index] = new_processes[i];
            queue_tail_index=queue_tail_index+1;
            }
        }
        printf("\n");
    }
    
    if(priority_queue[queue_head_index]!=NULL){
    printf("New queue:[%d %d) [",queue_head_index,queue_tail_index);
    for(int i= queue_head_index;i<queue_tail_index;i++){
        printf("%s ",priority_queue[i]->task->name);
    }
    printf("]");
    printf("\n");
    }
   
    
    //run the process with highest priority
    q_sort(priority_queue,num_processes,queue_head_index,queue_tail_index);

    if(priority_queue[queue_head_index]!=NULL){
    struct node* process_to_run = priority_queue[queue_head_index];
    current_running_process = process_to_run;
    printf("Process with highest priority %s \n",process_to_run->task->name);
    run(process_to_run->task,process_to_run->task->burst);
    }
    
    

    time ++;

    
    

    printf("\n");
    


    }
    
    


}
