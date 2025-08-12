/*
    controller.c - API for generic joystick controller
    Implements player movement with deadzones (preventing random movement) and button state
    Provides functions to update player position and shooting action
    Needs joystick driver, entity definitions and vec2 struct
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "controller.h"
#include "../Drivers/joystick.h"
#include "../Utilities/vec2.h"
#include "../Utilities/entities.h"


#define MOVE_SPEED      5

#define X_DEADZONE_MIN 120
#define X_DEADZONE_MAX 135
#define Y_DEADZONE_MIN 120
#define Y_DEADZONE_MAX 135

static int1 button_pressed = 0;
static int1 last_button_state = 0;
static int1 just_pressed = 0;
static int1 just_released = 0;

void controller_init(){
    //wrapper function to initialize the driver
    joystick_init();
}

void controller_update(Vec2 *player_pos){
    //updates player position and button state
    int x = get_Joystick_X();  // 0–255
    int y = get_Joystick_Y();  // 0–255

    // LEFT
    if (x < X_DEADZONE_MIN && player_pos->x > PLAYER_MIN_X)
        player_pos->x -= MOVE_SPEED;
    // RIGHT
    else if (x > X_DEADZONE_MAX && player_pos->x < PLAYER_MAX_X)
        player_pos->x += MOVE_SPEED;

    // DOWN
    if (y < Y_DEADZONE_MIN && player_pos->y < PLAYER_BOT_Y)
        player_pos->y += MOVE_SPEED;
    // UP
    else if (y > Y_DEADZONE_MAX && player_pos->y > PLAYER_TOP_Y)
        player_pos->y -= MOVE_SPEED;

    if (player_pos->x < PLAYER_MIN_X)
        player_pos->x = PLAYER_MIN_X;
    if (player_pos->x > PLAYER_MAX_X)
        player_pos->x = PLAYER_MAX_X;

    if (player_pos->y < PLAYER_TOP_Y)
        player_pos->y = PLAYER_TOP_Y;
    if (player_pos->y > PLAYER_BOT_Y)
        player_pos->y = PLAYER_BOT_Y;

    
    //button handling section
    int1 sw = get_Joystick_SW();
    button_pressed = !sw;               // active-low
    just_pressed = (button_pressed && !last_button_state);
    just_released = (!button_pressed && last_button_state);
    last_button_state = button_pressed;
}

void controller_update_button_only(){
    //updates only the button - used for menu screens
    int1 sw = get_Joystick_SW();
    button_pressed = !sw;               // active-low
    just_pressed = (button_pressed && !last_button_state);
    just_released = (!button_pressed && last_button_state);
    last_button_state = button_pressed;
}

int1 is_button_pressed(){
    return button_pressed;
}

int1 is_button_just_pressed(){
    return just_pressed;
}

int1 is_button_just_released(){
    return just_released;
}

void controller_shoot(Player p, Bullet *b){
    //allow shooting action if bullet is not active
    if (is_button_pressed() && !b->is_active){
            b->is_active = 1;
            b->pos.x = p.pos.x - 6;
            b->pos.y = p.pos.y;
        }
}