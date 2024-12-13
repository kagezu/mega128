#include <type/buffer.h>
#include <display/display.h>
#include "text/text.h"
#include "font/arial_14.h"

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

  return  freeValue;
}

int main()
{
  Buffer<int, byte>  buf(100);

  text.setInterline(3);
  text.font(arial_14);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  text.printf(PSTR("mem %2u  \n\n"), memoryFree());


  text.printf(PSTR("mem %2u %u  \n\n"), memoryFree(), buf.read());

  for (;;);
  return 1;
}
