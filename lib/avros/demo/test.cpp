#include <core/core.h>
#include <display/display.h>
#include "text/text.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

Display lcd;
Text text(&lcd);

void func()
{
  // sei();
  byte *sp = (byte *)SP;
  // byte *sp = (byte *)Core::current()->_sp;
  text.printf(PSTR("pid = %x SP = %2x \n"), Core::current()->_pid, sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x\n"), *sp++);
  // cli();
}

// int main() GCC_NAKED;
int main()
{
  Core::init();
  Core::current()->load();
  sei();

  text.setInterline(3);
  text.font(standard_5x7);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  while (true) {

    byte *sp = (byte *)SP;
    // byte *sp = (byte *)Core::current()->_sp;
    text.printf(PSTR("\fpid = %x SP = %2x \n"), Core::current()->_pid, sp++);
    // text.printf(PSTR("%x."), *sp++);
    // text.printf(PSTR("%x\n"), *sp++);

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
    // Core::async(func);
    // Core::async(func);
    // Core::await();
    // Core::async(func);
    // Core::await(6);

    text.printf(PSTR("\nmain = %2x\n"), main);
    text.printf(PSTR("func = %2x\n"), func);
    text.printf(PSTR("async = %2x\n"), Core::async);

  }
}
