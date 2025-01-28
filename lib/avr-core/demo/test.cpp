#include <async/core.h>
#include <display.h>
#include "font/arial_14.h"
#include "font/standard_5x8.h"

Display lcd;

void func()
{
  uint8_t *sp = (uint8_t *)SP;
  lcd.printf(F("SP = %2x \n"), sp);
}

void func(uint8_t x)
{
  // uint8_t *sp = (uint8_t *)SP;
  lcd.printf(F("X = %x \n"), x);
}

#pragma GCC optimize "O0"
int main()
{
  sei();

  lcd.font(&standard_5x8);
  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  while (true) {
    // uint8_t *sp = (uint8_t *)SP;

    // func();
    Core::async(func);
    volatile uint8_t t1 = Core::count();
    Core::async(func, t1);
    volatile uint8_t t2 = Core::count();
    Core::async(func, t2);
    volatile uint8_t t3 = Core::count();
    Core::async(func, t1);
    volatile uint8_t t4 = Core::count();
    Core::async(func, t1);
    volatile uint8_t t5 = Core::count();
    Core::await();

    I_SAVE;
    lcd.printf(F("\ft = %x %x %x %x %x   \n"), t1, t2, t3, t4, t5);
    I_REST;

    lcd.printf(F("\nmain = %2x\n"), main);
    // lcd.printf(F("func = %2x\n"), func);
    // lcd.printf(F("async = %2x\n"), Core::async);

    // while (true);
  }

}
