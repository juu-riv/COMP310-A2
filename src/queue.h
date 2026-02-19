#ifndef A2_261120458_QUEUE_H
#define A2_261120458_QUEUE_H

struct queue_struct {
    struct PCB_struct *head;
    struct PCB_struct *tail;
};

void queue_init() ;
void queue_enqueue(struct PCB_struct *pcb);
struct PCB_struct *queue_dequeue();
int queue_is_empty();
void queue_destroy();

#endif //A2_261120458_QUEUE_H