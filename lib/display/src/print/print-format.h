#pragma once

#include <avr.h>
#include "display/config.h"
#include "print-format.h"
#include "font/font.h"

#define FONT_TAB_FACTOR     3

class PrintF {
public:
  PrintF() {};

  void printf(const  __FlashStringHelper *, ...);
  void print(const __FlashStringHelper *);
  void print(const char *);
  void print(uint16_t);
  void print(uint32_t);
  void print(int16_t);
  void print(int32_t);
  void print_h(uint64_t);
  void print_h(uint32_t);
  void print_h(uint16_t);
  void print_h(byte);
  void print(char);
  void letter(byte);
  virtual void symbol(byte *, uint16_t, uint16_t, byte, byte) = 0;

  void font(const Font *);
  void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
  void set_interline(byte interline) { _interline = _font.height + interline; }
  void set_interval(byte interval) { _interval = interval; }
  byte get_height() { return _font.height; }
  byte get_weight() { return _font.weight; }
  byte get_row() { return MAX_Y / _interline; }
  byte get_col() { return MAX_X / (_interval + _font.weight); }

private:
  Font  _font = {};     // Шрифт
  byte  _charSize = 0;  // Размер символа в байтах
  byte  _line = 0;      // Высота символа в байтах
  byte  _interline = 0; // Расстояние между строками
  byte  _interval = 0;  // Расстояние между символами
  byte  _tab_factor = FONT_TAB_FACTOR;
  uint16_t  point_x = 0;
  uint16_t  point_y = 0;

private:
  void next_position();
  void LF() { point_y += _interline; }
  void CR() { point_x = 0; }
  void TAB() { point_x = ((point_x / (_interval << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR; }
  void BS() { point_x -= (_font.weight + _interval); if (point_x > MAX_X) point_x = 0; }
  void escape() {}
};
