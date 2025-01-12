#include "display/display.h"

Display lcd;

int main(void)
{
  lcd.clear(RGB(0, 255, 255));

  byte k = 0;
  for (;;) {
    lcd.test(k);
    k += 4;
  }
}
