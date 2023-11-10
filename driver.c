/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int arrival;

    int num_processes = 0;


    in = fopen(argv[1],"r");

    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        burst = atoi(strsep(&temp,","));
        arrival = atoi(strsep(&temp,","));
        priority = atoi(strsep(&temp,","));
        num_processes = num_processes + 1;
        
        
        // add the task to the scheduler's list of tasks
        add(name,priority,burst,arrival);

        free(temp);
    }

    fclose(in);
    
    // invoke the scheduler
    printf("scheduling %d processes\n",num_processes);
    schedule(num_processes);

    return 0;
}
