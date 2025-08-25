/*
    display.c - Handles rendering and screen updates for the SH1106 OLED display
    Uses a framebuffer to draw pixels, digits, letters, words, and game entities (player, enemy, bullet)
    Provides functions to clear, update, and draw on the screen efficiently
    Implements movement logic for player, enemies, and bullets
    Includes game start and game over screen rendering
    Depends on sh1106 driver and utility modules for fonts, sprites, vectors, and entities
    Coordinate system: (0,0) top-left; (128,64) bottom-right
*/

#include "../main.h"

#include "display.h"
#include "../Utilities/font.h"
#include "../Utilities/sprites.h"
#include "../Utilities/vec2.h"
#include "../Drivers/sh1106.h"
#include "../Utilities/entities.h"

void display_init(){
    //wrapper for oled driver to initialize
    sh1106_init();
    display_clear();
    display_update();
}

void display_clear(){
    //wrapper for oled driver clear
    sh1106_clear_buffer();
}

void display_update(){
    //wrapper for oled driver update
    sh1106_update_screen();
}

void display_draw_digit(Vec2 pos, int digit){
    //draws digit to the display
    if (digit < 0 || digit > 9) return;

    for (int col = 0; col < 3; col++){
        for (int row = 0; row < 5; row++){
            int pixel = (digit_font_3x5[digit][col] >> row) & 1;
            sh1106_draw_pixel(pos.x + col, pos.y + row, pixel);
        }
    }
}

void display_draw_number(Vec2 pos, int number) {
    //draws complete number with positioning for each digit
    if (number < 0 || number > 100) return;

    if (number == 100) {
        Vec2 pos1, pos2;

        display_draw_digit(pos, 1);

        pos1.x = pos.x + 4;
        pos1.y = pos.y;
        display_draw_digit(pos1, 0);

        pos2.x = pos.x + 8;
        pos2.y = pos.y;
        display_draw_digit(pos2, 0);

        return;
    }

    int tens = number / 10;
    int ones = number % 10;

    if (tens > 0) {
        display_draw_digit(pos, tens);
    } else {
        //clear tens digit area if number < 10
        for (int col = 0; col < 3; col++) {
            for (int row = 0; row < 5; row++) {
                sh1106_draw_pixel(pos.x + col, pos.y + row, 0);
            }
        }
    }

    Vec2 ones_pos;
    ones_pos.x = pos.x + 4;
    ones_pos.y = pos.y;
    display_draw_digit(ones_pos, ones);
}

void display_draw_letter(Vec2 pos, char c){
    //displays letter at pos.x, pos.y
    for (int i = 0; i < LETTER_ARR_SIZE; i++){
        if (letter_index_3x5[i] == c){
            for (int col = 0; col < 3; col++){
                for (int row = 0; row < 5; row++){
                    int pixel = (letter_font_3x5[i][col] >> row) & 1;
                    sh1106_draw_pixel(pos.x + col, pos.y + row, pixel);
                }
            }
            return;
        }
    }
}

void display_draw_word(Vec2 pos, int8 word[], int length){
    //draws word at (pos.x, pos.y)
    for (int i = 0; i < length; i++){
        display_draw_letter(pos, word[i]);
        pos.x += 4; // move right to draw next letter
    }
}

void display_draw_entity(Vec2 pos, int8 *sprite_data, int h, int w){
    //draws entity
    int i, j;
    for (i = 0; i < h; i++){
        for (j = 0; j < w; j++){
            int pixel = (sprite_data[i] >> j) & 1;
            sh1106_draw_pixel(pos.x + j, pos.y + i, pixel);
        }
    }
}

void display_clear_entity(Vec2 pos, int h, int w){
    //clears an entity
    int i, j;
    for (i = 0; i < h; i++){
        for (j = 0; j < w; j++){
            sh1106_draw_pixel(pos.x + j, pos.y + i, 0);
        }
    }
}

void display_draw_player(Vec2 pos){
    //draws player at pos.x and pos.y
    display_draw_entity(pos, PLAYER_SPRITE, 8, 8);
}

void display_draw_enemy(Vec2 pos){
    //draws enemy at pos.x and pos.y
    display_draw_entity(pos, ENEMY_SPRITE, 8, 8);
}

void display_draw_bullet(Vec2 pos){
    //draws bullet at pos.x and pos.y
    display_draw_entity(pos, BULLET_SPRITE, 8, 8);
}

void display_draw_score(int score){
    //draws score at fixed position (pos)
    Vec2 pos = {112, 0};
    display_draw_number(pos, score);
}

void display_clear_score(){
    //clears score from fixed position (pos)
    Vec2 pos = {112, 0};
    display_clear_entity(pos, 11, 5);
}

Vec2 display_move_player(Vec2 pos){
    //handles player movement
    Vec2 last_pos = pos;

    display_clear_entity(last_pos, 8, 8);

    if (pos.x < PLAYER_MIN_X) pos.x = PLAYER_MIN_X;
    if (pos.x > PLAYER_MAX_X) pos.x = PLAYER_MAX_X;

    if (pos.y < PLAYER_TOP_Y) pos.y = PLAYER_TOP_Y;
    if (pos.y > PLAYER_BOT_Y) pos.y = PLAYER_BOT_Y;

    display_draw_player(pos);
    return pos;
}

Enemy display_move_enemy(Enemy enemy){
    //handles enemy movement
    Vec2 last_pos = enemy.pos;

    if (enemy.reached_y_edge != 0){
        enemy.pos.x += ENEMY_STEP_X;
        enemy.vdir = (enemy.reached_y_edge == 1)? 1 : -1;
        enemy.reached_y_edge = 0;

        display_clear_entity(last_pos, 8, 8);
        display_draw_enemy(enemy.pos);
        return enemy;
    }

    enemy.pos.y += enemy.vdir * ENEMY_STEP_Y;

    if (enemy.pos.y <= ENEMY_TOP_Y){
        enemy.pos.y = ENEMY_TOP_Y;
        enemy.reached_y_edge = 1;       //reached top
    }else if (enemy.pos.y >= ENEMY_BOT_Y){
        enemy.pos.y = ENEMY_BOT_Y;
        enemy.reached_y_edge = -1;      //reached bottom
    }

    display_clear_entity(last_pos, 8, 8);
    display_draw_enemy(enemy.pos);

    return enemy;
}


Bullet display_move_bullet(Bullet bullet){
    //handles bullet movement
    if (!bullet.is_active) return bullet;

    Vec2 last_pos = bullet.pos;
    bullet.pos.x -= BULLET_SPEED;

    display_clear_entity(last_pos, 8, 8);
    display_draw_bullet(bullet.pos);
    
    return bullet;
}

void display_gameover_screen(int score, int high_score){
    //draws game over screen
    Vec2 gameover_pos = {46, 10};

    Vec2 hs_label_pos = {10, 20};
    Vec2 hs_pos = {10, 26};

    Vec2 ss_label_pos = {110, 20};
    Vec2 ss_pos = {110, 26};

    Vec2 restart_label_pos = {32, 54};

    int8 gameover_text[] = {'g', 'a', 'm', 'e', ' ', 'o', 'v', 'e', 'r'};
    int8 hs_text[] = {'h', 's'};
    int8 ss_text[] = {'s', 's'};
    int8 click_to_restart_text[] = {
        'c', 'l', 'i', 'c', 'k', ' ',
        't', 'o', ' ',
        'r', 'e', 's', 't', 'a', 'r', 't'
    };
    
    display_draw_word(gameover_pos, gameover_text, 9);
    display_draw_word(hs_label_pos, hs_text, 2);
    display_draw_word(ss_label_pos, ss_text, 2);
    display_draw_word(restart_label_pos, click_to_restart_text, 16);

    display_draw_number(hs_pos, high_score);
    display_draw_number(ss_pos, score);
}

void display_start_screen(){
    //draws start screen
    Vec2 title_pos = {8, 5};
    Vec2 start_pos = {36, 59};

    Vec2 enemy_pos  = {10, 32};
    Vec2 bullet_pos = {50, 32};
    Vec2 player_pos = {90, 32};

    int8 title_text[] = {
        's', 'p', 'a', 'c', 'e', ' ',
        'i', 'n', 'v', 'a', 'd', 'e', 'r', 's',
        ' ', ' ', ' ',
        'p', 'i', 'c', ' ',
        'e', 'd', 'i', 't', 'i', 'o', 'n'
    };

    int8 start_text[] = {
        'c', 'l', 'i', 'c', 'k', ' ',
        't', 'o', ' ',
        's', 't', 'a', 'r', 't'
    };

    display_draw_word(title_pos, title_text, 28);
    display_draw_word(start_pos, start_text, 14);

    display_draw_player(player_pos);
    display_draw_bullet(bullet_pos);
    display_draw_enemy(enemy_pos);
}