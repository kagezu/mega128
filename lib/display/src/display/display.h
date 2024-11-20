#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "display.config.h"
#include "draw/draw.h"

#ifdef LCD_SPI
#include "ST7735S_SPI/ST7735S_SPI.h"
class Display : public ST7735S_SPI, public Draw {
#else
#include "ST7735S/ST7735S.h"
class Display : public ST7735S, public Draw {
#endif

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
  void rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
  void clear(RGB color);
  void symbol(byte *source, byte x, byte y, byte dx, byte dy);
};

#endif
