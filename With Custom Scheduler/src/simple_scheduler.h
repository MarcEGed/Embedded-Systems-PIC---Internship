//simple_scheduler.h

#ifndef SIMPLE_SCHEDULER_H
#define SIMPLE_SCHEDULER_H

typedef void (*task_fn_t)(void);  //function pointer for tasks

#define SCHED_MAX_TASKS 12

typedef struct{
    task_fn_t fn;
    unsigned char active; //1 for active, 0 for inactive
} Task;

typedef struct{
    unsigned char count;
} Semaphore;

typedef struct{
    unsigned char full;             //1 for full, 0 for empty
    unsigned char data[8];          //size can be changed
} Mailbox;

//Scheduler functions
void sched_init(void);
int sched_add_task(task_fn_t fn);
void sched_run_cycle(void);             //run one cycle
void sched_run_forever(unsigned int cycle_delay_ms);

//Semaphore functions
void sem_init(Semaphore *s, unsigned char init);
unsigned char sem_trywait(Semaphore *s);
void sem_signal(Semaphore *s);

//Mailbox functions
void mbox_init(Mailbox *m);
unsigned char mbox_send(Mailbox *m, int8 buf[], int8 size);
unsigned char mbox_receive(Mailbox *m, int8 out[], int8 size);

#endif
