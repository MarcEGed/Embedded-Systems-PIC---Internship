//joystick.h

#ifndef JOYSTICK_H
#define JOYSTICK_H

#define VRX_PIN_INDEX 0   //RA2
#define VRY_PIN_INDEX 1   //RA1
#define SW_PIN      PIN_B0

void joystick_init();

int get_Joystick_X();
int get_Joystick_Y();
int1 get_Joystick_SW();

#endif