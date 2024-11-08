#include "display/display.h"

Display lcd;

int main(void)
{

  byte k = 3, i;

  lcd.clear(0U);
  lcd.fat(k);

  RGB c(0, 0, 0);
  while (true) {
    for (i = 0; i < 128 - k; i++) {
      lcd.color(c);
      lcd.lineFat(63, 79, i, 0);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b += 2;
      c.g++;
      c.r += 3;
    }
    for (i = 0; i < 159 - k; i++) {
      lcd.color(c);
      lcd.lineFat(63, 79, 127, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b += 3;
      c.g++;
      c.r += 2;
    }
    for (i = 127; i < 128 && i>k; i--) {
      lcd.color(c);
      lcd.lineFat(63, 79, i, 159);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b++;
      c.g += 3;
      c.r += 2;
    }
    for (i = 159; i < 160 && i>k; i--) {
      lcd.color(c);
      lcd.lineFat(63, 79, 0, i);
      // lcd.circleFat(63, 79, (i >> 2) + 10);
      c.b--;
      c.g += 2;
      c.r++;
    }
  }
}
