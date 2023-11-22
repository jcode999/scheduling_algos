#include "list.h"
#include "task.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

#define my_quantum 4

struct node* list_head = NULL;
struct node** list_head_ptr = &list_head;





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
void add_processes_to_queue(struct node* new_processes[],int queue_length,struct node** circular_queue_head,struct node** circular_queue_tail){
    // printf("New Processes: ");
        if(new_processes[0]!=NULL){
            // printf("[");
            for(int i=0;i<queue_length;i++){
                if(new_processes[i]!=NULL){
                    // printf("%s ",new_processes[i]->task->name);
                    printf("Inserting [%s] in the circular queue\n",new_processes[i]->task->name);
                    insert_fifo(circular_queue_tail,circular_queue_head,new_processes[i]->task);
                }

            }
            printf("]\n");

        }
        else
        printf("[]\n");

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




void schedule(int num_processes){
    struct node* circular_queue_h = NULL;
    struct node** circular_queue_head = &circular_queue_h;

    struct node* circular_queue_t = NULL;
    struct node** circular_queue_tail = &circular_queue_t;

    struct node* current_running_process = NULL;
    struct node* next_process_to_run = NULL;
    
    


    int time = 0;

   
    int process_run_time = 0;
    int num_completion = 0;
    printf("QUANTUM: %d\n",my_quantum);
    //num_completion<=num_processes
    while(1){
        //current process start time
        printf("time: %d\n",time);
        printf("completed processes: %d\n",num_completion);
        
        
        //get processes ready at time t
        struct node* new_processes[num_processes];
        for(int i =0;i<num_processes;i++){
            new_processes[i] = NULL;
        }
        get_processes(time,num_processes,list_head,new_processes);
        add_processes_to_queue(new_processes,num_processes,circular_queue_head,circular_queue_tail);
        printf("[TEST][schedule_rr()]: Traversing the ll_queue \n");
        traverse(*circular_queue_head);

        //if no process is running [start from head]
        if(current_running_process==NULL && circular_queue_h!=NULL){
            printf("No Current Running Process. \nSetting new process [%s]\n",circular_queue_h->task->name);
            current_running_process = circular_queue_h;
        }
        //if there is a proceess running
        if(current_running_process!=NULL){
            printf("Current Running Process: [%s]\n",current_running_process->task->name);
            //run the process
            run(current_running_process->task,current_running_process->task->burst);
            current_running_process->task->burst = current_running_process->task->burst -1;
            process_run_time++;
            //check if it ran for time qunatum
            if (process_run_time == my_quantum){
                next_process_to_run = current_running_process->next;
                if(next_process_to_run!=NULL)
                printf("Current Process Qunatum Reached.\n Next Process: [%s] \n",next_process_to_run->task->name);
                else{
                    printf("Current Process Qunatum Reached.\n Next Process: [NULL] \n");
                }
                process_run_time = 0;//reset counter
            }
            //check if current running process has finished its execution
            if(current_running_process->task->burst <=0){
                next_process_to_run = current_running_process->next;
                delete(circular_queue_head,current_running_process->task);
                num_completion++;
                if(next_process_to_run!=NULL)
                printf("Current Process Finished Its Execution.\n Next Process: [%s] \n",next_process_to_run->task->name);
                printf("queue after deletion \n");
                traverse(circular_queue_h);
            }
            if(next_process_to_run==NULL) //end of the queue, restart from head.
            {
                printf("end of the queue reached.. Quantum[%d]\n",my_quantum);
                next_process_to_run = circular_queue_h;
                if(circular_queue_h==NULL){//if all processes in queue has completed then exit
                    break;
                }
            }
            current_running_process = next_process_to_run;
            

        }
        time++;
        printf("\n");
        
 
    }
    clean_up(circular_queue_h);
}