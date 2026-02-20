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
    if (policy == FCFS) {
        errCode = FCFS_scheduler();
    }
    return errCode;
}

int FCFS_scheduler() {
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