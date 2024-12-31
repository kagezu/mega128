#ifndef DRAW_H
#define DRAW_H

#include "display.config.h"
#include "rgb/rgb.h"

class  Draw {
protected:
  uint8_t _fat = 2;
  RGB _color = RGB(255, 255, 255);
  RGB _background = RGB(0, 0, 0);
  uint8_t _transparent = 0;

public:
  void fat(uint8_t f) { _fat = f; }
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void transparent(uint8_t t) { _transparent = t; }

  // Реализуется в данном классе
public:
  // Линии и фигуры
  void w_line(uint8_t x, uint8_t y, uint8_t x1);
  void h_line(uint8_t x, uint8_t y, uint8_t y1);
  void w_line_fat(uint8_t x, uint8_t y, uint8_t x1);
  void h_line_fat(uint8_t x, uint8_t y, uint8_t y1);

  inline void pixel(uint8_t x, uint8_t y) { pixel(x, y, _color); }
  void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void round_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius);
  void rect_line(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  void circle(uint8_t x, uint8_t y, uint8_t  radius);

  // Утолщённые линии и фигуры
  void line_fat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void rect_fat(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  void circle_fat(uint8_t x, uint8_t y, uint8_t  radius);

  // Полные фигуры
  void circle_fill(uint8_t x, uint8_t y, uint8_t  radius);
  void round_rect_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t  radius);

  // Реализуется в дочернем классе
public:
  virtual void pixel(uint8_t x, uint8_t y, RGB color);
  virtual void rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
  virtual void clear(RGB color);
  virtual void symbol(byte *source, byte x, byte y, byte dx, byte dy);
};
#endif
