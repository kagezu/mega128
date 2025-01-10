#include <display.h>
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;

int main(void)
{
  // lcd.init();
  lcd.clear();
  lcd.color(RGB(0x80, 0xE0, 0xFF));

  // lcd.font(&micro_5x6);
  // lcd.font(&system_5x7);
  // lcd.font(&standard_5x8);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  lcd.font(&arial_14);
  lcd.set_interline(1);

  byte *ptr = (byte *)0x100;

  while (true) {
    for (byte k = lcd.get_height() - 1; k >= 0; k--) {
      // lcd.update();
      // lcd.update();
      // lcd.update();
      // lcd.update();
      // lcd.update();
      // lcd.clear();
      byte *p = ptr;
      for (byte i = 0; i < lcd.get_row(); i++) {
        lcd.at(0, i * (lcd.get_height()) + k);
        lcd.printf(F("%2x %x %x %x %x"), p, *p, *(p + 1), *(p + 2), *(p + 3));
        p += 4;
      }
    }
    ptr += 4;
  }
}
