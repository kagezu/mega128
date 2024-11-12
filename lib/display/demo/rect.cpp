#include "display/display.h"

Display lcd;

int main(void)
{
  RGB c = 0u;
  lcd.clear(0U);
  lcd.color(0xa0a0ffU);
  lcd.pixel(50, 50, 0xa0a0ffU);
  lcd.roundRectFill(5, 5, 90, 90, 20);
  while (true) {
    lcd.color(c);
    lcd.roundRectFill(10, 10, 80, 80, 15);
    c.r += 3;
    c.g += 5;
    c.b += 7;
  }
}
