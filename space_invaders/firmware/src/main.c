#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tasks.h"


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
