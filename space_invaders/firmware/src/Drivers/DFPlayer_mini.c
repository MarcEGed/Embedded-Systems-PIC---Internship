#include "../main.h"
#include "dfplayer_mini.h"


static int16 df_checksum(int8 cmd, int8 feedback, int8 param1, int8 param2) {
   int16 sum = 0 - (DF_VER + DF_LEN + cmd + feedback + param1 + param2);
   return sum;
}


static void df_send(int8 cmd, int8 param1, int8 param2, int1 feedback) {
   int16 checksum = df_checksum(cmd, feedback, param1, param2);

   putc(DF_START, DFPLAYER);
   putc(DF_VER, DFPLAYER);
   putc(DF_LEN, DFPLAYER);
   putc(cmd, DFPLAYER);
   putc(feedback, DFPLAYER);
   putc(param1, DFPLAYER);
   putc(param2, DFPLAYER);
   putc(HIGH_BYTE(checksum), DFPLAYER);
   putc(LOW_BYTE(checksum), DFPLAYER);
   putc(DF_END, DFPLAYER);
}


void df_init(void) {
   delay_ms(200);
}

void df_setVolume(int8 vol) {
   if(vol > 30) vol = 30;
   df_send(0x06, 0x00, vol, 0);
}

void df_playTrack(int16 track) {
   df_send(0x03, HIGH_BYTE(track), LOW_BYTE(track), 0);
}

void df_pause(void) {
   df_send(0x0E, 0x00, 0x00, 0);
}

void df_resume(void) {
   df_send(0x0D, 0x00, 0x00, 0);
}

void df_stop(void) {
   df_send(0x16, 0x00, 0x00, 0);
}

void df_playFolder(int8 folder, int8 track) {
   df_send(0x0F, folder, track, 0);
}
