//main.c

#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include "API/display.h"
#include "API/controller.h"
#include "API/memory.h"
#include "Utilities/entities.h"
#include "Utilities/vec2.h"

#include "simple_scheduler.h"

#define ENEMIES_PER_WAVE     6          //size of enemy wave
#define ENEMY_X_LIMIT        75         // x limit for enemies that leads to game over

Enemy enemies[ENEMIES_PER_WAVE];
Player p = {{80, 50}};
Bullet b = {{0, 0}, 0};
int score = 0;
int last_score = 0;
int wave_spawned = 0;
int game_over_flag = 0;

Semaphore enemy_move_sem;
int enemy_move_interval = 6;
int frame_counter = 0;

Mailbox collision_mbox;

//spawns a wave of enemies with the positions set
void spawn_enemy_wave() {
    Enemy new_wave[ENEMIES_PER_WAVE] = {
        {{5,  5}, 1, 0, 1 },
        {{5, 25}, 1, 0, 1 },
        {{5, 45}, 1, 0, 1 },
        {{30,  5}, 1, 0, 1 },
        {{30, 25}, 1, 0, 1 },
        {{30, 45}, 1, 0, 1 }   
    };

    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        enemies[i] = new_wave[i];
        display_draw_enemy(enemies[i].pos);
    }
}

void task_player(void){
    //task for all player controls
    display_clear_entity(p.pos, 8, 8);
    controller_update(&p.pos);
    controller_shoot(p, &b);
    p.pos = display_move_player(p.pos);
}

void task_bullet(void){
    //task for bullet movement
    b = display_move_bullet(b);
}

void task_collision(void){
    //task for checking collisions between enemies and bullets
    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        if (check_collisionBE(b, enemies[i]) && b.is_active && enemies[i].is_active) {
            int8 hit_flag[1] = {1};

            mbox_send(&collision_mbox, hit_flag, 1);

            b.is_active = 0;
            enemies[i].is_active = 0;
            display_clear_entity(enemies[i].pos, 8, 8);
            display_clear_entity(b.pos, 8, 8);
            break;
        }
    }

    //makes sure the bullet becomes inactive once it reaches left edge of screen
    if (b.is_active && b.pos.x <= BULLET_MIN_X){
        b.is_active = 0;
        display_clear_entity(b.pos, 8, 8);
    }
}

void task_display(void) {
    //task to update display and move enemies if needed
    display_update();
    frame_counter++;

    if (frame_counter % enemy_move_interval == 0){
        sem_signal(&enemy_move_sem);
    }
}

void task_enemies(void) {
    //task to handle all enemy spawning logic and movement
    int active_count = 0;
    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        if (enemies[i].is_active) {
            active_count++;
        }
    }
    if (active_count == 0 && !wave_spawned) {
        if (enemy_move_interval > 1) {
            enemy_move_interval--;
        }
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
                    game_over_flag = 1; //signal to stop scheduler
                }
            }
        }
    }
}

void task_score(void) {
    //task to handle score updates and drawing
    int8 hit_flag[1];

    while (mbox_receive(&collision_mbox, hit_flag, 1)){
        if (hit_flag[0] == 1) score++;
    }

    if (score != last_score) {
        display_clear_score();
        display_draw_score(score);
        last_score = score;
    }
}

void start_screen_loop() {
    //creates start screen and waits for user input to proceed
    display_clear();
    display_start_screen();
    display_update();

    while (TRUE) {
        controller_update_button_only();
        if (is_button_just_pressed()) {
            display_clear();
            delay_ms(100);
            break;
        }
        delay_ms(100);
    }
}

void game_over_loop() {
    //creates game over screen and updates highscore if needed
    int hs = load_highscore();
    save_highscore(score, hs);

    display_clear();
    display_gameover_screen(score, hs);
    display_update();

    while (TRUE) {
        controller_update_button_only();
        if (is_button_just_pressed()) {
            display_clear();
            delay_ms(100);
            break;
        }
        delay_ms(100);
    }
}

void reset_globals(void){
    //resets all global variables to be able to restart the game
    score = 0;
    last_score = 0;
    enemy_move_interval = 2;
    frame_counter = 0;
    wave_spawned = 0;
    game_over_flag = 0;
    p.pos.x = 80; 
    p.pos.y = 50;
    b.is_active = 0;
}

void main() {
    display_init();
    controller_init();
    sem_init(&enemy_move_sem, 0);
    mbox_init(&collision_mbox);

    while (TRUE) {
        start_screen_loop();
        
        reset_globals();
        display_clear();
        spawn_enemy_wave();
        display_draw_score(score);

        //setup scheduler tasks
        sched_init();
        sched_add_task(task_bullet);
        sched_add_task(task_collision);
        sched_add_task(task_player);
        sched_add_task(task_enemies);
        sched_add_task(task_score);
        sched_add_task(task_display);

        //keep running scheduler until player loses
        while (!game_over_flag) {
            sched_run_cycle();
            delay_ms(100);
        }

        game_over_loop();
    }
}
