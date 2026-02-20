#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"
#include "queue.h"
#include "scheduler.h"
#include "shellmemory.h"

static struct queue_struct readyqueue;

void queue_init() {
    readyqueue.head = NULL;
    readyqueue.tail = NULL;
}

void policy_enqueue(struct PCB_struct *pcb) {
    enum policy_enum policy = get_policy();
    if (policy == FCFS || policy == RR) {
        queue_enqueue_fifo(pcb);
    }
    else {
        queue_enqueue_priority(pcb);
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
    struct PCB_struct *curr = readyqueue.head;
    struct PCB_struct *prev = NULL;

    while (curr != NULL) {
        struct PCB_struct *next = curr->next;
        if (!curr->background) {
            if (prev == NULL) {
                readyqueue.head = next;
            }
            else {
                prev->next = next;
            }
            curr->next = NULL;
            mem_free(curr);
            PCB_free(curr);
        }
        else {
            prev = curr;
        }
        curr = next;
    }
    if (prev == NULL) {
        readyqueue.tail = NULL;
    }
    else {
        readyqueue.tail = prev;
    }
}

void queue_aged() {
    struct PCB_struct *curr = readyqueue.head;
    while (curr != NULL) {
        curr->priority--;
        curr = curr->next;
    }
}