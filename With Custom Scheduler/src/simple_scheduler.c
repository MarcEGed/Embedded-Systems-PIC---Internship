#include <18F46K22.h>
#use delay(clock=16M)
#include "simple_scheduler.h"

static Task task_list[SCHED_MAX_TASKS];

void sched_init(void){
    //initializes the scheduler
    unsigned char i;

    for (i = 0; i < SCHED_MAX_TASKS; i++){
        task_list[i].fn = 0;
        task_list[i].active = 0;
    }
}

int sched_add_task(task_fn_t fn){
    //adds task to scheduler list
    unsigned char i;
    for (i = 0; i < SCHED_MAX_TASKS; i++){
        if (task_list[i].active == 0){
            task_list[i].fn = fn;
            task_list[i].active = 1;
            return i;
        }
    }
    return -1; //no space in the list
}

void sched_run_cycle(void){
    //runs a single scheduler cycle
    unsigned char i;
    for (i = 0; i < SCHED_MAX_TASKS; i++){
        if (task_list[i].active && task_list[i].fn != 0){
            task_list[i].fn();
        }
    }
}

void sched_run_forever(unsigned int cycle_delay_ms){
    //runs the scheduler indefinitely
    while (1){
        sched_run_cycle();
        delay_ms(cycle_delay_ms);
    }
}

void sem_init(Semaphore *s, unsigned char init){
    //initializes semaphore
    if (s != 0){
        s->count = init;
    }
}

unsigned char sem_trywait(Semaphore *s){    
    //checks if available but doesn't block
    if (s == 0) return 0;
    if (s->count > 0){
        s->count--;
        return 1;
    }
    return 0;
}

void sem_signal(Semaphore *s){
    //sends a signal to semaphore s 
    if (s != 0){
        s->count++;
    }
}

void mbox_init(Mailbox *m){
    //initializes mailbox
    unsigned char i;
    if (m != 0){
        m->full = 0;
        for (i = 0; i < sizeof(m->data); i++){
            m->data[i] = 0;
        }
    }
}

unsigned char mbox_send(Mailbox *m, int8 buf[], int8 size) {
    //sends data to mailbox
    if (m->full || size > sizeof(m->data)) {
        return 0; // fail
    }
    for (unsigned char i = 0; i < size; i++) {
        m->data[i] = buf[i];
    }
    m->full = 1;
    return 1; // success
}

unsigned char mbox_receive(Mailbox *m, int8 out[], int8 size) {
    //receives data from mailbox
    if (!m->full || size > sizeof(m->data)) {
        return 0; // fail
    }
    for (unsigned char i = 0; i < size; i++) {
        out[i] = m->data[i];
    }
    m->full = 0;
    return 1; // success
}
