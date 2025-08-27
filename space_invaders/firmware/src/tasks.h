#ifndef TASKS_H
#define TASKS_H

#include "scheduler.h"

void task_start_screen(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_game_init(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_player(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_bullet(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_collision(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_display(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_enemies(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_score(s_task_handle_t me, s_task_msg_t** msg, void* arg);
void task_game_over(s_task_handle_t me, s_task_msg_t** msg, void* arg);

bool tasks_init(void);

bool create_start_screen_task(void);
bool create_game_init_task(void);
bool create_player_task(void);
bool create_enemies_task(void);
bool create_bullet_task(void);
bool create_collision_task(void);
bool create_display_task(void);
bool create_score_task(void);
bool create_game_over_task(void);

extern s_task_handle_t start_screen_task_handle;
extern s_task_handle_t game_init_task_handle;
extern s_task_handle_t player_task_handle;
extern s_task_handle_t bullet_task_handle;
extern s_task_handle_t collision_task_handle;
extern s_task_handle_t display_task_handle;
extern s_task_handle_t enemies_task_handle;
extern s_task_handle_t score_task_handle;
extern s_task_handle_t game_over_task_handle;

#endif