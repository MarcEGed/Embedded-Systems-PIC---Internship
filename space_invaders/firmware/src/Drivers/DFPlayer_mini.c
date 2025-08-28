#includ "../main.h"

#include "DFPlayer_mini.h"

#define BUSY_PIN PIN_B0

int DFPM_get_checksum(int buff[]){
    int sum = 0;

    for (int i = 1; i < 7; i++){
        sum += buff[i];
    }
    
    return -sum;
}

void DFPM_send_cmd(int8 cmd, int16 arg, int1 feedback){
    int8 buff[10];
    int16 checksum;

    buff[0] = 0x7E;                         //start byte
    buff[1] = 0xFF;                         //version
    buff[2] = 0x06;                         //length
    buff[3] = cmd;                          //command
    buff[4] = feedback? 1 : 0;
    buff[5] = (arg >> 8) & 0xFF;            //Arg MSB
    buff[6] = arg & 0xFF;                   //ARG LSB

    checksum = DFPM_get_checksum(buff);

    buff[7] = (checksum >> 8) & 0xFF;       //Checksum MSB
    buff[8] = checksum & 0xFF;              //Checksum LSB
    buff[9] = 0xEF;                         //end byte

    for (int i = 0; i < 10; i++){
        fputc(buff[i], DFP);
    }
}

void DFPM_set_device(int16 device){
    DFPM_send_cmd(DFP_CMD_PLAYSOURCE, device, 0);
}

void DFPM_set_EQ(int16 eq){
    DFPM_send_cmd(DFP_CMD_SETEQ, eq, 0);
}

void DFPM_set_volume(int16 volume){
    DFPM_send_cmd(DFP_CMD_SETVOL, volume, 0);
}

void DFPM_play_track(int16 track_num){
    DFPM_send_cmd(0x03, track_num, 0);
}

void DFPM_stop(){
    DFPM_send_cmd(0x16, 0, 0);
}