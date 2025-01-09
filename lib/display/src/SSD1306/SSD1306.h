#pragma once
#include "display/config.h"
#include "SSD1306_I2C.h"

#ifdef LCD_PRINT
#include "print/print-font.h"
#endif
#ifdef LCD_DRAW
#include "draw/draw.h"
#endif

class SSD1306
  : public SSD1306_I2C
  // #ifdef LCD_PRINT
  //   , public PrintFont
  // #endif
  // #ifdef LCD_DRAW
  //   , public Draw
  // #endif
{
public:
  // SSD1306_I2C();

private:
  RGB _color = RGB(255, 255, 255);
  RGB _background = 0;

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  // void clear() { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, _background); }
  // void clear(RGB color) { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color); }

  // Скринсейвер
  void demo(byte);
  void test(byte);

  // Реализация интерфейса PrintLCD
  void symbol(byte *source, byte x, byte y, byte dx, byte dy);

  // Реализация интерфейса GFX
  void pixel(byte x, byte y) { SSD1306_I2C::pixel(x, y, _color); }
  void rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
};
