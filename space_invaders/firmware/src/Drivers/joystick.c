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
    //replaced by init_adc()
    delay_ms(5);
}

uint16_t get_Joystick_X(){
	return get_adc_live(VRX_PIN_INDEX);
}

uint16_t get_Joystick_Y(){
	return get_adc_live(VRY_PIN_INDEX);
}

int1 get_Joystick_SW(){
    //active low
    return input(SW_PIN);
}
