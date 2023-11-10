#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int burst, int arrival, int priority);

// invoke the scheduler
void schedule(int num_processes);

//test
void schedule_fcfs(int length);

void get_processes(int time,int num_processes,struct node** head,struct node* process_list[]);
