#include "task.h"
// length of a time quantum
#define QUANTUM 2

// run the specified task for the following time slice
void run(Task *task, int slice);
