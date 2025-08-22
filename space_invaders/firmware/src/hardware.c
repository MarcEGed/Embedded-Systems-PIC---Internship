#include "main.h"
#include "hardware.h"
#include "scheduler.h"

/*local prototypes:*/
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/**
 * @brief Function to setup all IOs
 * 
 */
void init_io_ports(void)
{
    set_tris_a(0x03);    /*A: all outputs expect AN0 & 1*/
    output_a(0x00);      /*all ZEROs*/

    set_tris_b(0x00);    /*B: All outputs*/
    output_b(0x00);      /*all ZEROs*/

    set_tris_c(0x98);    /*C: all outputs, except RC3 (SCL), RC4 (SDA) and RC7 (UART - Rx)*/
    output_c(0x00);      /*all ZEROs*/

    set_tris_d(0x00);    /*D: all outputs*/
    output_d(0x00);      /*all ZEROs*/

    set_tris_e(0x08);    /*E*/
    output_e(0x00);      /*all ZEROs*/
}
///////////////////////////////////////////////////////////////////////////////
/**
 * 
 * @return 
 */
bool init_hw(void)
{
    init_io_ports();

    //Timer1: used for scheduler
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //for 1ms interrupt
    
    /*Create related task(s)*/
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, heart_beat_led, NULL, NULL);         /*runs every 100ms*/
}


/**
 * 
 * @param me
 * @param msg
 */
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   static uint8_t heart_beat_cnt = 0;

   //Heart Beat
    if (bit_test(heart_beat_cnt, 4))
    {
        heart_beat_cnt = 0;
        LED1OFF();
        restart_wdt();  //reset WDT
    } else if ((!bit_test(heart_beat_cnt, 2)) && (!bit_test(heart_beat_cnt, 3)))
    {
        LED1TOGGLE();
        heart_beat_cnt++;
    } else
    {
        heart_beat_cnt++;
    }
}
