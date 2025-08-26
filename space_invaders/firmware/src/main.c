#include "main.h"
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tasks.h"
#include "API/display.h"
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

        printf("=== SH1106 DEBUG TEST ===\r\n");
        
        // Test 1: Basic initialization
        printf("1. Calling sh1106_init()...\r\n");
        sh1106_init();
        printf("   sh1106_init() completed\r\n");
        
        // Test 2: Clear buffer and draw clean content
        printf("2. Clearing buffer and drawing clean content...\r\n");
        sh1106_clear_buffer();
        printf("   Buffer cleared successfully\r\n");
        
        // Test 3: Draw a simple test pattern
        printf("3. Drawing test pattern...\r\n");
        // Draw a cross in the center
        for(int i = 0; i < 10; i++) {
            sh1106_draw_pixel(64 + i, 32, 1);  // Horizontal line
            sh1106_draw_pixel(64 - i, 32, 1);  // Horizontal line
            sh1106_draw_pixel(64, 32 + i, 1);  // Vertical line
            sh1106_draw_pixel(64, 32 - i, 1);  // Vertical line
        }
        printf("   Test pattern drawn\r\n");
        
        // Test 4: Update screen
        printf("4. Updating screen...\r\n");
        sh1106_update_screen();
        printf("   Screen update completed\r\n");
        
        printf("=== SH1106 TEST COMPLETE ===\r\n");
        delay_ms(2000);  // Wait to see if anything appears
        
        // Now test the display API
        printf("Testing display API...\r\n");
        Vec2 pos = {50, 50};
        display_init();
        display_clear();
        display_draw_enemy(pos);
        display_update();

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
