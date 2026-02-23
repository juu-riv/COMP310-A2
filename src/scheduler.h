#ifndef A2_261120458_SCHEDULER_H
#define A2_261120458_SCHEDULER_H
#define MAX_ARGS 5

#include "pcb.h"

enum policy_enum {
    FCFS,
    SJF,
    RR,
    RR30,
    AGING
};

void set_policy(enum policy_enum scheduling_policy);
enum policy_enum get_policy(void);
int get_is_running(void);
int get_is_working(void);
void set_is_multithreaded(void);
void set_has_to_quit(void);
void scheduler_workers(void);
int scheduler();
int non_preemptive_scheduler();
int preemptive_scheduler();
void *preemptive_scheduler_mt(void *args);
void scheduler_stop(void);
void scheduler_enqueue(struct PCB_struct *pcb);
void scheduler_enqueue_first(struct PCB_struct *pcb);

#endif //A2_261120458_SCHEDULER_H