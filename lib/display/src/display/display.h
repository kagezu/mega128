#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "ST7735S/ST7735S.h"
#include "draw/draw.h"

class Display : private ST7735S, public Draw {
public:
  // Специфические для данного класса
  void pixel(byte x, byte y, byte r, byte g, byte b);

  // в разработке
public:
  void symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy);

  // Скринсейвер
public:
  void demo(byte d);

  // Реализация интерфейса Draw
public:
  void pixel(byte x, byte y, RGB color);
  void wLine(uint8_t x, uint8_t y, uint8_t length);
  void hLine(uint8_t x, uint8_t y, uint8_t length);

public:
  inline void clear(RGB color) { rect(0, 0, MAX_X, MAX_Y, color); };
};

#endif
