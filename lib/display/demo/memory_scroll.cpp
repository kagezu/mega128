#include <display.h>
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;

#define INTERLINE 3

int main(void)
{
  // lcd.init();
  lcd.background(RGB(0x04, 0x04, 0x10));
  lcd.color(RGB(0xFF, 0xE0, 0xE0));
  lcd.clear();

  // lcd.font(&micro_5x6);
  // lcd.font(&system_5x7);
  lcd.font(&standard_5x8);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  // lcd.font(&arial_14);
  lcd.set_interline(INTERLINE);

  byte *ptr = (byte *)0x100;

  while (true) {
    for (char k = lcd.get_height() + INTERLINE; k > 0; k--) {
      byte *p = ptr;
      lcd.at(0, k);
      for (byte i = 0; i < lcd.get_row(); i++) {
        lcd.printf(F("%2x %x %x %x %x"), p, *p, *(p + 1), *(p + 2), *(p + 3));
        lcd.printf(F(" %x %x %x %x \n"), *(p + 4), *(p + 5), *(p + 6), *(p + 7));
        p += 8;
      }
    }
    ptr += 8;
  }
}
