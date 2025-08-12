#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Utilities/vec2.h"
#include "../Utilities/entities.h"

void controller_init();
void controller_update(Vec2 *pos);
void controller_update_button_only();

int1 is_button_pressed();
int1 is_button_just_pressed();
int1 is_button_just_released();

void controller_shoot(Player p, Bullet *b);

#endif