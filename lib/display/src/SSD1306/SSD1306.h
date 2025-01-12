#pragma once
#include "display/config.h"
#ifdef _SSD1306_

#include "SSD1306_I2C.h"

#ifdef LCD_PRINT
#include "print/print-font.h"
#endif
#ifdef LCD_DRAW
#include "draw/draw.h"
#endif

class SSD1306
  : public SSD1306_I2C
#ifdef LCD_PRINT
  , public PrintFont
#endif
#ifdef LCD_DRAW
  , public Draw
#endif
{
private:
  RGB _color = 1;
  RGB _background = 0;

public:
  void init();
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { memset(buffer, _background, (LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)); }
  void clear(RGB color) { memset(buffer, color, (LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)); }

  // Скринсейвер
  void demo(byte);
  void test(byte);

  // Реализация интерфейса PrintLCD
  void symbol(byte *source, byte x, byte y, byte dx, byte dy);

  // Реализация интерфейса GFX
  void pixel(byte, byte);
  void rect_fill(byte x, byte y, byte x1, byte y1);
};

#endif
