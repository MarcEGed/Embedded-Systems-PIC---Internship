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
        
        ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 100, task_start_screen, &start_screen_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 0, task_game_init, &game_init_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 50, task_player, &player_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 50, task_enemies, &enemies_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 50, task_bullet, &bullet_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 10, task_collision, &collision_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_display, &display_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, task_score, &score_task_handle, NULL);
        ret &= s_task_create(true, S_TASK_LOW_PRIORITY, 500, task_game_over, &game_over_task_handle, NULL);
        
        
        if (true == ret)    /*success?*/
        {
            printf("OK\n\r");
            while (true)
            {
                // uint16_t x_val = get_adc(1);
                // uint16_t y_val = get_adc(0);
                // int1 sw_val = get_Joystick_SW();
                
                
                // static uint16_t print_counter = 0;
                // if (++print_counter >= 500) {
                //     printf("Joystick X: %d, Y: %d, SW: %d\r\n", (int)x_val, (int)y_val, sw_val);
                //     print_counter = 0;
                // }
                
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
