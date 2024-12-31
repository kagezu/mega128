#include "display/display.h"
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x7.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;

void get(void **ptr)
{
  lcd.print("ptr: ");
  lcd.print((uint16_t)ptr);
  lcd.printf(PSTR(" "));

  lcd.print("*ptr: ");
  lcd.print((uint16_t)*ptr);
  lcd.printf(PSTR(" "));

  lcd.print("**ptr: ");
  lcd.print((uint16_t) * *(uint16_t **)ptr);
  lcd.printf(PSTR(" "));
  lcd.printf(PSTR(" "));

  *ptr = (void *)222;
}

word a = 11111;

int main(void)
{
  lcd.clear(0U);
  lcd.color(RGB(0x80, 0xE0, 0xFF));

  //lcd.font(micro_5x6);
  //lcd.font(system_5x7);
  //lcd.font(standard_5x7);
  //lcd.font(number_8x16);
  //lcd.font(number_15x31);
  lcd.font(&arial_14);
  lcd.set_interline(0);

  // for (byte i = 32; i < 200; i++) {
  //  lcd.symbol(i);
  // }

  // word *ptr = &a;
  // get((void **)&ptr);
  // get((void **)&ptr);

  // lcd.printR(PSTR("просто 654*543"));
  //lcd.print(12300678u);
  //lcd.print(" = ");
  //lcd.print((word)12300);
  //lcd.print(" - ");
  //lcd.print((byte)123);

// /*
  byte *ptr = (byte *)0x100;

  while (true) {
    for (char k = lcd.get_height() - 1; k >= 0; k--) {
      byte *p = (byte *)ptr;
      for (byte i = 0; i < lcd.get_row(); i++) {
        lcd.at(0, i * (lcd.get_height()) + k);
        lcd.print_h((word)p);
        for (byte j = 0; j < 4; j++) {
          lcd.write(' ');
          lcd.print_h(*p++);
        }
      }
    }
    ptr += 4;
  }
  // */

}
