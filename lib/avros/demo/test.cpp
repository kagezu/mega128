#include <core/core.h>
#include <display/display.h>
#include "text/text.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

Display lcd;
Text text(&lcd);

void func()
{
  byte *sp = (byte *)SP;
  text.printf(PSTR("SP = %2x \n"), sp);
}

void func(byte x)
{
  // byte *sp = (byte *)SP;
  text.printf(PSTR("X = %x \n"), x);
}

#pragma GCC optimize "O0"
int main()
{
  sei();

  text.font(standard_5x7);
  text.set_inter_line(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  while (true) {
    // byte *sp = (byte *)SP;

    // func();
    Core::async(func);
    volatile byte t1 = Core::count();
    Core::async(func, t1);
    volatile byte t2 = Core::count();
    Core::async(func, t2);
    volatile byte t3 = Core::count();
    Core::async(func, t1);
    volatile byte t4 = Core::count();
    Core::async(func, t1);
    volatile byte t5 = Core::count();
    Core::await();

    I_SAVE;
    text.printf(PSTR("\ft = %x %x %x %x %x   \n"), t1, t2, t3, t4, t5);
    I_REST;

    text.printf(PSTR("\nmain = %2x\n"), main);
    // text.printf(PSTR("func = %2x\n"), func);
    // text.printf(PSTR("async = %2x\n"), Core::async);

    // while (true);
  }

}
