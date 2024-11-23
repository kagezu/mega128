#include "display/display.h"
#include "text.h"
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x7.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"
#include "font/arial_14.h"

Display lcd;
Text micro(&lcd);

void get(void **ptr)
{
  micro.print("ptr: ");
  micro.print((uint16_t)ptr);
  micro.printR(PSTR(" "));

  micro.print("*ptr: ");
  micro.print((uint16_t)*ptr);
  micro.printR(PSTR(" "));

  micro.print("**ptr: ");
  micro.print((uint16_t) * *(uint16_t **)ptr);
  micro.printR(PSTR(" "));
  micro.printR(PSTR(" "));

  *ptr = (void *)222;
}

word a = 11111;

int main(void)
{
  lcd.clear(0U);
  lcd.color(RGB(0x80E0FFu));

  // micro.font(micro_5x6);
  micro.font(system_5x7);
  // micro.font(standard_5x7);
  // micro.font(number_8x16);
  // micro.font(number_15x31);
  // micro.font(arial_14);

  // for (byte i = 32; i < 200; i++) {
  //   micro.symbol(i);
  // }

  word *ptr = &a;
  get((void **)&ptr);
  get((void **)&ptr);

  //  micro.printR(PSTR("просто 654*543"));
  // micro.print(12300678u);
  // micro.print(" = ");
  // micro.print((word)12300);
  // micro.print(" - ");
  // micro.print((byte)123);

/*
  byte *ptr = (byte *)0x100;

  while (true) {
    for (char k = micro.getHeight() - 1; k >= 0; k--) {
      byte *p = (byte *)ptr;
      for (byte i = 0; i < micro.getRow(); i++) {
        micro.at(0, i * micro.getHeight() + k);
        micro.printHex((word)p);
        for (byte j = 0; j < 4; j++) {
          micro.symbol(' ');
          micro.printHex(*p++);
        }
      }
    }
    ptr += 4;
  }
*/

}
