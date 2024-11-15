#include "display/display.h"
#include "text.h"
#include "font/system_5x7.h"
#include "font/micro_5x6.h"
#include "font/standard_5x7.h"
#include "font/number_8x16.h"
#include "font/number_15x31.h"

Display lcd;
Text micro(&lcd);

int main(void)
{
  lcd.clear(0U);
  lcd.color(RGB(0x80E0FFu));

  // micro.font(micro_5x6);
  // micro.font(system_5x7);
  // micro.font(standard_5x7);
  // micro.font(number_8x16);
  // micro.font(number_15x31);

  for (byte i = 32; i < 192; i++) {
    micro.symbol(i);
  }


  micro.at(0, 110);
  micro.printR("АБВГДЕЖЗИЙКЛМНОПРСТ");
  micro.at(0, 130);
  micro.printR("просто ghjGHJ  эюя");

  /*
      byte *ptr = (byte *)0x100;

      while (true) {
        for (char k = 9; k >= 0; k--) {
          byte *p = (byte *)ptr;
          for (byte i = 0; i < 15; i++) {
            micro.at(0, i * 10 + k);
            // micro.at(0, i * 18);
            micro.printHex((word)p);
            for (byte j = 0; j < 8; j++)
              micro.printHex(*p++);
          }
        }
        ptr += 8;
      }
    */
}
