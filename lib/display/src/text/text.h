#ifndef FONT_H
#define FONT_H

#include <Arduino.h>
#include "display/display.h"

#define FONT_COUNT  pgm_read_byte(_font)
#define FONT_FIRST  pgm_read_byte(_font+1)
#define FONT_WEIGHT pgm_read_byte(_font+2)
#define FONT_HEIGHT pgm_read_byte(_font+3)

class Text {
private:
  Draw *_display;
  uint16_t  _font;
  uint16_t  _offset;
  uint8_t  _charSize;
  uint8_t  _line;

public:
  uint8_t cursorX = 0;
  uint8_t cursorY = 0;

public:
  Text(Draw *lcd) :_display(lcd) {};

  void font(const uint8_t *font);
  void symbol(uint8_t symbol);
  void at(uint8_t x, uint8_t y);

  void print(const char *string);
  void printPstr(const char *string);
  void printR(const char *string);

  void print(uint8_t number);
  void print(uint16_t number);
  void print(uint32_t number);

  uint8_t hexToChar(uint8_t number);
  void printHex(uint32_t number);
  void printHex(uint16_t number);
  void printHex(uint8_t number);

  uint8_t getRow() { return MAX_Y / (FONT_HEIGHT + 1); }
  uint8_t getCol() { return MAX_X / (FONT_WEIGHT + 1); }

  uint8_t getHeight() { return FONT_HEIGHT + 1; }

};

#endif
