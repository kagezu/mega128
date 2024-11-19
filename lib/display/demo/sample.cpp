#include "display/display.h"

Display lcd;

int main(void)
{
  for (;;) {
    lcd.clear(0xf800);
    lcd.clear(0x07E0);
    lcd.clear(0x001F);
    lcd.clear(0x0);
    // for (int i = 0; i < 300; i++) {
    //   lcd.color(random(65535));
    //   lcd.rectLine(random(127), random(159), random(127), random(159)); // rectangle at x, y, with, hight, color
    // }
  }
}
