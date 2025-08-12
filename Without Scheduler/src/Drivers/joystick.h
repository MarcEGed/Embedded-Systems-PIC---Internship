//joystick.h

#ifndef JOYSTICK_H
#define JOYSTICK_H

unsigned int16 x;   //VRX on AN2
unsigned int16 y;   //VRY on AN1
int1 sw;            //Button on B0

void joystick_init();

int get_Joystick_X();
int get_Joystick_Y();
int1 get_Joystick_SW();

#endif