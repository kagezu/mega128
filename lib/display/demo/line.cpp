#include "display/display.h"

Display lcd;

int main(void)
{

  byte k = 2, i;

  lcd.clear(0);
  lcd.fat(k);

  RGB c = 0x000;
  while (true) {
    for (i = 0; i < 128; i++) {
      lcd.color(c);
      lcd.line(63, 79, i, 0);
      // lcd.lineFat(63, 79, i, 0);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      // lcd.circleFill(63, 79, (i >> 2) + 10);
      c.b(1);
      c.g(1);
      c.r(1);
    }
    for (i = 0; i < 159; i++) {
      lcd.color(c);
      lcd.line(63, 79, 127, i);
      // lcd.lineFat(63, 79, 127, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      // lcd.circleFill(63, 79, (i >> 2) + 10);
      c.b(1);
      c.g(1);
      c.r(-1);
    }
    for (i = 127; i < 128; i--) {
      lcd.color(c);
      lcd.line(63, 79, i, 159);
      // lcd.lineFat(63, 79, i, 159);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      // lcd.circleFill(63, 79, (i >> 2) + 10);
      c.b(1);
      c.g(-1);
      c.r(1);
    }
    for (i = 159; i < 160; i--) {
      lcd.color(c);
      lcd.line(63, 79, 0, i);
      // lcd.lineFat(63, 79, 0, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      // lcd.circleFill(63, 79, (i >> 2) + 10);
      c.b(1);
      c.g(1);
      c.r(1);
    }

      c.b(5);
      c.g(5);
      c.r(5);
  }
}
