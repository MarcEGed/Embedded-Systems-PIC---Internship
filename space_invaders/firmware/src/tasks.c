/*
    tasks.c - contains all tasks needed for the MCU 
    Depends on the APIs in the API folder, utilities and scheduler
*/

#include "main.h"
#include "scheduler.h"
#include "API/display.h"
#include "API/controller.h"
#include "API/memory.h"
#include "Utilities/entities.h"
#include "Utilities/vec2.h"
#include "tasks.h"

#define ENEMIES_PER_WAVE 6
#define ENEMY_X_LIMIT    75

#define GAME_START    0
#define GAME_PLAYING  1
#define GAME_OVER     2

s_task_handle_t start_screen_task_handle = NULL;
s_task_handle_t game_init_task_handle = NULL;
s_task_handle_t player_task_handle = NULL;
s_task_handle_t bullet_task_handle = NULL;
s_task_handle_t collision_task_handle = NULL;
s_task_handle_t display_task_handle = NULL;
s_task_handle_t enemies_task_handle = NULL;
s_task_handle_t score_task_handle = NULL;
s_task_handle_t game_over_task_handle = NULL;

static int game_state = GAME_START;

static Enemy enemies[ENEMIES_PER_WAVE];
static Player p = {{80, 50}};
static Bullet b = {{0, 0}, 0};
static int score = 0;
static int last_score = 0;
static int wave_spawned = 0;

static int enemy_move_interval = 6;
static int frame_counter = 0;
static int enemy_move_counter = 0;

static int collision_detected = 0;

static void reset_globals(void) {
    //resets all global variables
    score = 0;
    last_score = 0;
    enemy_move_interval = 2;
    frame_counter = 0;
    enemy_move_counter = 0;
    wave_spawned = 0;
    collision_detected = 0;
    p.pos.x = 80;
    p.pos.y = 50;
    b.is_active = 0;
}

static void spawn_enemy_wave(void) {
    //spawns the array of enemies
    Enemy new_wave[ENEMIES_PER_WAVE] = {
        {{5, 5}, 1, 0, 1},
        {{5, 25}, 1, 0, 1},
        {{5, 45}, 1, 0, 1},
        {{30, 5}, 1, 0, 1},
        {{30, 25}, 1, 0, 1},
        {{30, 45}, 1, 0, 1}
    };

    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        enemies[i] = new_wave[i];
        display_draw_enemy(enemies[i].pos);
    }
}

bool tasks_init(void) {
    bool ret = true;
    
    //initializes APIs and game variables
    display_init();
    controller_init();

    reset_globals();
    display_clear();
    spawn_enemy_wave();
    display_draw_score(score);
    display_update();
    frame_counter = 0;
    game_state = GAME_START;
    
    return ret;
}

void task_start_screen(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    static int initialized = 0;

    if (game_state != GAME_START) {
        initialized = 0;
        return;
    }

    if (!initialized) {
        display_clear();
        initialized = 1;
    }

    display_start_screen();
    display_update();

    controller_update_button_only();
    if (is_button_just_pressed()) {
        game_state = GAME_PLAYING;
    }
}

void task_game_init(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    static int initialized = 0;

    if (game_state != GAME_PLAYING) {
        initialized = 0;
        return;
    }

    if (!initialized) {
        // Initialize game state for new game
        reset_globals();
        display_clear();
        spawn_enemy_wave();
        display_draw_score(score);
        initialized = 1;
    }
}

void task_player(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    display_clear_entity(p.pos, 8, 8);
    controller_update(&p.pos);
    controller_shoot(p, &b);
    p.pos = display_move_player(p.pos);
}

void task_bullet(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    b = display_move_bullet(b);
}

void task_collision(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        if (check_collisionBE(b, enemies[i]) && b.is_active && enemies[i].is_active) {
            collision_detected = 1;
            b.is_active = 0;
            enemies[i].is_active = 0;
            display_clear_entity(enemies[i].pos, 8, 8);
            display_clear_entity(b.pos, 8, 8);
            break;
        }
    }

    if (b.is_active && b.pos.x <= BULLET_MIN_X) {
        b.is_active = 0;
        display_clear_entity(b.pos, 8, 8);
    }
}

void task_enemies(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    int active_count = 0;
    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        if (enemies[i].is_active) active_count++;
    }

    if (active_count == 0 && !wave_spawned) {
        if (enemy_move_interval > 1) enemy_move_interval--;
        spawn_enemy_wave();
        wave_spawned = 1;
    }
    if (wave_spawned && active_count > 0) {
        wave_spawned = 0;
    }

    // Move enemies based on frame counter
    enemy_move_counter++;
    if (enemy_move_counter >= enemy_move_interval) {
        enemy_move_counter = 0;
        for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
            if (enemies[i].is_active) {
                enemies[i] = display_move_enemy(enemies[i]);
                if (enemies[i].pos.x >= ENEMY_X_LIMIT) {
                    game_state = GAME_OVER;
                }
            }
        }
    }
}

void task_score(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    if (collision_detected) {
        score++;
        collision_detected = 0;
    }

    if (score != last_score) {
        display_clear_score();
        display_draw_score(score);
        last_score = score;
    }
}

void task_display(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    if (game_state != GAME_PLAYING) return;

    display_update();
    frame_counter++;
}

void task_game_over(s_task_handle_t me, s_task_msg_t** msg, void* arg) {
    static int hs_saved = 0;

    if (game_state != GAME_OVER) {
        hs_saved = 0;
        return;
    }

    if (!hs_saved) {
        int hs = load_highscore();
        save_highscore(score, hs);
        hs_saved = 1;
    }

    display_clear();
    display_gameover_screen(score, load_highscore());
    display_update();

    controller_update_button_only();
    if (is_button_just_pressed()) {
        game_state = GAME_START;
    }
}