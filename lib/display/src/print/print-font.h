#pragma once

#include <Arduino.h>
#include "display/config.h"
#include "print-format.h"
#include "font/font.h"

#define FONT_TAB_FACTOR     3

class PrintFont : public PrintFormat {
private:
  Font  _font = {};     // Шрифт
  byte  _charSize = 0;  // Размер символа в байтах
  byte  _line = 0;      // Высота символа в байтах
  byte  _interline = 0; // Расстояние между строками
  byte  _interval = 0;  // Расстояние между символами
  byte  _tab_factor = FONT_TAB_FACTOR;

public:
  byte point_x = 0;
  byte point_y = 0;

public:

  void font(const Font *);

  void letter(byte);
  void write(byte);
  void write(byte *, byte);

  byte get_row() { return MAX_Y / _interline; }
  byte get_col() { return MAX_X / _interval; }

public:
  inline  void at(byte x, byte y) { point_x = x; point_y = y; }
  inline void set_interline(byte interline) { _interline = _font.height + interline; }
  inline void set_interval(byte interval) { _interval = interval; }
  inline byte get_height() { return _font.height; }
  inline byte get_weight() { return _font.weight; }

  // Вертикальная табуляция / Перевод строки
  inline void LF() { point_y += _interline; }
  // Возврат каретки
  inline void CR() { point_x = 0; }
  // Табуляция
  inline void TAB() { point_x = ((point_x / (_interval << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR; }
  // Шаг назад
  inline void BS() { point_x -= (_font.weight + _interval); if (point_x > MAX_X) point_x = 0; }

  // Escape (обратный вызов)
  inline void escape() {}

  virtual void symbol(byte *source, byte point_x, byte point_y, byte dx, byte dy) = 0;
  virtual void clear();
};
