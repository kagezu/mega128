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

//


// void func()__attribute__((noinline)); __attribute__((naked))
//  
// void func() __attribute__((noinline));
void func()
{
  byte *sp = (byte *)(SP + 1);
  text.printf(PSTR("%2x "), sp);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);
}

// void init9() GCC_INIT(9) GCC_NAKED;
// void init9()
// {
  // core.create();
// }

// int main() GCC_NAKED;
int main()
{
  Core::init();
  Core::current()->load();

  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  asm volatile ("nop");
  
  sei();

  text.setInterline(3);
  text.font(arial_14);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  // Core::async(func);
  // Core::async(func);
  // Core::await();
  func();

  // byte *sp = (byte *)(core.current()->_sp + 1);
  byte *sp = (byte *)(SP + 1);
  text.printf(PSTR("%2x "), sp);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x."), *sp++);
  text.printf(PSTR("%x\n"), *sp++);

  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x."), *sp++);
  // text.printf(PSTR("%x\n"), *sp++);


  text.printf(PSTR("\nmain = %2x\n"), main);
  text.printf(PSTR("func = %2x\n"), func);
  for (;;);
  Core::await();
}
