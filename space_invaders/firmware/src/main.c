#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tasks.h"
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

        ret &= tasks_init();
        
        ret &= create_start_screen_task();
        ret &= create_game_init_task();
        ret &= create_player_task();
        ret &= create_enemies_task();
        ret &= create_bullet_task();
        ret &= create_collision_task();
        ret &= create_display_task();
        ret &= create_score_task();
        ret &= create_game_over_task();
        
        
        if (true == ret)    /*success?*/
        {
            printf("OK\n\r");
            while (true)
            {
                uint16_t x_val = get_Joystick_X();
                uint16_t y_val = get_Joystick_Y();
                //int1 sw_val = get_Joystick_SW();
                
                
                static uint16_t print_counter = 0;
                if (++print_counter >= 500) {
                    printf("Joystick X: %lu, Y: %lu\r\n", x_val, y_val);
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
