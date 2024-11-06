#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "ST7735S.h"

class LcdDriver : private ST7735S {
public:
  // primitive
  void pixel(byte x, byte y, byte r, byte g, byte b);
  void pixel(byte x, byte y, RGB color);
  void line(byte x0, byte y0, byte x1, byte y1, RGB color);
  void circle(byte x, byte y, byte r, RGB color);

  // в разработке
public:
  inline void clear(uint32_t color) { rect(0, 0, MAX_X - 1, MAX_Y - 1, color); };
  void symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy);

  // Тесты
  void demo(byte d);
};

#endif
