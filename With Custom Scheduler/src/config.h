#ifndef CONFIG_H
#define CONFIG_H

#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#define SCHED_TICK 40

#endif