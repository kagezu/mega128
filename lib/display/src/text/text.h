#pragma once

#include <Arduino.h>
#include "display/display.h"

#define FONT_COUNT  pgm_read_byte(_font)
#define FONT_FIRST  pgm_read_byte(_font+1)
#define FONT_WEIGHT pgm_read_byte(_font+2)
#define FONT_HEIGHT pgm_read_byte(_font+3)

#define FONT_OFFSET         4
#define FONT_TAB_FACTOR     2

class Text {
private:
  Draw *_display;
  word  _font;
  word  _offset;
  byte  _charSize;
  byte  _line;
  byte  _interline;
  byte  _interval;

public:
  byte cursorX = 0;
  byte cursorY = 0;

public:
  Text(Draw *lcd) :_display(lcd) {};

  void font(const byte *font);

  void printf(const char *string, ...);
  void symbol(byte symbol);
  void print(const char *string);
  void print_pstr(const char *string);

  void print(byte number);
  void print(word number);
  void print(uint32_t number);
  void print(int8_t number);
  void print(int16_t number);
  void print(int32_t number);

  byte hex_to_char(byte number);
  void printHex(uint64_t number);
  void printHex(uint32_t number);
  void printHex(word number);
  void printHex(byte number);

  byte get_row() { return MAX_Y / _interline; }
  byte get_col() { return MAX_X / _interval; }

public:
  inline  void at(byte x, byte y) { cursorX = x; cursorY = y; }
  inline void set_inter_line(byte interline) { _interline = FONT_HEIGHT + interline; }
  inline void set_interval(byte interval) { _interval = (FONT_WEIGHT & 0x7f) + interval; }

  // Вертикальная табуляция / Перевод строки
  inline void LF() { cursorY += _interline; }
  // Возврат каретки
  inline void CR() { cursorX = 0; }
  // Табуляция
  inline void TAB() { cursorX = ((cursorX / (_interval << FONT_TAB_FACTOR) + 1) * _interval) << FONT_TAB_FACTOR; }
  // Шаг назад
  inline void BS() { cursorX -= _interval; if (cursorX > MAX_X) cursorX = 0; }

  // Сигнал (обратный вызов)
  inline void bel() {}
  // Escape (обратный вызов)
  inline void escape() {}
};
