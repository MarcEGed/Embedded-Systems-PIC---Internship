#ifndef  HARDWARE_H
#define  HARDWARE_H

#include "stdbool.h"    /*in order to use "bool" type*/

/*LEDs*/
#define LED1PIN             PIN_C0
#define LED1ON()            output_high(LED1PIN)
#define LED1OFF()           output_low(LED1PIN)
#define LED1TOGGLE()        output_toggle(LED1PIN)
#define GET_LED1_STATE()    input(LED1PIN)

///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif
