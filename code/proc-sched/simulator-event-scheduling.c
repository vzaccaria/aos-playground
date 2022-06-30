#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/** Type definitions **/
typedef struct task_description {
    const char *name;
    unsigned int a_time;
    unsigned int C_time;
    unsigned int remaining_time;
} task_t;


/** Task set and simulation definitions **/
#define MAX_RUNNING_TIME 100
#define NR_TASKS 4
#define RR_QUANTUM 2

task_t taskset[NR_TASKS] = {
        { .name="T0", .a_time=2, .C_time=7 },
        { .name="T1", .a_time=6, .C_time=1 },
        { .name="T2", .a_time=0, .C_time=3 },
        { .name="T3", .a_time=3, .C_time=3 },
};

/** Simulation variables **/

task_t *running_task = NULL;
task_t *history[MAX_RUNNING_TIME];

/** Scheduling policy **/

task_t *SJF(task_t *ready_queue[], unsigned int nr_ready_queue, int t) {

    if (running_task != NULL) {
        // SJF is non-preemptive, we cannot preempt the task
        return running_task;
    }
    
    int min_idx=-1;
    unsigned int i, min_C = INT_MAX;
    for(i=0; i<nr_ready_queue; i++) {
        if (ready_queue[i]->C_time < min_C) {
            min_C = ready_queue[i]->C_time;
            min_idx = i;
        }
    }
    
    if (min_idx == -1) {
        return NULL;
    } else {
        return ready_queue[min_idx];
    }
}

/** Select scheduling policy **/

task_t * (*schedpol)(task_t *[], unsigned int, int) = &SJF;

/** Simulator code **/

void simulator(void) {
    unsigned int i;
    for (i=0; i<NR_TASKS; i++) {
        taskset[i].remaining_time = taskset[i].C_time;
    }
    
    int completed = 0;
    unsigned int t;
    for (t=0; t<MAX_RUNNING_TIME; t++) {
        // For each time unit...
        
        if (running_task != NULL && running_task->remaining_time == 0) {
            // Previously running task has terminated
            running_task = NULL; 
            completed++;
        }
        
        if (completed == NR_TASKS) {
            break;
        }
        
        // Creation of the ready queue
        task_t *ready_queue[NR_TASKS];  
        int nr_task_ready = 0;

        for (i=0; i<NR_TASKS; i++) {
            if ( t >= taskset[i].a_time && taskset[i].remaining_time > 0) {
                // Task is ready and not yet finished
                ready_queue[nr_task_ready] = &taskset[i];
                nr_task_ready++;
            }
        }
        
        task_t *next_task = schedpol(ready_queue, nr_task_ready, t);
        next_task->remaining_time = next_task->remaining_time - 1;

        printf("t=%3d: %4s", t, next_task->name);

        if (running_task != NULL && running_task != next_task) {
            printf(" PREEMPTION OCCURRED");
        }
        printf("\n");
        
        running_task = next_task;
    }

}

int main()
{

    simulator();

}
