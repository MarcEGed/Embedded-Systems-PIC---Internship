/*
    memory.c - Highscore saving system using 24LC512 EEPROM
    Provides functions to load and save the game's highscore
    Reads/writes single byte from/to EEPROM at predefined address
    Depends on eeprom_24LC512 driver for I2C communication
    Ensures highscore is only updated if new score is higher
*/

#include <18F46K22.h>
#use delay(clock=16M)

#include "../Drivers/eeprom_24LC512.h"

int load_highscore(){
    int highscore = eeprom_read_byte();
    return highscore;
}

void save_highscore(int score, int hs){
    if (score > hs){
        eeprom_write_byte(score);
    }
}
