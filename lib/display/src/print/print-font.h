#pragma once

#include <Arduino.h>
#include "display.config.h"
#include "print-format.h"

#define FONT_COUNT  pgm_read_byte(_font)
#define FONT_FIRST  pgm_read_byte(_font+1)
#define FONT_WEIGHT pgm_read_byte(_font+2)
#define FONT_HEIGHT pgm_read_byte(_font+3)

#define FONT_OFFSET         4
#define FONT_TAB_FACTOR     2

struct Font {
  byte count;
  byte first;
  byte weight;
  byte height;
};

class PrintFont : public PrintFormat {
private:
  word  _font;
  word  _offset;
  byte  _charSize;
  byte  _line;
  byte  _interline;
  byte  _interval;

public:
  byte point_x = 0;
  byte point_y = 0;

public:

  void font(const byte *font);

  void letter(byte);
  void write(byte);

  byte get_row() { return MAX_Y / _interline; }
  byte get_col() { return MAX_X / _interval; }

public:
  inline  void at(byte x, byte y) { point_x = x; point_y = y; }
  inline void set_interline(byte interline) { _interline = FONT_HEIGHT + interline; }
  inline void set_interval(byte interval) { _interval = (FONT_WEIGHT & 0x7f) + interval; }
  inline byte get_height() { return FONT_HEIGHT; }
  inline byte get_weight() { return FONT_WEIGHT & 0x7f; }

  // Вертикальная табуляция / Перевод строки
  inline void LF() { point_y += _interline; }
  // Возврат каретки
  inline void CR() { point_x = 0; }
  // Табуляция
  inline void TAB() { point_x = ((point_x / (_interval << FONT_TAB_FACTOR) + 1) * _interval) << FONT_TAB_FACTOR; }
  // Шаг назад
  inline void BS() { point_x -= _interval; if (point_x > MAX_X) point_x = 0; }

  // Escape (обратный вызов)
  inline void escape() {}

  virtual void symbol(byte *source, byte point_x, byte point_y, byte dx, byte dy);

};
