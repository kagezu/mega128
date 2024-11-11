#ifndef FONT_H
#define FONT_H

#include <Arduino.h>
#include "display/display.h"

class Text {
private:
  Draw *_display;
  byte *_font;

public:
  Text(Draw *lcd) :_display(lcd) {};
  void font(const byte *font) { _font = (byte *)font; }

  byte cursorX = 0;
  byte cursorY = 0;

  void at(byte x, byte y)
  {
    cursorX = x;
    cursorY = y;
  }

  void symbol(byte symbol)
  {
    byte dx = pgm_read_byte(_font);
    byte dy = pgm_read_byte(_font + 1);
    byte ds = pgm_read_byte(_font + 2);
    byte *source = (byte *)(_font + (symbol - ds) * dy + 3);

    _display->symbol(source, cursorX, cursorY, dx, dy);
    cursorX += dx + 1;
    if (cursorX > MAX_X - dx) {
      cursorY += dy + 2;
      cursorX = 0;
      if (cursorY > MAX_Y - dy)
        cursorY = 0;
    }
  }
  /*
  void symbol(byte symbol)
  {
    byte dx = pgm_read_byte(_font);
    byte dy = pgm_read_byte(_font + 1);
    byte ds = pgm_read_byte(_font + 2);
    byte *source = (byte *)(_font + (symbol - ds) * dy + 3);

    _display->symbol(source, cursorX, cursorY, dx, dy);
    cursorX += dx + 1;
    if (cursorX > MAX_X - dx) {
      cursorY += dy + 2;
      cursorX = 0;
      if (cursorY > MAX_Y - dy)
        cursorY = 0;
    }
  }
*/
  void print(const char *string)
  {
    while (char ch = *string++)
      symbol(ch);
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
    string[1] = ':';
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
    string[0] = '0';
    string[1] = ':';
    string[6] = 0;

    string[5] = hexToChar(number);
    number >>= 4;
    string[4] = hexToChar(number);
    number >>= 4;
    string[3] = hexToChar(number);
    number >>= 4;
    string[2] = hexToChar(number);

    print(string);
  }

  void printHex(byte number)
  {
    char string[4];
    string[3] = 0;
    string[2] = hexToChar(number);
    number >>= 4;
    string[1] = hexToChar(number);
    string[0] = '.';
    print(string);
  }
};
#endif
