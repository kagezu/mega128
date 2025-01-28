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

  void send_command(uint8_t data);
  void set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void send_byte(uint8_t data);
  void send_rgb(uint8_t r, uint8_t g, uint8_t b);
  void send_rgb(uint16_t data); // формат 0x0rgb / RGB_16
  void send_rgb(uint32_t color);
  void send_rgb(RGB color);
  void rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, RGB color);
};

#endif
#endif
