#ifndef MAIN_H
#define MAIN_H
#include <18F46K22.h>
#device ADC=8           /*ADC resolution in bits*/
#device ANSI            /*ANSI C*/

/*Chip configuration*/
#FUSES NOWDT            //No Watch Dog Timer
#FUSES PUT              //Power Up Timer
#FUSES NOBROWNOUT       //No brownout reset
#FUSES BORV29           //Brownout reset at 2.85V
#FUSES TIMER3B5         //Timer3 Clock In is on pin B5
#FUSES LVP              //Low Voltage Programming

#use delay(internal=64000000)

#use I2C(master, slow, I2C1)        /*I2C HAL*/
#use RS232(baud=115200, UART1)      /*UART HAL*/

#use fast_io(ALL)

#include "target_port.h"
#endif
