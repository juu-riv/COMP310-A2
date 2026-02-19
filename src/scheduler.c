#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "pcb.h"
#include "queue.h"
#include "shellmemory.h"
#include "interpreter.h"

static enum policy_enum policy = FCFS;
static int quantum = 2;

void set_policy(enum policy_enum scheduling_policy) {
    policy = scheduling_policy;
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
            errCode = scheduler_to_interpreter(line);
            pcb->pc++;
        }
        mem_free(pcb);
        PCB_free(pcb);
    }
    return errCode;
}

int scheduler_to_interpreter(char *line) {
    int errCode = 0;
    char *copy = strdup(line);

    char *argv[MAX_ARGS];
    int argc = 0;

    char *token = strtok(copy, " ");
    while (token != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    errCode = interpreter(argv, argc);
    free(copy);
    return errCode;
}