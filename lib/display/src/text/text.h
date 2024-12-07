#ifndef FONT_H
#define FONT_H

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
  uint16_t  _font;
  uint16_t  _offset;
  uint8_t  _charSize;
  uint8_t  _line;
  uint8_t  _interline;
  uint8_t  _interval;

public:
  uint8_t cursorX = 0;
  uint8_t cursorY = 0;

public:
  Text(Draw *lcd) :_display(lcd) {};

  void font(const uint8_t *font);

  void printf(const char *string, ...);
  void symbol(uint8_t symbol);
  void print(const char *string);
  void printPstr(const char *string);

  void print(uint8_t number);
  void print(uint16_t number);
  void print(uint32_t number);
  void print(int8_t number);
  void print(int16_t number);
  void print(int32_t number);

  uint8_t hexToChar(uint8_t number);
  void printHex(uint64_t number);
  void printHex(uint32_t number);
  void printHex(uint16_t number);
  void printHex(uint8_t number);

  uint8_t getRow() { return MAX_Y / _interline; }
  uint8_t getCol() { return MAX_X / _interval; }

public:
  inline  void at(uint8_t x, uint8_t y) { cursorX = x; cursorY = y; }
  inline void setInterline(uint8_t interline) { _interline = FONT_HEIGHT + interline; }
  inline void setInterval(uint8_t interval) { _interval = (FONT_WEIGHT & 0x7f) + interval; }

  // Вертикальная табуляция / Перевод строки
  inline void printLF() { cursorY += _interline; }
  // Возврат каретки
  inline void printCR() { cursorX = 0; }
  // Табуляция
  inline void printTAB() { cursorX = ((cursorX / (_interval << FONT_TAB_FACTOR) + 1) * _interval) << FONT_TAB_FACTOR; }
  // Шаг назад
  inline void printBS() { cursorX -= _interval; if (cursorX > MAX_X) cursorX = 0; }

  // Сигнал (обратный вызов)
  inline void bel() {}
  // Escape (обратный вызов)
  inline void escape() {}
};

#endif
