// #define F_CPU 14745600L
#include <Arduino.h>

void error()
{
  PORTF = 0xFF;
  while (true)
    ;
}

int main(void)
{

  PORTF = 0;
  DDRF = 0xFF;

  PORTE &= ~_BV(PE6); // Set 0 to A16
  DDRE |= _BV(PE6);   // Output A16

  MCUCR |= _BV(SRE); // XRAM Enable

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

  return 0;
}
