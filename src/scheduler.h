#ifndef A2_261120458_SCHEDULER_H
#define A2_261120458_SCHEDULER_H
#define MAX_ARGS 5

enum policy_enum {
    FCFS,
    SJF,
    RR,
    AGING
};

void set_policy(enum policy_enum scheduling_policy);
enum policy_enum get_policy(void);
int scheduler();
int non_preemptive_scheduler();
int preemptive_scheduler();

#endif //A2_261120458_SCHEDULER_H