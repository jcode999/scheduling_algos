void schedule_old(int num_processes){
    struct node* circular_queue_h = NULL;
    struct node** circular_queue_head = &circular_queue_h;

    struct node* circular_queue_t = NULL;
    struct node** circular_queue_tail = &circular_queue_t;

    struct node* current_running_process = NULL;
    struct node* next_process_to_run = NULL;
    
    struct gant_chart_item* chart_item[100];
    int g_insert_index = 0;

    for(int i=0;i<100;i++){
        chart_item[i] = NULL;
    }
    


    int time = 0;

    int process_in_time = 0;
    int process_run_time = 0;
    int num_completion = 0;
    //num_completion<=num_processes
    while(1){
        //get processes ready at time t
        printf("time %d: ",time);
        struct node* new_processes[num_processes];
        for(int i =0;i<num_processes;i++){
            new_processes[i] = NULL;
        }
        get_processes(time,num_processes,list_head,new_processes);
        //add those new processes to the queue
        add_processes_to_queue(new_processes,num_processes,circular_queue_head,circular_queue_tail);
        // traverse(circular_queue_h);
        //if no process is running [start from head]
        if(current_running_process==NULL && circular_queue_h!=NULL){
            current_running_process = circular_queue_h;
        }
        //if there is a proceess running
        if(current_running_process!=NULL){
            //run the process
            run(current_running_process->task,current_running_process->task->remaining_burst_time);
            process_run_time ++;
            current_running_process->task->remaining_burst_time = current_running_process->task->remaining_burst_time -1 ;
            //check if it ran for time qunatum
            if (process_run_time == my_quantum){

                printf("Time Quantum Reached.\n");
                
                
                struct gant_chart_item* foo = malloc(sizeof(struct gant_chart_item));
                foo->task_id=current_running_process->task->name;
                foo->run_time = process_run_time;
                chart_item[g_insert_index] = foo;
                g_insert_index++;

                next_process_to_run = current_running_process->next;
                process_run_time = 0;//reset counter
                
                
            }
            //check if current running process has finished its execution
            if(current_running_process->task->remaining_burst_time <=0){
                
                
                struct gant_chart_item* foo = malloc(sizeof(struct gant_chart_item));
                foo->task_id=current_running_process->task->name;
                foo->run_time = process_run_time;
                chart_item[g_insert_index] = foo;
                g_insert_index++;
                printf("Process Finished Execution.\n");

                next_process_to_run = current_running_process->next;
                //mark completion time
                current_running_process->task->completion_time = time+1;
                delete(circular_queue_head,current_running_process->task);
                num_completion++;
                process_run_time = 0;//reset counter
                
            }
            //circle back to head
            if(next_process_to_run==NULL)
            {
                next_process_to_run = circular_queue_h;
                //if all processes in queue has completed then exit
                if(circular_queue_h==NULL){
                    break;
                }
            }
            
            //if there is next process linked to the current process, run it next.
            current_running_process = next_process_to_run;
            

        }
        //increment the time
        time++;
        
        
 
    }
    clean_up(circular_queue_h);
    report(list_head,num_processes);
    clean_up(list_head);

    int start_time = 0;
    for(int i =0;i<100;i++){
        if(chart_item[i]!=NULL){
            printf("%d--[%s]--",start_time,chart_item[i]->task_id);
            start_time = start_time + chart_item[i]->run_time;
            free(chart_item[i]);
        }
    }
}