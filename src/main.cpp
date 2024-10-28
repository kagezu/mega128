#include "init.h"
#include "ST7735S.h"
#include "font_6x5.h"

ST7735S lcd;

const byte dx = 4;
const byte dy = 5;

byte cursor_x = 0;
byte cursor_y = 0;

void set_at(byte x, byte y)
{
  cursor_x = x;
  cursor_y = y;
}

void symbol(byte symbol)
{
  lcd.symbol(symbol, cursor_x, cursor_y, dx, dy);
  cursor_x += dx + 2;
  if (cursor_x >= MAX_X - dx)
  {
    cursor_y += dy + 2;
    cursor_x = 0;
    if (cursor_y >= MAX_Y - dy)
      cursor_y = 0;
  }
}

void print(char *string, byte length)
{
  for (byte i = 0; i < length; i++)
    symbol(string[i]);
}

int main(void)
{
  // uint32_t x;
  byte x;
  while (true)
    lcd.demo(x++);
  // lcd.clear(x++ + (x << 7) + (x << 14));
}
