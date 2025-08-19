//main.c

#include "config.h"
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
        delay_ms(SCHED_TICK);
    }
}
