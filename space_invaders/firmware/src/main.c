#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tasks.h"
#include "Drivers/sh1106.h"

void main(void)
{
    bool ret;

    delay_ms(250);

    ret = scheduler_init(get_ticks_counter);    /*Initialize scheduler*/

    if (true == ret)
    {
        /*Create tasks:*/
        ret &= init_system();       /*initialize system and create task(s)*/
        ret &= init_hw();           /*initialize HW*/
        ret &= init_adc();          /*initialize ADC*/
        //ret &= init_player_draw_task();
        
        //TODO: list others:

        //TEST CODE _ TO BE REMOVED
        BYTE data;
        i2c_start();
        i2c_write(0xA0);
        i2c_write(0x0000 >> 8);
        i2c_write(0x0000 & 0xFF);
        i2c_start();
        i2c_write(0xA1);
        data = i2c_read(0);
        i2c_stop();
        printf("EEPROM[0x0000] = %u\r\n", data);
        ////////////////////////////


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
