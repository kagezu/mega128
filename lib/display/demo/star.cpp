#include <display.h>

Display display;

#define COUNT_STAR  50

byte cycle[COUNT_STAR][2];
byte i;

int main()
{
  TCCR0B = 1;
  sei();
  display.init();
  display.update();
  while (true) {
    for (char j = 0; j < 3; j++) {
      byte x = rand() & 0x7f, y = rand() & 0x1f;
      display.color(0);
      display.pixel(cycle[i][0], cycle[i][1]);
      display.color(1);
      display.pixel(x, y);
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    delay(50);
    display.update();
  }
}
