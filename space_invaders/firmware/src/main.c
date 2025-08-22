#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"

void main(void)
{
    bool ret;

    ret = scheduler_init(get_ticks_counter);    /*Initialize scheduler*/

    if (true == ret)
    {
        /*Create tasks:*/
        ret &= init_system();       /*initialize system and create task(s)*/
        ret &= init_hw();           /*initialize HW*/
        ret &= init_adc();          /*initialize ADC*/
        
        //TODO: list others:


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
