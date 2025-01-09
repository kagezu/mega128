#include <avr/pgmspace.h>
#include "ST7735_SPI.h"

#ifdef LCD_SPI
#ifdef _ST7735_ 

#define SPI_WAIT  while (!(SPSR & _BV(SPIF)));

void ST7735_SPI::send_command(byte command)
{
  SPI_WAIT;
  COMMAND_MODE; // Запись команды
  SPDR = command;
  asm volatile("nop");
  SPI_WAIT;
  DATA_MODE // Запись данных
};

void ST7735_SPI::set_addr(byte x0, byte y0, byte x1, byte y1)
{
  send_command(CASET); // Column Address Set
  SPDR = 0;
  send_byte(x0);
  SPI_WAIT;
  SPDR = 0;
  send_byte(x1);

  send_command(RASET); // Row Address Set
  SPDR = 0;
  send_byte(y0);
  SPI_WAIT;
  SPDR = 0;
  send_byte(y1);

  send_command(RAMWR); // Memory Write
};

void ST7735_SPI::send_byte(byte data)
{
  SPI_WAIT;
  SPDR = data;
};

#if RGB_FORMAT == RGB_12
void ST7735_SPI::send_rgb(uint16_t color)
{
  static byte half, flag = 0;
  byte data;

  if (flag) {
    data = half | (color >> 8);
    SPI_WAIT;
    SPDR = data;
    flag = 0;
    SPI_WAIT;
    SPDR = color;
  }
  else {
    data = color >> 4;
    SPI_WAIT;
    SPDR = data;
    half = color << 4;
    flag = 1;
  }
};

#elif RGB_FORMAT == RGB_16
void ST7735_SPI::send_rgb(uint16_t data)
{
  SPI_WAIT;
  SPDR = data >> 8;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = data;
};

#elif RGB_FORMAT == RGB_18
void ST7735_SPI::send_rgb(uint16_t data) // формат 0x0rgb
{
  send_rgb((data >> 4) & 0xf0, data & 0xf0, data << 4);
}
#endif

void ST7735_SPI::send_rgb(uint32_t color)
{
  send_rgb(color >> 16, color >> 8, color);
}

void ST7735_SPI::send_rgb(RGB color)
{
#if RGB_FORMAT == RGB_12 || RGB_FORMAT == RGB_16
  send_rgb((uint16_t)color);
#elif RGB_FORMAT == RGB_18
  send_rgb(color.red, color.green, color.blue);
#endif
}

void ST7735_SPI::send_rgb(byte r, byte g, byte b)
{

#if RGB_FORMAT == RGB_12

  static byte half, flag = 0;
  byte data;
  if (flag) {
    data = half | (b >> 4);
    SPI_WAIT;
    SPDR = data;
    flag = 0;
    data = (g & 0xf0) | (r >> 4);
    SPI_WAIT;
    SPDR = data;
  }
  else {
    data = (b & 0xf0) | (g >> 4);
    SPI_WAIT;
    SPDR = data;
    half = r & 0xf0;
    flag = 1;
  }

#elif RGB_FORMAT == RGB_16

  byte data = (g >> 5) | (b & 0xf8);
  SPI_WAIT;
  SPDR = data;

  data = ((g & 0x1c) << 3) | (r >> 3);
  SPI_WAIT;
  SPDR = data;

#elif RGB_FORMAT == RGB_18

  SPI_WAIT;
  SPDR = b;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = g;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = r;

#endif


};

void ST7735_SPI::rect(byte x0, byte y0, byte x1, byte y1, RGB color)
{
  DISPLAY_CONNECT;

  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
  set_addr(x0, y0, x1, y1);

#if RGB_FORMAT == RGB_12

  byte hByte = color >> 4;
  byte mByte = (color << 4) | ((color & 0xf00) >> 8);
  byte lByte = color;
  len >>= 1;

#elif RGB_FORMAT == RGB_16

  byte hByte = (uint16_t)color >> 8;
  byte lByte = (uint16_t)color;

#endif

  while (len--) {

  #if RGB_FORMAT == RGB_12

    SPI_WAIT;
    SPDR = hByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = mByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = lByte;

  #elif RGB_FORMAT == RGB_16

    SPI_WAIT;
    SPDR = hByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = lByte;

  #elif RGB_FORMAT == RGB_18

    SPI_WAIT;
    SPDR = color.blue;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.green;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.red;

  #endif

  }

  DISPLAY_DISCONNECT
};

#endif
#endif
