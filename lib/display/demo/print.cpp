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
  RGB background(0x04, 0x00, 0x40);
  lcd.clear(background);
  lcd.background(background);
  lcd.color(RGB(0xff, 0xff, 0x04));

  // lcd.font(&micro_5x6);
  // lcd.font(&system_5x7);
  // lcd.font(&standard_5x7);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  lcd.font(&arial_14);
  lcd.set_interline(3);

  byte x = 48;

  lcd.printf(PSTR("\f"));
  lcd.printf(PSTR("%2u \n"), x);
  lcd.printf(PSTR("\t\t\t%2i \n"), -30000);
  lcd.printf(PSTR("\t\t\t%.u....\n"), 30000);
  lcd.printf(PSTR("\t\t\t%4.u \n"), 400000l);

  while (true);
}
