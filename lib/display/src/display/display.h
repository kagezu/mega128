#pragma once

#include "display.config.h"
#include "print/print-font.h"

#ifdef LCD_SPI
#include "ST7735S_SPI/ST7735S_SPI.h"
#else
#include "ST7735S/ST7735S.h"
#endif

class Display
#ifdef LCD_SPI
  : public ST7735S_SPI
#else
  : public ST7735S
#endif
  , public PrintFont
{
protected:
  RGB _color = RGB(255, 255, 255);
  RGB _background = 0;

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, _background); }
  void clear(RGB color) { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color); }
  void symbol(byte *, byte, byte, byte, byte);

  /*
    // Специфические для данного класса
    void scan_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source);
    void scan_bitmap(RGB *source);
  */

  // Скринсейвер
  void demo(byte);
  void test(byte);

  // Реализация интерфейса Draw
  void pixel(byte, byte);
  void rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
};
