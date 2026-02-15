#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"
#include "queue.h"

struct queue_struct readyqueue;

void queue_init() {
    readyqueue.head = NULL;
    readyqueue.tail = NULL;
}

void queue_enqueue(struct PCB_struct *pcb) {
    if (readyqueue.head == NULL) {
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
    if (readyqueue.head == NULL) {
        return NULL;
    }
    struct PCB_struct *tmp = readyqueue.head;
    readyqueue.head = readyqueue.head->next;
    tmp->next = NULL;
    return tmp;
}