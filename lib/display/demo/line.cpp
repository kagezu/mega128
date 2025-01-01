#include "display/display.h"

Display lcd;

int main(void)
{

  byte i;

  lcd.clear(0);

  RGB c = 0x000;
  while (true) {
    for (i = 0; i < 128; i++) {
      lcd.color(c);
      lcd.line(63, 79, i, 0);
      c.b(1);
      c.g(1);
      c.r(1);
    }
    for (i = 0; i < 159; i++) {
      lcd.color(c);
      lcd.line(63, 79, 127, i);
      c.b(1);
      c.g(1);
      c.r(-1);
    }
    for (i = 127; i < 128; i--) {
      lcd.color(c);
      lcd.line(63, 79, i, 159);
      c.b(1);
      c.g(-1);
      c.r(1);
    }
    for (i = 159; i < 160; i--) {
      lcd.color(c);
      lcd.line(63, 79, 0, i);
      c.b(1);
      c.g(1);
      c.r(1);
    }

    c.b(5);
    c.g(5);
    c.r(5);
  }
}
