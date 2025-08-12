//24LC512.c     eeprom driver
#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16M)
#use i2c(Master, SDA=PIN_C4, SCL=PIN_C3, FAST=100000)

#include "eeprom_24LC512.h"

void eeprom_write_byte(BYTE data) {
    //Writes byte to STATE_ADDR
    i2c_start();
    i2c_write(EEPROM_ADDR_WRITE);
    i2c_write(STATE_ADDR >> 8);     //upper bits
    i2c_write(STATE_ADDR & 0xFF);   //lower bits
    i2c_write(data);
    i2c_stop();
    delay_ms(5);
}

BYTE eeprom_read_byte() {
    //Reads byte from STATE_ADDR
    BYTE data;
    i2c_start();
    i2c_write(EEPROM_ADDR_WRITE);
    i2c_write(STATE_ADDR >> 8);     //upper bits
    i2c_write(STATE_ADDR & 0xFF);   //lower bits
    i2c_start();
    i2c_write(EEPROM_ADDR_READ);
    data = i2c_read(0);
    i2c_stop();
    return data;
}