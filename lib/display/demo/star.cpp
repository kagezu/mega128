#include <SSD1306/SSD1306.h>

SSD1306 display;

#define COUNT_STAR  50

byte cycle[COUNT_STAR][2];
byte i;

int main()
{
  TCCR0B = 1;
  sei();
  display.begin(0x3C);
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
