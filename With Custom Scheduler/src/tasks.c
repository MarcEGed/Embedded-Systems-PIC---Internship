#include <18F46K22.h>
#include "API/display.h"
#include "API/controller.h"
#include "API/memory.h"
#include "Utilities/entities.h"
#include "Utilities/vec2.h"
#include "simple_scheduler.h"
#include "tasks.h"

#define ENEMIES_PER_WAVE 6
#define ENEMY_X_LIMIT    75

#define GAME_START    0
#define GAME_PLAYING  1
#define GAME_OVER     2

static int game_state = GAME_PLAYING;

static Enemy enemies[ENEMIES_PER_WAVE];
static Player p = {{80, 50}};
static Bullet b = {{0, 0}, 0};
static int score = 0;
static int last_score = 0;
static int wave_spawned = 0;

static Semaphore enemy_move_sem;
static int enemy_move_interval = 6;
static int frame_counter = 0;
static Mailbox collision_mbox;

static void reset_globals(void) {
    score = 0;
    last_score = 0;
    enemy_move_interval = 2;
    frame_counter = 0;
    wave_spawned = 0;
    p.pos.x = 80;
    p.pos.y = 50;
    b.is_active = 0;
}

static void spawn_enemy_wave(void) {
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

void tasks_init(void) {
    display_init();
    controller_init();
    sem_init(&enemy_move_sem, 0);
    mbox_init(&collision_mbox);

    reset_globals();
    display_clear();
    spawn_enemy_wave();
    display_draw_score(score);
    display_update();
    frame_counter = 0;
    game_state = GAME_PLAYING;
}

void task_start_screen(void) {
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

void task_game_init(void) {
    static int initialized = 0;

    if (game_state != GAME_PLAYING) {
        initialized = 0;
        return;
    }

    if (!initialized) {
        reset_globals();
        display_clear();
        spawn_enemy_wave();
        display_draw_score(score);
        initialized = 1;
    }
}

void task_player(void) {
    if (game_state != GAME_PLAYING) return;

    display_clear_entity(p.pos, 8, 8);
    controller_update(&p.pos);
    controller_shoot(p, &b);
    p.pos = display_move_player(p.pos);
}

void task_bullet(void) {
    if (game_state != GAME_PLAYING) return;

    b = display_move_bullet(b);
}

void task_collision(void) {
    if (game_state != GAME_PLAYING) return;

    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        if (check_collisionBE(b, enemies[i]) && b.is_active && enemies[i].is_active) {
            int hit_flag[1] = {1};
            mbox_send(&collision_mbox, hit_flag, 1);
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

void task_enemies(void) {
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

    if (sem_trywait(&enemy_move_sem)) {
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

void task_score(void) {
    if (game_state != GAME_PLAYING) return;

    int hit_flag[1];
    while (mbox_receive(&collision_mbox, hit_flag, 1)) {
        if (hit_flag[0] == 1) score++;
    }

    if (score != last_score) {
        display_clear_score();
        display_draw_score(score);
        last_score = score;
    }
}

void task_display(void) {
    if (game_state != GAME_PLAYING) return;

    display_update();
    frame_counter++;
    if (frame_counter % enemy_move_interval == 0) {
        sem_signal(&enemy_move_sem);
    }
}

void task_game_over(void) {
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
