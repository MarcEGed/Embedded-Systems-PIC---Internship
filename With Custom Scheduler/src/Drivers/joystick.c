/*
    joystick.c - Driver for generic joystick module (written for PIC18F46K22)
    Reads X/Y axes via ADC channels RA2 for X and RA1 for Y
    Reads SW (switch) state via digital input PIN_B0
    Needs ADC modules and digital I/O ports to function
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "joystick.h"

#define VRX_PIN 2   //RA2
#define VRY_PIN 1   //RA1
#define SW_PIN  PIN_B0

void joystick_init(){
    //initializes all ports needed
    setup_adc_ports(sAN1 | sAN2);
    setup_adc(ADC_CLOCK_INTERNAL);

    port_b_pullups(TRUE);
    input(SW_PIN);
}

int get_Joystick_X(){
    //get X adc value 
    set_adc_channel(VRX_PIN);
    delay_us(10);
    return read_adc();
}

int get_Joystick_Y(){
    //get Y adc value
    set_adc_channel(VRY_PIN);
    delay_us(10);
    return read_adc();
}

int1 get_Joystick_SW(){
    //get switch state
    return input(SW_PIN);
}

