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
  byte *_font;
  word  _offset;
  byte  _charSize;
  byte  _line;

public:
  byte cursorX = 0;
  byte cursorY = 0;

  Text(Draw *lcd) :_display(lcd) {};
  void font(const byte *font)
  {
    _font = (byte *)font;
    _line = (1 + ((FONT_HEIGHT - 1) >> 3));
    _charSize = FONT_WEIGHT * _line;

    _offset = (word)_font + 4;
    if (!FONT_WEIGHT)
      _offset += (FONT_COUNT + 1) * 2;
  }

  void at(byte x, byte y)
  {
    cursorX = x;
    cursorY = y;
  }

  void symbol(byte symbol)
  {
    symbol -= FONT_FIRST;
    if (FONT_COUNT <= symbol) return;

    byte dx = FONT_WEIGHT;
    byte dy = FONT_HEIGHT;
    word source;

    if (dx)
      source = _offset + symbol * _charSize;
    else {
      source = pgm_read_word(_font + symbol * 2 + 4);
      dx = (pgm_read_word(_font + symbol * 2 + 6) - source) / _line;
      source += _offset;
    }

    _display->symbol((byte *)source, cursorX, cursorY, dx, dy);
    cursorX += dx + 1;
    if (cursorX > MAX_X - dx) {
      cursorY += dy + 1;
      cursorX = 0;
      if (cursorY > MAX_Y - dy)
        cursorY = 0;
    }
  }

  void printR(const char *string)
  {
    while (byte ch = *string++) {
      if (ch < 0xd0)
        symbol(ch);
    }
  }

  void print(const char *string)
  {
    while (char ch = *string++)
      symbol(ch);
  }

  void printPstrR(const char *string)
  {
    word *str = (word *)++string;
    while (*str & 0xff00) {
      char ch = pgm_read_byte(str++);
      symbol(ch);
    }
  }

  void printPstr(const char *string)
  {
    while (char ch = pgm_read_byte(string++))
      symbol(ch);
  }

  void print(word number)
  {
    char string[6];
    byte i = 0;
    word mult = 10000;
    while (number || mult) {
      char n = number / mult;
      if (i || n)
        string[i++] = '0' + n;
      number -= n * mult;
      mult /= 10;
    }
    string[i] = 0;
    print(string);
  }

  byte hexToChar(byte number)
  {
    number &= 0xf;
    return number > 9 ? number + 7 + '0' : number + '0';
  }

  void printHex(uint32_t number)
  {
    char string[11];
    string[0] = '0';
    string[1] = 'x';
    string[10] = 0;

    string[9] = hexToChar(number);
    number >>= 4;
    string[8] = hexToChar(number);
    number >>= 4;
    string[7] = hexToChar(number);
    number >>= 4;
    string[6] = hexToChar(number);
    number >>= 4;
    string[5] = hexToChar(number);
    number >>= 4;
    string[4] = hexToChar(number);
    number >>= 4;
    string[3] = hexToChar(number);
    number >>= 4;
    string[2] = hexToChar(number);

    print(string);
  }

  void printHex(word number)
  {
    char string[7];
    // string[0] = '0';
    // string[1] = 'x';
    string[4] = 0;

    string[3] = hexToChar(number);
    number >>= 4;
    string[2] = hexToChar(number);
    number >>= 4;
    string[1] = hexToChar(number);
    number >>= 4;
    string[0] = hexToChar(number);

    print(string);
  }

  void printHex(byte number)
  {
    char string[4];
    string[3] = 0;
    string[2] = hexToChar(number);
    number >>= 4;
    string[1] = hexToChar(number);
    string[0] = ' ';
    print(string);
  }
};
#endif
