#include "display/display.h"

Display lcd;

int main(void)
{
  RGB c = 0;
  lcd.clear(0);
  lcd.color(0x11f);
  lcd.roundRectFill(5, 5, 90, 90, 20);
  while (true) {
    lcd.color(c);
    lcd.roundRectFill(10, 10, 80, 80, 15);
    c.r(3);
    c.g(5);
    c.b(7);
  }
}
