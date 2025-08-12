//memory.c
//complete

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
