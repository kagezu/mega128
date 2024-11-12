#include "display/display.h"

Display lcd;

int main(void)
{

  byte k = 2, i;

  lcd.clear(0U);
  lcd.fat(k);

  RGB c = 0xffffffu;
  while (true) {
    for (i = 0; i < 128; i++) {
      lcd.color(c);
      lcd.line(63, 79, i, 0);
      // lcd.lineFat(63, 79, i, 0);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b++;
      c.g++;
      c.r++;
    }
    for (i = 0; i < 159; i++) {
      lcd.color(c);
      lcd.line(63, 79, 127, i);
      // lcd.lineFat(63, 79, 127, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b++;
      c.g++;
      c.r--;
    }
    for (i = 127; i < 128; i--) {
      lcd.color(c);
      lcd.line(63, 79, i, 159);
      // lcd.lineFat(63, 79, i, 159);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b++;
      c.g--;
      c.r++;
    }
    for (i = 159; i < 160; i--) {
      lcd.color(c);
      lcd.line(63, 79, 0, i);
      // lcd.lineFat(63, 79, 0, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b--;
      c.g++;
      c.r--;
    }
  }
}
