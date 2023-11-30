#include "list.h"
#include "task.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

#define my_quantum 10

struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;

struct node* list_tail = NULL;
struct node** list_tail_ptr = &list_tail;

struct gant_chart_item{
    char * task_id;
    int run_time;
};

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
void get_processes_untill_time(int start_time,int end_time,int num_processes,struct node* head, struct node* process_list[],struct node* current_queue){
    for (int i =0;i<num_processes;i++){
        process_list[i] = NULL;
    }
    struct node *temp;
    temp = head;
    int l_index = 0;
    //[todo: implement other efficient searching algorithm]
    while (temp != NULL) {
        if((temp->task->arrival_time>=start_time) && (temp->task->arrival_time <= end_time)){
            if((is_here(temp->task->name,current_queue)==0)){
            process_list[l_index] = temp;
            // printf("%s ",process_list[l_index]->task->name);
            l_index++;
            }
        }
        temp = temp->next; 
    }
    // printf("]\n");
}

void add_processes_to_queue(struct node* new_processes[],int queue_length,struct node** circular_queue_head,struct node** circular_queue_tail){
    
        if(new_processes[0]!=NULL){
            for(int i=0;i<queue_length;i++){
                if(new_processes[i]!=NULL){
                    
                    insert_fifo(circular_queue_tail,circular_queue_head,new_processes[i]->task);
                }
            }
        }
      

}

void add(char *name, int priority, int burst, int arrival_time){
    Task* task = malloc(sizeof(Task));
    task->burst = burst;
    task->name = name;
    task->priority = priority;
    task->tid = 0000; //todo: figure this out
    task->arrival_time = arrival_time;
    task->remaining_burst_time = task->burst; //updates as process runs.initially same as the burst time
    task->completion_time = -1;//initially not determined 
    task->wait_time = -1;//initially not determined 
    task->turn_around_time = -1;//initially not determined 
    
    insert_fifo(list_tail_ptr,list_head_ptr,task);
}

void report(struct node* task_list,int num_processes){
    struct node *temp;
    temp = task_list;
    int total_wait_time = 0;
    while (temp != NULL) {
        //Turn Around Time = Completion Time - Arrival Time
        temp->task->turn_around_time = temp->task->completion_time - temp->task->arrival_time;
        //Waiting Time = Turn Around Time - Burst Time
        temp->task->wait_time = temp->task->turn_around_time - temp->task->burst;
        total_wait_time = temp->task->wait_time + total_wait_time;
        printf("[%s] [%d] [%d] [%d]\n",temp->task->name,temp->task->completion_time,temp->task->turn_around_time,temp->task->wait_time);
        // printf("[%s] [%d] [%d] [%d] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst,temp->task->completion_time,temp->task->turn_around_time,temp->task->wait_time);
        temp = temp->next;
    }
    printf("Total Waiting Time: %d\n",total_wait_time);
    printf("Average Waiting Time: %d\n", total_wait_time/num_processes);
}


void schedule(int num_process){
    printf("quantum: %d\n ",QUANTUM);
    struct node* circular_queue = NULL;
    struct node** head = &circular_queue;

    struct node* circular_queue_tail = NULL;
    struct node** tail = &circular_queue_tail;

    struct node* new_processes[num_process];
    for (int i =0;i<num_process;i++){
        new_processes[i] = NULL;
    }
    struct node* current_process = NULL;
    int process_enter_time = 0;

    //if there is no process running 
    if(current_process == NULL ){
       //fetch the first process to enter and mark it as the current process
       struct node temp;
       current_process = &temp;
       current_process->task = list_head->task;
       current_process->next = NULL;
       new_processes[0] = current_process;
       add_processes_to_queue(new_processes,num_process,head,tail);
       current_process= *head;
       process_enter_time = current_process->task->arrival_time;
    }
    int slice = 0;
    int completed_processes = 0;
    struct node* next_process = NULL;
    
    while(completed_processes<=num_process){
        // printf("current queue\n");
        // traverse(circular_queue);
        if(current_process->task->remaining_burst_time<=QUANTUM){
           slice = current_process->task->remaining_burst_time;
        }
        else{
            slice = QUANTUM;
        }
       
        //run the process for that duration
        run(current_process->task,slice);
        

        //get processes between this time frame
        //printf("fetching new processes from time [%d to %d]\n",process_enter_time, process_enter_time + slice);
        get_processes_untill_time(process_enter_time,process_enter_time + slice,num_process,list_head,new_processes,circular_queue);

        //add those processes to the queue
        add_processes_to_queue(new_processes,num_process,head,tail);
        next_process = current_process->next;
        

        if(current_process->task->remaining_burst_time<=QUANTUM){  
            current_process->task->completion_time=process_enter_time+slice;   
            printf("[%s] completed at [%d]\n",current_process->task->name,current_process->task->completion_time);
            q_delete(head,tail,current_process->task);
            completed_processes++;
        }
         else{
            slice = QUANTUM;
            current_process->task->remaining_burst_time = current_process->task->remaining_burst_time - slice;
        }
        
        if(next_process==NULL && circular_queue!=NULL){//reached end of the ready queue
            next_process = circular_queue;
        }

        if(next_process==NULL && completed_processes<num_process){//reached end of the ready queue but not all processes have been scheduled
            //seek continuously untill we find a process to run
            
            int start = process_enter_time + slice;
            while(new_processes[0]==NULL){
            int end = start + QUANTUM;
            get_processes_untill_time(start,end,num_process,list_head,new_processes,circular_queue);
            add_processes_to_queue(new_processes,num_process,head,tail);
            
            next_process = circular_queue;
            start = end;
            }
            process_enter_time = next_process->task->arrival_time;
            slice = 0;//yet to be determinded for this process
            
            
        }
        if(next_process==NULL){
            break;
        }
        process_enter_time = process_enter_time + slice;
        current_process = next_process;
       printf("next process[%s]  enter time: %d remaining burst: %d\n",next_process->task->name, process_enter_time,next_process->task->remaining_burst_time);
        

    //    printf("\n");
    
    }
    printf("\nReport\n<proc><comp><TAT><wait>\n");
    report(list_head,num_process);
    clean_up(circular_queue);
    // clean_up(list_head);
    



}