//joystick.h

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define VRX_PIN_INDEX 1
#define VRY_PIN_INDEX 0
#define SW_PIN      PIN_B0

void joystick_init();
uint16_t get_Joystick_X();
uint16_t get_Joystick_Y();
int1 get_Joystick_SW();

#endif