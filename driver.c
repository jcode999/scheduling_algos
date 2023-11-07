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


    in = fopen(argv[1],"r");

    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        burst = atoi(strsep(&temp,","));
        arrival = atoi(strsep(&temp,","));
        priority = atoi(strsep(&temp,","));
        
        
        // add the task to the scheduler's list of tasks
        add(name,burst,arrival,priority);

        free(temp);
    }

    fclose(in);
    
    // invoke the scheduler
    schedule();

    return 0;
}
