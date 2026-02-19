#ifndef A2_261120458_SCHEDULER_H
#define A2_261120458_SCHEDULER_H
#define MAX_ARGS 5

enum policy_enum {
    FCFS
};

void set_policy(enum policy_enum scheduling_policy);
int scheduler();
int FCFS_scheduler();

int scheduler_to_interpreter(char *line);

#endif //A2_261120458_SCHEDULER_H