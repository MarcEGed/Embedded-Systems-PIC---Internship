#include "tasks.h"
#include "main.h"

static s_task_handle_t player_draw_task_handle;

static Vec2 player_pos = {90, 25};

void task_draw_player(s_task_handle_t task_handle, s_task_msg_t **msg_head, void* arg)
{
    display_draw_player(player_pos);
    
    display_update();
    
}

bool init_player_draw_task(void){

    bool ret = s_task_create(
        true,                           // active
        S_TASK_NORMAL_PRIORITY,        // priority
        100,                           // ticks (100ms period)
        task_draw_player,              // task function
        &player_draw_task_handle,      // task handle
        NULL                           // arguments
    );
    
    return ret;
}
