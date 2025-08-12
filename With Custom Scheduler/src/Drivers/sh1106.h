//sh1106.h
#ifndef SH1106_H
#define SH1106_H

#define SH1106_ADDR 0x78
#define WIDTH 128
#define HEIGHT 64

extern BYTE buffer[8][128];

void sh1106_cmd(unsigned char cmd);
void sh1106_data(unsigned char data);
void sh1106_init();
void sh1106_update_screen();
void sh1106_draw_pixel(int x, int y, int color);
void sh1106_clear_buffer();

#endif
