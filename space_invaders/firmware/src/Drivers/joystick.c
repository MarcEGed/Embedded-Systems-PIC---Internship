/*
    joystick.c - Driver for generic joystick module (written for PIC18F46K22)
    Reads X/Y axes via ADC channels RA2 for X and RA1 for Y
    Reads SW (switch) state via digital input PIN_B0
    Needs ADC modules and digital I/O ports to function
*/

#include "../main.h"
#include "joystick.h"
#include "../adc.h"

void joystick_init(){
    // No need for ADC setup here - it's handled by the ADC system
    delay_ms(5);
}

uint16_t get_Joystick_X(){
    // Get X axis ADC value (AN1/RA2)
    return get_adc(VRX_PIN_INDEX);
}

uint16_t get_Joystick_Y(){
    // Get Y axis ADC value (AN2/RA1) 
    return get_adc(VRY_PIN_INDEX);
}

int1 get_Joystick_SW(){
    //active low
    return input(SW_PIN);
}
