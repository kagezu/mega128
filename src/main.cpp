#include <Arduino.h>
#include "ST7735S.h"
#include "font_6x5.h"
#include "util/atomic.h"

ST7735S lcd(RGB_12);

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
  lcd.clear(0x00);

  char string[] = "0123456789 1234567890X";
  for (byte i = 0; i < 23; i++)
    print(string, sizeof(string));

  /*
    lcd.clear(0x000);
    byte x = 0;
    while (true)
    {
      lcd.test(x++);
    }


    PORTE &= ~_BV(PE6); // Set 0 to A16
    DDRE |= _BV(PE6);   // Output A16

    MCUCR |= _BV(SRE); // ESRAM Enable

    byte __volatile__ *address = (byte *)0x1000;
    // PORTE ^= _BV(PE6);
  }
  */
}
