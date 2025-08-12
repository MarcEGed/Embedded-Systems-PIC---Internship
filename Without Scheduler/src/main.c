#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include "Utilities/vec2.h"
#include "Drivers/joystick.h"
#include "API/display.h"
#include "API/controller.h"
#include "Utilities/entities.h"
#include "API/memory.h"

#define ENEMIES_PER_WAVE     6
#define ENEMY_X_LIMIT        80

// Dummy constants for your example (adjust to your headers)
#define PLAYER_MIN_X 0
#define PLAYER_MAX_X 127
#define PLAYER_TOP_Y 0
#define PLAYER_BOT_Y 63

Enemy enemies[ENEMIES_PER_WAVE];

// Spawns a wave of enemies in their starting positions
void spawn_enemy_wave() {
    Enemy new_wave[ENEMIES_PER_WAVE] = {
        { 'e',  {5,  5}, 1, 0, 1 },  // enemies[0]
        { 'e',  {5, 25}, 1, 0, 1 },  // enemies[1]
        { 'e',  {5, 45}, 1, 0, 1 },  // enemies[2]
        { 'e', {30,  5}, 1, 0, 1 },  // enemies[3]
        { 'e', {30, 25}, 1, 0, 1 },  // enemies[4]
        { 'e', {30, 45}, 1, 0, 1 }   // enemies[5]
    };

    for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
        enemies[i] = new_wave[i];
        display_draw_enemy(enemies[i].pos);
    }
}

// The main gameplay loop — runs until game over
int main_loop() {
    Player p = {'p', {80, 50}};
    Bullet b = {'b', {0, 0}, 0};

    int frame_counter = 0;
    int enemy_move_interval = 2;
    int score = 0;
    int last_score = 0;
    int wave_spawned = 0;

    spawn_enemy_wave();
    display_draw_score(score);

    while (TRUE) {
        display_clear_entity(p.pos, 8, 8);
        controller_update(&p.pos);
        controller_shoot(p, &b);

        if (b.is_active) {
            b = display_move_bullet(b);
        }

        // COLLISION CHECK
        for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
            if (check_collisionBE(b, enemies[i]) && b.is_active && enemies[i].is_active) {
                b.is_active = 0;
                enemies[i].is_active = 0;
                display_clear_entity(enemies[i].pos, 8, 8);
                display_clear_entity(b.pos, 8, 8);
                score++;
                break;
            }
        }

        // WAVE SYSTEM
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

        // ENEMY MOVEMENT
        if (frame_counter % enemy_move_interval == 0) {
            for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
                if (enemies[i].is_active) {
                    enemies[i] = display_move_enemy(enemies[i]);
                    if (enemies[i].pos.x >= ENEMY_X_LIMIT) {
                        printf("Enemy[%u] pos: x=%u, y=%u\r\n", i, enemies[i].pos.x, enemies[i].pos.y);
                        return score;  // Exit main_loop, triggers game over
                    }
                }
            }
        }

        // PLAYER MOVEMENT
        p.pos = display_move_player(p.pos);

        // SCORE UPDATER
        if (score != last_score) {
            display_clear_score();
            display_draw_score(score);
            last_score = score;
        }

        // SCREEN UPDATE
        display_update();
        delay_ms(100);
        frame_counter++;
    }
}

// Displays the game over screen and waits for restart button press
void game_over_loop(int score) {
    int hs = load_highscore();
    save_highscore(score, hs);

    display_clear();
    display_gameover_screen(score, hs);
    display_update();

    while (TRUE) {
        controller_update_button_only();  // update joystick button state
        if (is_button_just_pressed()) {
            display_clear();
            delay_ms(100);
            break;  // exit loop to restart game
        }
        delay_ms(100);
    }
}

void start_screen_loop(){
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

void main() {
    display_init();
    controller_init();

    while (TRUE) {
        start_screen_loop();
        int final_score = main_loop();        // Run the game, exits on game over condition
        game_over_loop(final_score);  // Show game over and wait for button press
        // Restart: loop back and run main_loop() again
    }
}
