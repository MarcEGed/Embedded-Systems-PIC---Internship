/*
    joystick.c - Driver for generic joystick module (written for PIC18F46K22)
    Reads X/Y axes via ADC channels RA2 for X and RA1 for Y
    Reads SW (switch) state via digital input PIN_B0
    Needs ADC modules and digital I/O ports to function
*/

#include "main.h"
#include "joystick.h"
#include "adc.h"


void joystick_init(){
    //initializes all ports needed
    port_b_pullups(TRUE);
    input(SW_PIN);

    init_adc();
}

int get_Joystick_X(){
    //get X adc value 
    return (int)get_adc_live(VRX_PIN_INDEX);
}

int get_Joystick_Y(){
    //get Y adc value
    return (int)get_adc_live(VRY_PIN_INDEX);
}

int1 get_Joystick_SW(){
    //get switch state
    return input(SW_PIN);
}

