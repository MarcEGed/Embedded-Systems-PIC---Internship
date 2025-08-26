/*
    joystick.c - Driver for generic joystick module (written for PIC18F46K22)
    Reads X/Y axes via ADC channels AN1 (RA2) for X and AN2 (RA1) for Y
    Reads SW (switch) state via digital input PIN_B0
    Needs ADC modules and digital I/O ports to function
*/

#include "main.h"
#include "joystick.h"
#include "adc.h"

void joystick_init(){
    // Initialize joystick - pin B0 is already configured as input in hardware.c
    // No additional configuration needed since hardware.c handles the pin setup
    
    // Give a small delay for any settling
    delay_ms(5);
}

int get_Joystick_X(){
    // Get X axis ADC value (AN1/RA2)
    return (int)get_adc(VRX_PIN_INDEX);
}

int get_Joystick_Y(){
    // Get Y axis ADC value (AN2/RA1) 
    return (int)get_adc(VRY_PIN_INDEX);
}

int1 get_Joystick_SW(){
    // Get switch state - read the pin safely
    // Note: Switch is active low (pressed = 0, released = 1)
    // If you need inverted logic, change this to: return !input(SW_PIN);
    return input(SW_PIN);
}

