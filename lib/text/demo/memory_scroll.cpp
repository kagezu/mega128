#include "display/display.h"
#include "text.h"
#include "font/font_6x5.h"
#include "font/number_8x16.h"

Display lcd;
Text micro(&lcd);

int main(void)
{
  // XMCRB = 0;
  // bitSet(MCUCR, SRE);

  lcd.clear(0U);
  lcd.color(RGB(0x80E0FFu));

  micro.font(number_8x16);

  byte *ptr = (byte *)0x100;

  while (true) {
    // for (char k = 7; k >= 0; k--) {
    byte *p = (byte *)ptr;
    for (byte i = 0; i < 9; i++) {
      // micro.at(0, i * 8 + k);
      micro.at(0, i * 18);
      micro.printHex((word)p);
      for (byte j = 0; j < 2; j++)
        micro.printHex(*p++);
    }
    // }
    ptr += 2;
  }
}
