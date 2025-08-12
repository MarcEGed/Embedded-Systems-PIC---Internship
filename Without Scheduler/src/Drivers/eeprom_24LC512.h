//24LC512.h     i2c eeprom driver
#ifndef EEPROM_24LC512
#define EEPROM_24LC512

#define EEPROM_ADDR_WRITE  0xA0
#define EEPROM_ADDR_READ   0xA1

#define STATE_ADDR 0x0000

void eeprom_write_byte(BYTE data);
BYTE eeprom_read_byte();

#endif