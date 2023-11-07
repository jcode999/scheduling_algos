#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int burst, int arrival, int priority);

// invoke the scheduler
void schedule();
