#include <18F46K22.h>
#device ADC=8           /* 8-bit ADC */
#device ANSI            /* ANSI C */

#FUSES NOWDT            // No Watch Dog Timer
#FUSES PUT              // Power Up Timer
#FUSES NOBROWNOUT       // No brownout reset
#FUSES BORV29           // Brownout reset at 2.85V
#FUSES TIMER3B5         // Timer3 Clock In is on pin B5
#FUSES LVP              // Low Voltage Programming

#use delay(internal=64000000)             // 64 MHz internal oscillator
#use RS232(baud=115200, UART1)            // UART1 for terminal
#use fast_io(ALL)                         // fast I/O

typedef unsigned char uint8_t;

void main(void)
{
    uint8_t adc_x, adc_y;

    // Configure analog pins AN1 and AN2
    setup_adc_ports(sAN1 | sAN2, VSS_VDD);
    setup_adc(ADC_CLOCK_DIV_64);   // ADC clock
    set_adc_channel(1);            // start with AN1

    printf("Starting ADC test...\r\n");

    while(TRUE)
    {
        // Read AN1 (joystick X)
        set_adc_channel(1);
        delay_us(20);               // allow acquisition time
        adc_x = read_adc();

        // Read AN2 (joystick Y)
        set_adc_channel(2);
        delay_us(20);
        adc_y = read_adc();

        // Print raw 0-255 values to terminal
        printf("X=%3u  Y=%3u\r\n", adc_x, adc_y);

        delay_ms(100);              // sample every 100 ms
    }
}

