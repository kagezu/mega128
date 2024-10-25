#include <Arduino.h>
#include "ST7735S.h"

void error()
{
  PORTF = 0xFF;
  while (true)
    ;
}

ST7735S lcd;

int main(void)
{

  delayMicroseconds(55000);

  lcd.init();
  lcd.clear(0);

  int x = 1;
  while (true)
  {
    lcd.test(x++);
    //  lcd.clear(0x00f);
    //  lcd.clear(0x0f0);
    //  lcd.clear(0xf00);
  }

  /*
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

  // return 0;
}
