#include <Arduino.h>
#include "ST7735S.h"
#include "font_6x5.h"

void error()
{
  PORTF = 0xFF;
  while (true)
    ;
}

ST7735S lcd;

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

  SET_CS // CS Снять выбор дисплея
}

int main(void)
{
  lcd.clear(0x500);

  char string[] = "0123456789 1234567890X";
  byte n = 0;
  byte y = 0;
  byte x = 0;

  while (n < sizeof(string) - 1)
  {
    setSymbol(x, y, string[n++]);

    x += 6;
    if (x > 123)
    {
      y += 7;
      x = 0;
      if (y > 153)
        y = 0;
    }
  }
  /*
    x = 1;
    while (true)
    {
      lcd.test(x++);
      //  lcd.clear(0x00f);
      //  lcd.clear(0x0f0);
      //  lcd.clear(0xf00);
    }
    */

  PORTE &= ~_BV(PE6); // Set 0 to A16
  DDRE |= _BV(PE6);   // Output A16

  MCUCR |= _BV(SRE); // ESRAM Enable

  while (true)
  {
    x = 0;
    y = 0;

    word __volatile__ *address = (word *)0x6000;
    do
    {
      lcd.pixel(x++, y, *address);
      address++;
      if (x > 127)
      {
        y++;
        x = 0;
        if (y > 159)
          y = 0;
      }
    } while (x || y);
    PORTE ^= _BV(PE6);
  }
}
