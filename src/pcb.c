#include "pcb.h"
#include <stdlib.h>

static int PID = 1;
int PID_get_value() {
    return PID++;
}

struct PCB_struct *PCB_init(int start) {
    struct PCB_struct *pcb = malloc(sizeof(struct PCB_struct));
    pcb->pid =  PID_get_value();
    pcb->background = 0;
    pcb->pc = start;
    pcb->start = start;
    pcb->length = 0;
    pcb->priority = 0;
    pcb->next = NULL;
    return pcb;
}

void PCB_free(struct PCB_struct *pcb) {
    if (pcb == NULL) { return; }
    free(pcb);
}


