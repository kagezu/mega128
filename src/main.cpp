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
    byte data = pgm_read_byte(font_6x5 + symbol * 6 + i);
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
  byte n = 30;
  for (byte y = 0; y < 150; y += 7)

    for (byte x = 0; x < 120; x += 6)
    {
      setSymbol(x, y, n++);
    }

  /*
  int x = 1;
  while (true)
  {
    lcd.test(x++);
    //  lcd.clear(0x00f);
    //  lcd.clear(0x0f0);
    //  lcd.clear(0xf00);
  }
    PORTF = 0;
    DDRF = 0xFF;

    PORTE &= ~_BV(PE6); // Set 0 to A16
    DDRE |= _BV(PE6);   // Output A16

    MCUCR |= _BV(SRE); // ESRAM Enable

    while (true)
    {
      uint8_t __volatile__ *address = (uint8_t *)0x1100;
      while (address++)
      {
        *address = 0;
        if (*address)
          error();

        *address = 0xff;
        if (*address != 0xff)
          error();
      }

      // delay(100);
      PORTF += 1;
    }
  */
}
