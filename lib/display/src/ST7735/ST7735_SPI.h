#pragma once
#include "display/default.h"

#ifdef LCD_SPI
#ifdef _ST7735_ 

#include "comands.h"
#include "rgb/rgb.h"

class ST7735_SPI {
public:
  ST7735_SPI()
  {
    SPCR = _BV(SPE) | _BV(MSTR);
    SPSR = _BV(SPI2X);
    TCCR0B |= _BV(CS00);
    SPDR = 0;
  }

  void send_command(byte data);
  void set_addr(byte x0, byte y0, byte x1, byte y1);
  void send_byte(byte data);
  void send_rgb(byte r, byte g, byte b);
  void send_rgb(uint16_t data); // формат 0x0rgb / RGB_16
  void send_rgb(uint32_t color);
  void send_rgb(RGB color);
  void rect(byte x0, byte y0, byte x1, byte y1, RGB color);
};

#endif
#endif
