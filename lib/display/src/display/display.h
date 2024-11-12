#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "ST7735S/ST7735S.h"
#include "draw/draw.h"

class Display : private ST7735S, public Draw {
public:
  // Специфические для данного класса
  void scanBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source);
  void scanBitmap(RGB *source);

  // Скринсейвер
public:
  void demo(byte d);

  // Реализация интерфейса Draw
public:
  void pixel(byte x, byte y, RGB color);
  inline void rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
  inline void clear(RGB color) { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color); };
  void symbol(byte *source, byte x, byte y, byte dx, byte dy);
};

#endif
