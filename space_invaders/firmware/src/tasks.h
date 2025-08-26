#ifndef TASKS_H
#define TASKS_H

#include "scheduler.h"
#include "API/display.h"
#include "Drivers/sh1106.h"
#include "Utilities/vec2.h"

void task_draw_pixel(s_task_handle_t task_handle, s_task_msg_t **msg_head, void* arg);

bool init_pixel_draw_task(void);

#endif
