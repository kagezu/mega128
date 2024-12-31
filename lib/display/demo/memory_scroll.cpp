#include "display/display.h"
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x7.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;

int main(void)
{
  lcd.clear(0);
  lcd.color(RGB(0x80, 0xE0, 0xFF));

  // lcd.font(&micro_5x6);
  // lcd.font(&system_5x7);
  // lcd.font(&standard_5x7);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  lcd.font(&arial_14);
  lcd.set_interline(0);

  byte *ptr = (byte *)0x100;

  while (true) {
    for (char k = lcd.get_height() - 1; k >= 0; k--) {
      byte *p = (byte *)ptr;
      for (byte i = 0; i < lcd.get_row(); i++) {
        lcd.at(0, i * (lcd.get_height()) + k);
        lcd.printf(PSTR("%2x %x %x %x %x"), p, *p++, *p++, *p++, *p++);
      }
    }
  ptr += 4;
  }
}
