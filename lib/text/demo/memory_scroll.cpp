#include "display/display.h"
#include "font.h"

Display lcd;
Font micro(&lcd);

int main(void)
{
  lcd.clear(0U);
  byte *ptr = (byte *)&lcd;
  while (true) {
    for (byte k = 7; k >= 0; k--) {
      byte *p = (byte *)ptr;
      for (byte i = 0; i < 18; i++) {
        micro.at(0, i * 8 + k);
        micro.printHex((word)p);
        for (byte j = 0; j < 4; j++)
          micro.printHex(*p++);
      }
    }
    ptr += 4;
  }
}
