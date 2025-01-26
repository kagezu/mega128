#pragma once
#include <core.h>
#include "display/default.h"
#include "comands.h"
#include "rgb/rgb.h"

class ST7735_SOFT {
protected:
  void send_command(byte data);
  void set_addr(byte x0, byte y0, byte x1, byte y1);
  void send_zero();
  void send_byte(byte data);
  void send_rgb(byte r, byte g, byte b);
  void send_rgb(uint16_t data); // формат 0x0rgb / RGB_16
  void send_rgb(uint32_t color);
  void send_rgb(RGB color);
  void rect(byte x0, byte y0, byte x1, byte y1, RGB color);
};

void ST7735_SOFT::send_command(byte command)
{
  COMMAND_MODE; // Запись команды
  send_byte(command);
  DATA_MODE // Запись данных
};

void ST7735_SOFT::set_addr(byte x0, byte y0, byte x1, byte y1)
{
  send_command(CASET); // Column Address Set
  send_zero();
  send_byte(x0);
  send_zero();
  send_byte(x1);

  send_command(RASET); // Row Address Set
  send_zero();
  send_byte(y0);
  send_zero();
  send_byte(y1);

  send_command(RAMWR); // Memory Write
};

void ST7735_SOFT::send_zero()
{
  LCD_PORT &= ~LCD_SDA;

  byte res = LCD_PORT & ~LCD_SCK;
  byte set = LCD_PORT | LCD_SCK;

  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
};

void ST7735_SOFT::send_byte(byte data)
{
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;
};

#if RGB_FORMAT == RGB_12
void ST7735_SOFT::send_rgb(uint16_t data)
{
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x800 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x400 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x200 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x100 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;
};

#elif RGB_FORMAT == RGB_16
void ST7735_SOFT::send_rgb(uint16_t data)
{
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x8000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1000 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x800 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x400 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x200 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x100 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;
};

#elif RGB_FORMAT == RGB_18
void ST7735_SOFT::send_rgb(uint16_t data) // формат 0x0rgb
{
  send_rgb((data >> 4) & 0xf0, data & 0xf0, data << 4);
}
#endif

void ST7735_SOFT::send_rgb(uint32_t color)
{
  send_rgb(color >> 16, color >> 8, color);
}

void ST7735_SOFT::send_rgb(RGB color)
{
#if RGB_FORMAT == RGB_12 || RGB_FORMAT == RGB_16
  send_rgb((uint16_t)color);
#elif RGB_FORMAT == RGB_18
  send_rgb(color.red, color.green, color.blue);
#endif
}

void ST7735_SOFT::send_rgb(byte r, byte g, byte b)
{
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = b & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = b & 0x8 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = b & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif

  LCD_PORT = g & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = g & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif

  LCD_PORT = r & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = r & 0x8 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = r & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif
};

void ST7735_SOFT::rect(byte x0, byte y0, byte x1, byte y1, RGB color)
{
  byte r = color.red();
  byte g = color.green();
  byte b = color.blue();

  DISPLAY_CONNECT;
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  while (len--) {
    LCD_PORT = b & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = b & 0x8 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = b & 0x4 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif

    LCD_PORT = g & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = g & 0x8 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x4 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif

    LCD_PORT = r & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = r & 0x8 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = r & 0x4 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif
  }
  DISPLAY_DISCONNECT;
};
