#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "pcb.h"
#include "queue.h"
#include "shellmemory.h"
#include "shell.h"

static enum policy_enum policy = FCFS;
static int quantum = 2;

void set_policy(enum policy_enum scheduling_policy) {
    policy = scheduling_policy;
}

enum policy_enum get_policy() {
    return policy;
}

int scheduler() {
    int errCode = 0;
    if (policy == FCFS || policy == SJF) {
        errCode = non_preemptive_scheduler();
    }
    else {
        errCode =  preemptive_scheduler();
    }
    queue_destroy();
    return errCode;
}

int non_preemptive_scheduler() {
    int errCode = 0;
    while (!queue_is_empty()) {
        struct PCB_struct *pcb = queue_dequeue();
        while (pcb->pc < pcb->start + pcb->length) {
            char *line = mem_get_from(pcb->pc);
            errCode = parseInput(line); // which calls interpreter()
            pcb->pc++;
        }
        mem_free(pcb);
        PCB_free(pcb);
    }
    return errCode;
}

int preemptive_scheduler() {
    int errCode = 0;

    if (policy == RR) {
        quantum = 2;
    }
    else if (policy == AGING) {
        quantum = 1;
    }

    while (!queue_is_empty()) {
        struct PCB_struct *pcb = queue_dequeue();
        int counter = quantum;
        while (counter > 0 && pcb->pc < pcb->start + pcb->length) {
            char *line = mem_get_from(pcb->pc);
            errCode = parseInput(line); // which calls interpreter()
            pcb->pc++;
            counter--;
        }
        if (policy == AGING) {
            queue_aged();
        }
        if (pcb->pc < pcb->start + pcb->length) {
            policy_enqueue(pcb);
        }
        else {
            mem_free(pcb);
            PCB_free(pcb);
        }
    }
    return errCode;
}