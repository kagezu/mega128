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
  // byte *sp = (byte *)Core::tasks.head()->_sp;
  text.printf(PSTR("pid %2x SP = %2x \n"), Core::task.curent()->start, sp);
}

GCC_INIT(7)
{
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);
  // asm volatile ("nop"::);

}

// int main();
int GCC_NAKED main()
{
  Core::init();
  // Core::task.create();
  // Core::task.load();

  sei();

  text.font(standard_5x7);
  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  while (true) {

  byte *sp = (byte *)SP;
  // byte *sp = (byte *)Core::tasks.head()->_sp;
  text.printf(PSTR("\fpid %2x SP = %2x \n"), Core::task.curent()->start, sp);

  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);
  asm volatile ("nop"::);

  // func();
  Core::async(func);
  Core::async(func);
  Core::async(func);
  Core::async(func);
  Core::async(func);
  // Core::await();
  // Core::async(func);
  // Core::await(6);

  text.printf(PSTR("\nmain = %2x\n"), main);
  text.printf(PSTR("func = %2x\n"), func);
  text.printf(PSTR("async = %2x\n"), Core::async);

  // while (true);
  }

}
