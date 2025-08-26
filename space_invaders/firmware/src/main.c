#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tasks.h"
#include "Drivers/sh1106.h"
#include "Drivers/eeprom_24LC512.h"
#include "Drivers/joystick.h"

void main(void)
{
    bool ret;

    delay_ms(250);

    ret = scheduler_init(get_ticks_counter);    /*Initialize scheduler*/

    if (true == ret)
    {
        /*Create tasks:*/
        ret = scheduler_init(get_ticks_counter);
        printf("scheduler_init ret = %u\r\n", ret);
         
        ret &= init_system();
        printf("after init_system ret = %u\r\n", ret);
         
        ret &= init_hw();
        printf("after init_hw ret = %u\r\n", ret);
         
        ret &= init_adc();
        printf("after init_adc ret = %u\r\n", ret);
         

        sh1106_init();
        sh1106_draw_pixel(50, 50, 1);
        sh1106_draw_pixel(25, 25, 1);
        sh1106_update_screen();
        
        printf("Reached before EEPROM test\r\n");
        
        //TODO: list others:

        //TEST CODE _ TO BE REMOVED
        BYTE data = eeprom_read_byte();
        printf("EEPROM[0x0000] = %u\r\n", data);

        //delay_ms(100);


        // Initialize joystick
        //joystick_init();
        printf("Joystick initialized\r\n");
        
        //delay_ms(100);
        
        if (true == ret)    /*success?*/
        {
            printf("OK\n\r");
            while (true)
            {
                int x_val = get_Joystick_X();
                int y_val = get_Joystick_Y();
                int1 sw_val = get_Joystick_SW();
                
                
                static uint16_t print_counter = 0;
                if (++print_counter >= 500) {
                    printf("Joystick X: %d, Y: %d, SW: %d\r\n", x_val, y_val, sw_val);
                    print_counter = 0;
                }
                
                scheduler();    /*run scheduler forever*/
            }
        }
        else
        {
            printf("ERROR\n\r");
            while (true);   /*stall*/
        }
    }
    else
    {
        while (true);   /*stall*/
    }
}
