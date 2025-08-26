#ifndef TASKS_H
#define TASKS_H

#include "scheduler.h"
#include "API/display.h"
#include "Utilities/vec2.h"

void task_draw_player(s_task_handle_t task_handle, s_task_msg_t **msg_head, void* arg);

bool init_player_draw_task(void);

#endif
