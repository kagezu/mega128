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
  text.printf(PSTR("pid %2x SP = %2x \n"), Core::task.current()->_start, sp);
}

#pragma GCC optimize "O0"
int main()
{
  sei();

  text.font(standard_5x7);
  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  while (true) {
    // byte *sp = (byte *)SP;
    // text.printf(PSTR("\fpid %2x SP = %2x \n"), Core::task.current()->_start, sp);

    // func();
    Core::async(func);
    byte t1 = Core::task.count();
    Core::async(func);
    byte t2 = Core::task.count();
    Core::async(func);
    byte t3 = Core::task.count();
    Core::async(func);
    byte t4 = Core::task.count();
    Core::async(func);
    byte t5 = Core::task.count();
    // Core::async(func);
    // Core::await();
    // Core::async(func);
    // Core::async(func);
    // Core::async(func);
    // Core::async(func);
    // Core::await(2);

    cli();
    text.printf(PSTR("\ft = %x %x %x %x %x   \n"), t1, t2, t3, t4, t5);
    sei();

    // text.printf(PSTR("\nmain = %2x\n"), main);
    // text.printf(PSTR("func = %2x\n"), func);
    // text.printf(PSTR("async = %2x\n"), Core::async);

    // while (true);
  }

}
