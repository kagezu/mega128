#include <Arduino.h>
#include "ST7735S.h"
#include "font_6x5.h"

class Font
{
public:
  ST7735S *display;

public:
  Font(ST7735S *lcd);

  const byte dx = 4;
  const byte dy = 6;

  byte cursor_x = 0;
  byte cursor_y = 0;

  void set_at(byte x, byte y)
  {
    cursor_x = x;
    cursor_y = y;
  }

  void symbol(byte symbol)
  {
    display->symbol(font_6x5, symbol, cursor_x, cursor_y, dx, dy);
    cursor_x += dx + 1;
    if (cursor_x > MAX_X - dx)
    {
      cursor_y += dy + 2;
      cursor_x = 0;
      if (cursor_y > MAX_Y - dy)
        cursor_y = 0;
    }
  }

  void print(const char *string)
  {
    while (char ch = *string++)
      symbol(ch);
  }

  void print(word number)
  {
    char string[6];
    byte i = 0;
    word mult = 10000;
    while (number)
    {
      char n = number / mult;
      if (i || n)
        string[i++] = '0' + n;
      number -= n * mult;
      mult /= 10;
    }
    string[i] = 0;
    print(string);
  }
};
