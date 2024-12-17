#include <core/core.h>
#include <display/display.h>
#include "text/text.h"
#include "font/arial_14.h"

// #pragma GCC optimize "O0"

Display lcd;
Text text(&lcd);

extern int __bss_end;
// Функция, возвращающая количество свободного ОЗУ (RAM)
word memoryFree()
{
  // word freeValue = ((word)&freeValue) - ((word)&__bss_end);
  // return  100 - ((25 * freeValue) >> 9);

  word freeValue;
  byte *x = (byte *)malloc(1);
  freeValue = ((word)&freeValue) - ((word)x);
  free(x);

  return  freeValue;
}

// __attribute__((naked))


// void func()__attribute__((noinline));
// __attribute__((noinline)) 
void func()
{
  // byte *sp = (byte *)(SP + 7);
  byte *sp = (byte *)Core::_adr.at(0);
  text.printf(PSTR("%x "), sp);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);
}

__attribute__((naked)) int main()
{
  // SP = 0x800;
  text.setInterline(3);
  text.font(arial_14);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  Core::test(func);

  func();

  byte *sp = (byte *)(SP + 1);
  text.printf(PSTR("%x "), sp);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);

  text.printf(PSTR("%2x\n"), func);
  text.printf(PSTR("%2x\n"), Core::test);
  text.printf(PSTR("%2x\n"), main);

  for (;;);
}

