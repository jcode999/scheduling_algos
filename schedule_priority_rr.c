#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "task.h"
#include "cpu.h"
#include "schedulers.h"
//keep data from file in this list
struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;

int is_here(char* task,struct node* queue){
    struct node *temp;
    temp = queue;
    while (temp != NULL) {
        if(temp->task->name == task){

            return 1;
        }
        temp = temp->next; 
    }
    return 0;


}
int is_here_array(char* task,struct node* list[],int size){
    for(int i=0;i<size;i++){
        if(list[i]!=NULL)
        if(list[i]->task->name == task)
        return 1;
    }
    return 0;
}

void initialize_null_array(struct node* list[],int size){
    for (int i=0;i<size;i++){
        (list[i]) = NULL;
    }

}
void initialize_empty(int list[],int size){
    for (int i=0;i<size;i++){
        (list[i]) = -1;
    }

}
void get_processes_untill_time(int start_time,int end_time,int num_processes,struct node* head, struct node* process_list[],struct node* current_queue,struct node* completed_list[]){
    for (int i =0;i<num_processes;i++){
        process_list[i] = NULL;
    }
    struct node *temp;
    temp = head;
    int l_index = 0;
    //[todo: implement other efficient searching algorithm]
    while (temp != NULL) {
        if((temp->task->arrival_time>=start_time) && (temp->task->arrival_time <= end_time)){
            if((is_here(temp->task->name,current_queue)==0) && is_here_array(temp->task->name,completed_list,num_processes)==0){
            process_list[l_index] = temp;
            // printf("%s ",process_list[l_index]->task->name);
            l_index++;
            }
        }
        temp = temp->next; 
    }
    // printf("]\n");
}
void add_to_queue(int p_index, struct node* queue[],Task *newTask){
    
    printf("adding task %s index %d\n",newTask->name,p_index);
    struct node** head = &queue[p_index];
    struct node** tail = NULL;
    struct node* tail_node = NULL;
    if(queue[p_index]==NULL)
        tail = &queue[p_index];
    else{
        tail_node = get_tail(queue[p_index]);
        tail = &tail_node;
    }

    
    insert_fifo(tail,head,newTask);

}

void round_robin(struct node* head_node,int *group_enter_time, struct node* queue[],int queue_indexes[],int num_processes,struct node* completed_processes[],int *completed_index, int* k){
    //k is the index of the current running group
    if(head_node==NULL){
        return;
    }
    struct node** head = &head_node;
    struct node* tail_node = get_tail(head_node);
    struct node** tail = &tail_node;

    struct node* current_process = head_node;
    struct node* next_process = NULL;
    int slice = 0;
    
    
    
    while(1){
        printf("List not empty\n");
        if(current_process->task->remaining_burst_time<=QUANTUM){
           slice = current_process->task->remaining_burst_time;
        }
        else{
            slice = QUANTUM;
        }
        //run the process for that duration
        run(current_process->task,slice);
        
        
        if(current_process->task->remaining_burst_time<=QUANTUM){  
            current_process->task->completion_time= (*group_enter_time)+slice;
            completed_processes[*completed_index] = current_process;
            (*completed_index)++;
            printf("[%s] completed at [%d]\n",current_process->task->name,current_process->task->completion_time);
            q_delete(head,tail,current_process->task);
        }
        else{
            slice = QUANTUM;
            current_process->task->remaining_burst_time = current_process->task->remaining_burst_time - slice;
        }

        
        
        struct node* new_processes[num_processes];
        initialize_null_array(new_processes,num_processes);
        get_processes_untill_time(*group_enter_time,*group_enter_time+slice,num_processes,list_head,new_processes,head_node,completed_processes);
         for(int i=0;i<num_processes;i++){
            if(new_processes[i]!=NULL){
                int p_index = new_processes[i]->task->priority;
                add_to_queue(p_index,queue,new_processes[i]->task);

            }
         } 
        int insert_index = 0;
        for(int i=0;i<MAX_PRIORITY;i++){
            if(queue[i]!=NULL){
                queue_indexes[insert_index] = queue[i]->task->priority;
                insert_index++;
            }
        }
        printf("Priority List(Unsorted): [");
        for(int i=0;i<MAX_PRIORITY;i++){
            if(queue_indexes[i]!=-1){
                printf("%d ",queue_indexes[i]);
            }

        }
        printf("]\n");

        if(queue_indexes[*k]<current_process->task->priority){
            *group_enter_time = *group_enter_time + slice;
            round_robin(queue[queue_indexes[*k]],group_enter_time,queue,queue_indexes,num_processes,completed_processes,completed_index,k);
        }
        printf("no process with higher priority at the moment..\n");
        next_process = current_process->next;
        if(next_process==NULL && head!=NULL){//reached end of the ready queue go back to head
            next_process = *head;
        }
        if(next_process==NULL){
            printf("all process complete for this queue\n Returning...\n");
            *k = (*k)+1;
            round_robin(queue[queue_indexes[*k]],group_enter_time,queue,queue_indexes,num_processes,completed_processes,completed_index,k);
            break;
        }
        *group_enter_time = *group_enter_time + slice;
        current_process = next_process;

        }
        return;

}


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
                        //apply fifo for tasks with same priority
                        else if(current_min->task->priority == current_item->task->priority){
                            if(current_min->task->arrival_time > current_item->task->arrival_time){
                                struct node* temp = current_min;
                                queue[i] = current_item;
                                queue[j] = temp;
                            }

                        }
                    }
                }
            }
        // printf("resturning:(head index: %d) %s\n",queue_head_index,queue[queue_head_index]->task->name);
        return;
    }
    printf("Empty Queue \n");
    
    
    
}

//add tasks to the above list
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

void schedule(int num_processes){
    //creating queues
    struct node* queue[MAX_PRIORITY];
    int queue_indexes[MAX_PRIORITY];
    initialize_empty(queue_indexes,MAX_PRIORITY);

    initialize_null_array(queue,MAX_PRIORITY);
    printf("initialized..\n");
    

    //select the first process to arrive initially
    struct node* new_processes[num_processes];

    sort_arrival_time(list_head);
    initialize_null_array(new_processes,num_processes);
    //could be more than one processes arriving at the same time
    get_processes(list_head->task->arrival_time,num_processes,list_head,new_processes);
    
    printf("sorted(arrival time)\n");
    traverse(list_head);

    for(int i=0;i<num_processes;i++){
        if(new_processes[i]!=NULL){
            int p_index = new_processes[i]->task->priority;
            add_to_queue(p_index,queue,new_processes[i]->task);

        }
    }
    
    int insert_index = 0;
    for(int i=0;i<MAX_PRIORITY;i++){
        if(queue[i]!=NULL){
            queue_indexes[insert_index] = queue[i]->task->priority;
            insert_index++;
        }

    }
    printf("Priority List(Unsorted): [");
     for(int i=0;i<MAX_PRIORITY;i++){
        if(queue_indexes[i]!=-1){
           printf("%d ",queue_indexes[i]);
        }

    }
    printf("]\n");
    
    struct node* group = queue[queue_indexes[0]];
    struct node* completed[num_processes];
    initialize_null_array(completed,num_processes);
    int group_enter_time = 0;
    printf("Running group with p %d\n",group->task->priority);
    printf("following processes will be run using rr\n");
    traverse(group);
    int completed_index = 0;
    int k = 0;
    round_robin(group,&group_enter_time,queue,queue_indexes,num_processes,completed,&completed_index,&k);


    //reset queue index;


    
    
}
