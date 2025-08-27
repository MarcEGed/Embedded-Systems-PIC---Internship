/*
   sh1106.c - SH1106 OLED driver for PIC18F46K22
   Designed specifically for this project with CCS C compiler
   Uses I2C1 with proper error handling and timing
   Address: 0x3C - write adress 0x78
*/

#include "../main.h"
#include "sh1106.h"

uint8_t buffer[8][128];

void sh1106_cmd(uint8_t cmd) {
   //send a command to the screen
   i2c_start();
   i2c_write(SH1106_ADDR);
   i2c_write(0x00);
   i2c_write(cmd);
   i2c_stop();
}

void sh1106_data(uint8_t data) {
   //send data to the screen
   i2c_start();
   i2c_write(SH1106_ADDR);
   i2c_write(0x40);
   i2c_write(data);
   i2c_stop();
}

void sh1106_init(void) {
   //initializes the screen
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

void sh1106_update_screen(void) {
   //pushes buffer to the screen
   for (uint8_t page = 0; page < 8; page++) {
      sh1106_cmd(0xB0 + page);
      sh1106_cmd(0x02);
      sh1106_cmd(0x10);
      for (uint8_t col = 0; col < 128; col++) {
         sh1106_data(buffer[page][col]);
      }
   }
}

void sh1106_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
   //draws pixel into the buffer
   if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

   uint8_t page = y / 8;
   uint8_t mask = 1 << (y % 8);

   if (color)
      buffer[page][x] |= mask;
   else
      buffer[page][x] &= ~mask;
}

void sh1106_clear_buffer() {
   //clears the buffer
   for (uint8_t page = 0; page < 8; page++) {
      for (uint8_t col = 0; col < 128; col++) {
         buffer[page][col] = 0x00;
      }
   }
}