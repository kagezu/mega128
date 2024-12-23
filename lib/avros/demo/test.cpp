#include <core/core.h>
#include <display/display.h>
#include "text/text.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

Display lcd;
Text text(&lcd);

void func()__attribute__((noinline)); __attribute__((naked))
//  
// void func() __attribute__((noinline));
void func()
{
  byte *sp = (byte *)SP;
  // byte *sp = (byte *)(Core::current()->_context);
  text.printf(PSTR("func SP = %2x "), sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);
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

  byte *sp = (byte *)SP;
  text.printf(PSTR("main SP = %2x "), sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);

  func();
  Core::async(func);
  // Core::async(func);
  // Core::async(func);
  // Core::async(func);
  // Core::async(func);
  Core::await();
  // Core::async(func);
  // Core::await();

  text.printf(PSTR("\nmain = %2x\n"), main);
  text.printf(PSTR("func = %2x\n"), func);
  text.printf(PSTR("async = %2x\n"), Core::async);
  while (true);
  Core::await();
}
