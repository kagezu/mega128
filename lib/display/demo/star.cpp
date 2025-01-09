#include <display.h>
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;

#define COUNT_STAR  50

byte cycle[COUNT_STAR][2];
RGB black(0, 0, 0);
RGB white(255, 255, 255);
byte i;

int main()
{
  // lcd.font(&micro_5x6);
  // lcd.font(&system_5x7);
  // lcd.font(&standard_5x8);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  lcd.font(&arial_14);
  lcd.init();
  // lcd.set_interval(2);

  while (true) {
    lcd.at(30, (MAX_Y - lcd.get_height()) >> 1);
    lcd.printf(PSTR("Hello World"));
    for (char j = 0; j < 20; j++) {
      byte x = rand() & MAX_X, y = rand() & MAX_Y;
      lcd.color(black);
      lcd.pixel(cycle[i][0], cycle[i][1]);
      lcd.color(white);
      lcd.pixel(x, y);
      cycle[i][0] = x;
      cycle[i][1] = y;
      i++;
      if (i == COUNT_STAR) i = 0;
    }
    lcd.update();
  }
}
