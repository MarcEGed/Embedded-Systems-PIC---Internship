//sh1106.c
#include <18F46K22.h>
#fuses INTRC_IO, NOWDT, NOLVP, NOPROTECT
#use delay(clock=16M)
#use i2c(Master, SDA=PIN_C4, SCL=PIN_C3, FAST=100000)

#include "sh1106.h"

BYTE buffer[8][128];

void sh1106_cmd(unsigned char cmd) {
   i2c_start();
   i2c_write(SH1106_ADDR);
   i2c_write(0x00);
   i2c_write(cmd);
   i2c_stop();
}

void sh1106_data(unsigned char data) {
   i2c_start();
   i2c_write(SH1106_ADDR);
   i2c_write(0x40);
   i2c_write(data);
   i2c_stop();
}

void sh1106_init() {
   delay_ms(100);                       //wait for power

   sh1106_cmd(0xAE);                    //display off
   sh1106_cmd(0xD5); sh1106_cmd(0x80);  //set clock
   sh1106_cmd(0xA8); sh1106_cmd(0x3F);  //multiplex 64
   sh1106_cmd(0xD3); sh1106_cmd(0x00);  //no offset
   sh1106_cmd(0x40);                    //start line0

   sh1106_cmd(0xAD); sh1106_cmd(0x8B);  //charge pump
   sh1106_cmd(0xA1);                    //mirror x
   sh1106_cmd(0xC8);                    //mirror y

   sh1106_cmd(0xDA); sh1106_cmd(0x12);  //com config
   sh1106_cmd(0x81); sh1106_cmd(0xCF);  //contrast
   sh1106_cmd(0xD9); sh1106_cmd(0xF1);  //precharge
   sh1106_cmd(0xDB); sh1106_cmd(0x40);  //vcom level

   sh1106_cmd(0xA4);                   //use display ram
   sh1106_cmd(0xA6);                   //normal display
   sh1106_cmd(0xAF);                   //display on
}

void sh1106_update_screen() {
   for (int page = 0; page < 8; page++) {
      sh1106_cmd(0xB0 + page);
      sh1106_cmd(0x02);
      sh1106_cmd(0x10);
      for (int col = 0; col < 128; col++) {
         sh1106_data(buffer[page][col]);
      }
   }
}

void sh1106_draw_pixel(int x, int y, int color) {
   if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

   int page = y / 8;
   BYTE mask = 1 << (y % 8);

   if (color)
      buffer[page][x] |= mask;
   else
      buffer[page][x] &= ~mask;
}

void sh1106_clear_buffer() {
   for (int page = 0; page < 8; page++) {
      for (int col = 0; col < 128; col++) {
         buffer[page][col] = 0x00;
      }
   }
}
