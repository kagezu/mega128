#pragma once
#include <core.h>
#include "display/default.h"
#include "const.h"
#include "pin.h"
#include "rgb/rgb.h"

class ST7735_SOFT {
protected:
  void send_command(byte data);
  void set_addr(byte x0, byte y0, byte x1, byte y1);
  void send_zero();
  void send_byte(byte data);
  void send_rgb(RGB color);
  void rect(byte x0, byte y0, byte x1, byte y1, RGB color);
  // void send_rgb(uint16_t data);
};

void ST7735_SOFT::send_command(byte command)
{
  L_RS(RES); // Запись команды
  send_byte(command);
  L_RS(SET); // Запись данных
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
  L_SDA(RES);

  byte res = L_SCK(SFR) & ~L_SCK(MASK);
  byte set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
  L_SCK(SFR) = res;
  L_SCK(SFR) = set;
};

void ST7735_SOFT::send_byte(byte data)
{
  byte b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  byte b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  byte set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = data & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x2 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x1 ? b1 : b0;
  L_SCK(SFR) = set;
};


void ST7735_SOFT::send_rgb(RGB color)
{
  byte r = color.red;
  byte g = color.green;
  byte b = color.blue;

  byte b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  byte b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  byte set = L_SCK(SFR) | L_SCK(MASK);

  L_SCK(SFR) = r & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = r & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = r & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = r & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = g & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = g & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = g & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = b & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = b & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = b & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = b0;
  L_SCK(SFR) = set;
#endif
};

void ST7735_SOFT::rect(byte x0, byte y0, byte x1, byte y1, RGB color)
{
  byte r = color.red;
  byte g = color.green;
  byte b = color.blue;

  L_CS(RES);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  byte b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  byte b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  byte set = L_SCK(SFR) | L_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
  while (len--) {
    L_SCK(SFR) = r & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = r & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = r & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = r & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif

    L_SCK(SFR) = g & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = g & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = g & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif

    L_SCK(SFR) = b & 0x80 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x40 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x20 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b & 0x10 ? b1 : b0;
    L_SCK(SFR) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(SFR) = b & 0x8 ? b1 : b0;
    L_SCK(SFR) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(SFR) = b & 0x4 ? b1 : b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
    L_SCK(SFR) = b0;
    L_SCK(SFR) = set;
  #endif
  }
  L_CS(SET);
};

/*
void ST7735_SOFT::send_rgb(uint16_t data)
{
  byte b0 = L_SCK(SFR) & ~(L_SDA(MASK) | L_SCK(MASK));
  byte b1 = (L_SCK(SFR) | L_SDA(MASK)) & ~L_SCK(MASK);
  byte set = L_SCK(SFR) | L_SCK(MASK);

#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(SFR) = data & 0x8000 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x4000 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x2000 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x1000 ? b1 : b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = data & 0x800 ? b1 : b0;
  L_SCK(SFR) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = data & 0x8000 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x8000 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x8000 ? b1 : b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = data & 0x400 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x200 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x100 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x80 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x40 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x20 ? b1 : b0;
  L_SCK(SFR) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = data & 0x400 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x400 ? b1 : b0;
  L_SCK(SFR) = set;
#endif

  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x8 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x4 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x2 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x1 ? b1 : b0;
  L_SCK(SFR) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
  L_SCK(SFR) = data & 0x10 ? b1 : b0;
  L_SCK(SFR) = set;
#endif
};
*/
