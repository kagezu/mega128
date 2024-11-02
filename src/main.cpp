#include <Arduino.h>
#include "ST7735S.h"
#include "font.h"
#include "x_page.h"


byte volatile m[32];
// ST7735S lcd;
// Font micro(&lcd);
XPage page(0, 0x0500);
void volatile *temp = page.create(25);
byte volatile (&a)[5][5] = (byte(&)[5][5])temp;

int main(void)
{
  for (byte y = 0; y < 5; y++)
    for (byte x = 0; x < 5; x++)
      a[y][x] = x + y;

  /*
    lcd.clear(0);
    byte *ptr = (byte *)a;
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

    byte x;
    while (true)
      lcd.demo(x++);
  */
}
