#include "display/display.h"

Display lcd;

int main(void)
{
  lcd.clear(RGB(0, 255, 255));

  uint8_t k = 0;
  for (;;) {
    lcd.test(k);
    k += 4;
  }
}
