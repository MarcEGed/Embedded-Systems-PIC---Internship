//display.h

#ifndef DISPLAY_H
#define DISPLAY_H

#include "../Utilities/vec2.h"
#include "../Utilities/entities.h"

void display_init();
void display_clear();
void display_update();

void display_gameover_screen(int score, int high_score);
void display_start_screen();

void display_draw_digit(Vec2 pos, int digit);
void display_draw_number(Vec2 pos, int number);

void display_draw_letter(Vec2 pos, char c);
void display_draw_word(Vec2 pos, int8 word[], int length);

void display_draw_player(Vec2 pos);
void display_draw_enemy(Vec2 pos);
void display_draw_bullet(Vec2 pos);
void display_draw_score(int score);

void display_clear_score();

void display_clear_entity(Vec2 pos, int h, int w);
void display_draw_entity(Vec2 pos, int8 sprite[], int h, int w);

Vec2 display_move_player(Vec2 pos);
Enemy display_move_enemy(Enemy enemy);
Bullet display_move_bullet(Bullet bullet);

#endif
