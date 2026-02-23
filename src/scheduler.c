#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "pcb.h"
#include "queue.h"
#include "shellmemory.h"
#include "shell.h"

static enum policy_enum policy = FCFS;
static int quantum = 2;

static int is_running = 0;

static int is_multithreaded = 0;
static pthread_t workers[2];
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void set_policy(enum policy_enum scheduling_policy) {
    policy = scheduling_policy;
}

enum policy_enum get_policy() {
    return policy;
}

int get_is_running() {
    return is_running;
}

void set_is_multithreaded() {
    is_multithreaded = 1;
}

int scheduler() {
    is_running = 1;
    int errCode = 0;

    if (policy == FCFS || policy == SJF) {
        if (is_multithreaded) {
            pthread_create(&workers[0], NULL, non_preemptive_scheduler_mt, NULL);
            pthread_create(&workers[1], NULL, non_preemptive_scheduler_mt, NULL);

            pthread_join(workers[0], NULL);
            pthread_join(workers[1], NULL);
        }
        else {
            errCode = non_preemptive_scheduler();
        }
    }
    else {
        if (is_multithreaded) {
            pthread_create(&workers[0], NULL, preemptive_scheduler_mt, NULL);
            pthread_create(&workers[1], NULL, preemptive_scheduler_mt, NULL);

            pthread_join(workers[0], NULL);
            pthread_join(workers[1], NULL);
        }
        else {
            errCode = preemptive_scheduler();
        }
    }

    is_running = 0;
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
    else if (policy == RR30) {
        quantum = 30;
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
            policy_reenqueue(pcb);
        }
        else {
            mem_free(pcb);
            PCB_free(pcb);
        }
    }
    return errCode;
}

void *non_preemptive_scheduler_mt(void *args) {
    while (1) {
        pthread_mutex_lock(&queue_mutex);

        while (queue_is_empty() && is_running) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        if (queue_is_empty() && !is_running) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        struct PCB_struct *pcb = queue_dequeue();
        pthread_mutex_unlock(&queue_mutex);
        while (pcb->pc < pcb->start + pcb->length) {
            char *line = mem_get_from(pcb->pc);
            parseInput(line); // which calls interpreter()
            pcb->pc++;
        }
        mem_free(pcb);
        PCB_free(pcb);
    }
    return NULL;
}


void *preemptive_scheduler_mt(void *args) {
    if (policy == RR) {
        quantum = 2;
    }
    else if (policy == RR30) {
        quantum = 30;
    }
    else if (policy == AGING) {
        quantum = 1;
    }
    while (1) {
        pthread_mutex_lock(&queue_mutex);

        while (queue_is_empty() && is_running) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        if (queue_is_empty() && !is_running) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }

        struct PCB_struct *pcb = queue_dequeue();
        pthread_mutex_unlock(&queue_mutex);

        int counter = quantum;
        while (counter > 0 && pcb->pc < pcb->start + pcb->length) {
            char *line = mem_get_from(pcb->pc);
            parseInput(line); // which calls interpreter()
            pcb->pc++;
            counter--;
        }

        pthread_mutex_lock(&queue_mutex);
        if (policy == AGING) {
            queue_aged();
        }
        if (pcb->pc < pcb->start + pcb->length) {
            policy_reenqueue(pcb);
        }
        else {
            mem_free(pcb);
            PCB_free(pcb);
        }
    }
    pthread_mutex_unlock(&queue_mutex);
    return NULL;
}

void scheduler_stop() {
    pthread_mutex_lock(&queue_mutex);
    is_running = 0;
    pthread_cond_broadcast(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);

    if (is_multithreaded) {
        pthread_join(workers[0], NULL);
        pthread_join(workers[1], NULL);
    }

    queue_destroy();
}

void scheduler_enqueue(struct PCB_struct *pcb) {
    pthread_mutex_lock(&queue_mutex);
    policy_enqueue(pcb);
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
}

void scheduler_enqueue_first(struct PCB_struct *pcb) {
    pthread_mutex_lock(&queue_mutex);
    queue_enqueue_first(pcb);
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
}