#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
//#include "tasks.h"
#include "Utilities/vec2.h"
#include "Drivers/eeprom_24LC512.h"
#include "Drivers/joystick.h"
#include "API/display.h"


void main(void)
{
    bool ret;

    delay_ms(250);

    ret = scheduler_init(get_ticks_counter);    /*Initialize scheduler*/

    if (true == ret)
    {   
        ret &= init_system();
        
        ret &= init_hw();
         
        ret &= init_adc();

        
        Vec2 pos = {50, 50};
        display_init();
        display_start_screen();
        display_update();
        /*
        //TODO: list others:

        //TEST CODE _ TO BE REMOVED
        BYTE data = eeprom_read_byte();
        printf("EEPROM[0x0000] = %u\r\n", data);

        //delay_ms(100);


        // Initialize joystick
        //joystick_init();
        printf("Joystick initialized\r\n");
        
        //delay_ms(100);*/
        
        if (true == ret)    /*success?*/
        {
            printf("OK\n\r");
            while (true)
            {
                //int x_val = get_Joystick_X();
                //int y_val = get_Joystick_Y();
                //int1 sw_val = get_Joystick_SW();
                
                
                //static uint16_t print_counter = 0;
                //if (++print_counter >= 500) {
                //    printf("Joystick X: %d, Y: %d, SW: %d\r\n", x_val, y_val, sw_val);
                //    print_counter = 0;
                //}
                
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
