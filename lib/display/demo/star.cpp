#include <display.h>
#include "font/arial_14.h"

Display display;

#define COUNT_STAR  50

byte cycle[COUNT_STAR][2];
RGB black(0, 0, 0);
RGB white(255, 255, 255);
byte i;

int main()
{
  TCCR0B = 1;
  sei();
  display.font(&arial_14);
  display.init();

  while (true) {
    display.at(30, (MAX_Y - 14) >> 1);
    display.printf(PSTR("Hello World"));
    for (char j = 0; j < 20; j++) {
      byte x = rand() & MAX_X, y = rand() & MAX_Y;
      display.color(black);
      display.pixel(cycle[i][0], cycle[i][1]);
      display.color(white);
      display.pixel(x, y);
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    display.update();
  }
}
