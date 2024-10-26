#include <Arduino.h>
#include "ST7735S.h"
#include "font_6x5.h"
#include "util/atomic.h"

ST7735S lcd(RGB_12);

void setSymbol(byte x, byte y, byte symbol)
{
  // lcd.setBlock(x, y, x + 5, y + 6);
  for (byte i = 0; i < 6; i++)
  {
    byte data = pgm_read_byte(font_6x5 + symbol * 6 + i - 192);
    byte v = 0x10;
    byte j = 0;
    while (v)
    {
      word color = data & v ? 0xfff : 0x500;
      // lcd.data_12(color);
      lcd.pixel(x + 5 - j, y + i, color);
      v = v >> 1;
      j++;
    }
  }
}

int main(void)
{
  // lcd.clear(0x500);
  /*
    char string[] = "0123456789 1234567890X";
    byte n = 0;
    byte y = 0;
    byte x = 0;

    while (n < sizeof(string) - 1)
    {
      // setSymbol(x, y, string[n++]);

      x += 6;
      if (x > 123)
      {
        y += 7;
        x = 0;
        if (y > 153)
          y = 0;
      }
    }
    */

  byte x;
  while (true)
  {
    lcd.test(x++);
  }

  /*
  PORTE &= ~_BV(PE6); // Set 0 to A16
  DDRE |= _BV(PE6);   // Output A16

  MCUCR |= _BV(SRE); // ESRAM Enable

  byte __volatile__ *address = (byte *)0x1000;
  // PORTE ^= _BV(PE6);
}
*/
}
