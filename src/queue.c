#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"
#include "queue.h"

static struct queue_struct readyqueue;

void queue_init() {
    readyqueue.head = NULL;
    readyqueue.tail = NULL;
}

void queue_enqueue(struct PCB_struct *pcb) {
    if (queue_is_empty()) {
        readyqueue.head = pcb;
        readyqueue.tail = pcb;
        pcb->next = NULL;
    }
    else {
        readyqueue.tail->next = pcb;
        readyqueue.tail = pcb;
    }
}

struct PCB_struct *queue_dequeue() {
    if (queue_is_empty()) {
        return NULL;
    }
    struct PCB_struct *tmp = readyqueue.head;
    readyqueue.head = readyqueue.head->next;
    tmp->next = NULL;
    return tmp;
}

int queue_is_empty() {
    return readyqueue.head == NULL;
}

void queue_destroy() {
    while (!queue_is_empty()) {
        struct PCB_struct *pcb = queue_dequeue();
        PCB_free(pcb);
    }
}