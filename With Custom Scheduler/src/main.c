#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include "simple_scheduler.h"
#include "tasks.h"

void main() {
    tasks_init();

    sched_init();
    sched_add_task(task_start_screen);
    sched_add_task(task_game_init);
    sched_add_task(task_enemies); 
    sched_add_task(task_player);
    sched_add_task(task_bullet);
    sched_add_task(task_collision);
    sched_add_task(task_score);
    sched_add_task(task_display);
    sched_add_task(task_game_over);

    while (TRUE) {
        sched_run_cycle();
        delay_ms(40);
    }
}
