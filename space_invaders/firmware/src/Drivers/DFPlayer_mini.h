#ifndef DFPLAYER_H
#define DFPLAYER_H

#define DF_START  0x7E
#define DF_VER    0xFF
#define DF_LEN    0x06
#define DF_END    0xEF

#define HIGH_BYTE(x)   ((x >> 8) & 0xFF)
#define LOW_BYTE(x)    (x & 0xFF)


void df_init(void);
void df_setVolume(int8 vol);
void df_playTrack(int16 track);
void df_pause(void);
void df_resume(void);
void df_stop(void);
void df_playFolder(int8 folder, int8 track);

#endif
