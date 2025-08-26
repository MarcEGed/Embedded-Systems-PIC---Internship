#include "main.h"
#include "tasks.h"

static s_task_handle_t pixel_draw_task_handle;

static Vec2 player_pos = {90, 25};

void task_draw_pixel(s_task_handle_t task_handle, s_task_msg_t **msg_head, void* arg){
    sh1106_init();
    sh1106_draw_pixel(50, 50, 1);
    sh1106_draw_pixel(25, 25, 1);
    sh1106_update_screen();
}

bool init_pixel_draw_task(void){
    bool ret = s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_draw_pixel, &pixel_draw_task_handle, NULL);
    return ret;
}
