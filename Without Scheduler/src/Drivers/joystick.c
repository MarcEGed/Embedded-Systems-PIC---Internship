//joystick.c
#include <18F46K22.h>
#use delay(clock=16M)

#include "joystick.h"

#define VRX_PIN 2   //RA2
#define VRY_PIN 1   //RA1
#define SW_PIN  PIN_B0

void joystick_init(){
    setup_adc_ports(sAN1 | sAN2);
    setup_adc(ADC_CLOCK_INTERNAL);

    port_b_pullups(TRUE);
    input(SW_PIN);
}

int get_Joystick_X(){
    set_adc_channel(VRX_PIN);
    delay_us(10);
    return read_adc();
}

int get_Joystick_Y(){
    set_adc_channel(VRY_PIN);
    delay_us(10);
    return read_adc();
}

int1 get_Joystick_SW(){
    return input(SW_PIN);
}

