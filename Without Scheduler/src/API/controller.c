/*
    0.0: Joystick X/Y completely functional. Clean up the code a bit then make sure all button-related functions work
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "controller.h"
#include "../Drivers/joystick.h"
#include "../Utilities/vec2.h"
#include "../Utilities/entities.h"

#define VER_BORDER 56
#define HOR_BORDER 120

#define MOVE_SPEED      5
#define ADC_MAX_VALUE   255

#define X_DEADZONE_MIN 120
#define X_DEADZONE_MAX 135
#define Y_DEADZONE_MIN 120
#define Y_DEADZONE_MAX 135

static int1 button_pressed = 0;
static int1 last_button_state = 0;
static int1 just_pressed = 0;
static int1 just_released = 0;

void controller_init(){
    joystick_init();
}

void controller_update(Vec2 *player_pos){
    int x = get_Joystick_X();  // 0–255
    int y = get_Joystick_Y();

    // LEFT
    if (x < X_DEADZONE_MIN && player_pos->x > 0)
        player_pos->x -= MOVE_SPEED;
    // RIGHT
    else if (x > X_DEADZONE_MAX && player_pos->x < HOR_BORDER)
        player_pos->x += MOVE_SPEED;

    // DOWN
    if (y < Y_DEADZONE_MIN && player_pos->y < VER_BORDER)
        player_pos->y += MOVE_SPEED;
    // UP
    else if (y > Y_DEADZONE_MAX && player_pos->y > 0)
        player_pos->y -= MOVE_SPEED;

    if (player_pos->x < 0)
        player_pos->x = 0;
    if (player_pos->x > HOR_BORDER)
        player_pos->x = HOR_BORDER;

    if (player_pos->y < 0)
        player_pos->y = 0;
    if (player_pos->y > VER_BORDER)
        player_pos->y = VER_BORDER;

    
    //button handling
    int1 sw = get_Joystick_SW();
    button_pressed = !sw;  // active-low
    just_pressed = (button_pressed && !last_button_state);
    just_released = (!button_pressed && last_button_state);
    last_button_state = button_pressed;
}

void controller_update_button_only(){
    int1 sw = get_Joystick_SW();
    button_pressed = !sw;  // active-low
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
    if (is_button_pressed() && !b->is_active){
            b->is_active = 1;
            b->pos.x = p.pos.x + 3;
            b->pos.y = p.pos.y;
        }
}