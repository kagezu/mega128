#include "display/display.h"

Display lcd;

int main(void)
{
  lcd.clear(0U);

  RGB c(0, 0, 0);
  while (true) {
    for (byte i = 0; i < 128; i++) {
      lcd.color(c);
      lcd.line(63, 79, i, 0);
      lcd.circle(63, 79, i);
      c.b += 2;
      c.g++;
      c.r += 3;
    }
    for (byte i = 0; i < 159; i++) {
      lcd.color(c);
      lcd.line(63, 79, 127, i);
      lcd.circle(63, 79, i);
      c.b += 3;
      c.g++;
      c.r += 2;
    }
    for (byte i = 127; i > 0; i--) {
      lcd.color(c);
      lcd.line(63, 79, i, 159);
      lcd.circle(63, 79, i);
      c.b++;
      c.g += 3;
      c.r += 2;
    }
    for (byte i = 159; i > 0; i--) {
      lcd.color(c);
      lcd.line(63, 79, 0, i);
      lcd.circle(63, 79, i);
      c.b--;
      c.g += 2;
      c.r++;
    }
  }
}
