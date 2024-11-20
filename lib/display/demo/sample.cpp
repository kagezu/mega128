#include "display/display.h"

Display lcd;

int main(void)
{
  for (;;) {

    lcd.clear(0xff0000);
    lcd.clear(0x00ff00);
    lcd.clear(0x0000ff);
    lcd.clear(0x0);

    // for (int i = 0; i < 300; i++) {
    //   lcd.color(random(65535)*random(255));
    //   lcd.rectLine(random(127), random(159), random(127), random(159));
    // }
  }
}
