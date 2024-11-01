#include <Arduino.h>
#include "ST7735S.h"
#include "font.h"
#include "x_page.h"

byte m[100];
ST7735S lcd;
Font micro(&lcd);
XPage page(0);
void *temp = page.create(100);
byte(&a)[10][10] = (byte(&)[10][10])temp;

int main(void)
{
  for (char y = 0; y < 10; y++)
    for (char x = 0; x < 10; x++)
      a[y][x] = x * y;

  lcd.clear(0);
  byte *ptr = (byte *)a;
  while (true) {
    for (char k = 7; k >= 0; k--) {
      byte *p = (byte *)ptr;
      for (char i = 0; i < 18; i++) {
        micro.at(0, i * 8 + k);
        micro.printHex((word)p);
        for (char j = 0; j < 4; j++)
          micro.printHex(*p++);
      }
    }
    ptr += 4;
  }

  byte x;
  while (true)
    lcd.demo(x++);
}
