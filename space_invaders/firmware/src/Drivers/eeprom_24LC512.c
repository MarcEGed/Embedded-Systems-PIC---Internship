/*
    eeprom_24LC512.c - Driver for 24LC512 I2C EEPROM (written for PIC18F46K22)
    Implements sinngle byte read/write to a fixed address (STATE_ADDR)
    Uses pin C4 (for SDA) and pin C3 (for SDL)
    Needs CCS I2C library
*/

#include "../main.h"

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