#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "pcb.h"
#include "queue.h"
#include "scheduler.h"
#include "shellmemory.h"

static struct queue_struct readyqueue;

void queue_init() {
    readyqueue.head = NULL;
    readyqueue.tail = NULL;
}

void policy_reenqueue(struct PCB_struct *pcb) {
    enum policy_enum policy = get_policy();
    if (policy == AGING) { queue_aged_enqueue(pcb); }
    else { policy_enqueue(pcb); }
}

void policy_enqueue(struct PCB_struct *pcb) {
    enum policy_enum policy = get_policy();
    if (policy == FCFS || policy == RR || policy == RR30) {
        queue_enqueue_fifo(pcb);
    }
    else {
        queue_enqueue_priority(pcb);
    }
}

void queue_enqueue_first(struct PCB_struct *pcb) {
    pcb->next = readyqueue.head;
    readyqueue.head = pcb;
    if (readyqueue.tail == NULL) {
        readyqueue.tail = pcb;
    }
}

void queue_enqueue_fifo(struct PCB_struct *pcb) {
    if (queue_is_empty()) {
        readyqueue.head = pcb;
        readyqueue.tail = pcb;
        pcb->next = NULL;
    }
    else {
        readyqueue.tail->next = pcb;
        readyqueue.tail = pcb;
        pcb->next = NULL;
    }
}

void queue_enqueue_priority(struct PCB_struct *pcb) {
    if (queue_is_empty()) {
        readyqueue.head = pcb;
        readyqueue.tail = pcb;
        pcb->next = NULL;
    }
    else {
        struct PCB_struct *prev = NULL;
        struct PCB_struct *curr = readyqueue.head;
        while (curr != NULL && compare_priority(curr, pcb) <= 0) {
            prev = curr;
            curr = curr->next;
        }
        if (prev == NULL) {
            pcb->next = readyqueue.head;
            readyqueue.head = pcb;
        }
        else {
            prev->next = pcb;
            pcb->next = curr;
        }
        if (curr == NULL) {
            readyqueue.tail = pcb;
        }
    }
}

int compare_priority(struct PCB_struct *curr, struct PCB_struct *pcb) {
    enum policy_enum policy = get_policy();
    if (policy == SJF) {
        return curr->length - pcb->length;
    }
    if (policy == AGING) {
        return curr->priority - pcb->priority;
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
    readyqueue.head = NULL;
    readyqueue.tail = NULL;
}

void queue_aged() {
    struct PCB_struct *curr = readyqueue.head;
    while (curr != NULL) {
        if (curr->priority > 0) { curr->priority--; }
        curr = curr->next;
    }
}

void queue_aged_enqueue(struct PCB_struct *pcb) {
    if (queue_is_empty()) {
        readyqueue.head = pcb;
        readyqueue.tail = pcb;
        pcb->next = NULL;
        return;
    }

    if (readyqueue.head->priority < pcb->priority) {
        queue_enqueue_priority(pcb);
    }
    else {
        pcb->next = readyqueue.head;
        readyqueue.head = pcb;

        if (readyqueue.tail == NULL) {
            readyqueue.tail = pcb;
        }
    }
}