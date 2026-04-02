//sh1106.h
#ifndef SH1106_H
#define SH1106_H

#define SH1106_ADDR 0x78
#define WIDTH 128
#define HEIGHT 64

void sh1106_cmd(uint8_t cmd);
void sh1106_data(uint8_t data);
void sh1106_init(void);
void sh1106_update_screen(void);
void sh1106_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void sh1106_clear_buffer();

#endif