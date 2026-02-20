#ifndef A2_261120458_PCB_H
#define A2_261120458_PCB_H


struct PCB_struct {
    int pid;        // unique process ID
    int background; // 1 = true, 0 = false
    int pc;         // current instruction index
    int start;      // start index in memory
    int length;     // number for lines of script
    int priority;   // priority for AGING
    struct PCB_struct *next;
};

int PID_get_value();
struct PCB_struct *PCB_init(int start);
void PCB_free(struct PCB_struct *pcb);

#endif //A2_261120458_PCB_H