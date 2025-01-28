#pragma once
#include "display/default.h"
#include "const.h"
#include "rgb/rgb.h"
#include "print/print-format.h"
// #include "draw/draw.h"

class ST7735 : public PrintF {

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { rect(0, 0, MAX_X, MAX_Y, _background); }
  void clear(RGB color) { rect(0, 0, MAX_X, MAX_Y, color); }
  void rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1) { rect(x, y, x1, y1, _color); }

  void pixel(uint8_t, uint8_t);
  void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t);

  // Скринсейвер
  void demo(uint8_t);
  void test(uint8_t);

protected:
  void send_config(const uint8_t *, uint8_t);

  virtual void select() = 0;
  virtual void deselect() = 0;
  virtual void send_command(uint8_t data) = 0;
  virtual void set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) = 0;
  virtual void send_zero() = 0;
  virtual void send_byte(uint8_t data) = 0;
  virtual void send_rgb(RGB color) = 0;
  virtual void rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, RGB color) = 0;

private:
  RGB _color = 0x00ffffff;
  RGB _background = 0;

};
