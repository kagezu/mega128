#include "init.h"
#include "ST7735S.h"
#include "font.h"

ST7735S lcd;
Font micro(&lcd);

int main(void)
{
  // lcd.clear(0);
  // while (true)
  // {
  //   micro.print("0123456789 0123456789 ABC");
  //   micro.print("ABCDEFGHIJ-abcdefghij xfd");
  // }

  byte x;
  while (true)
    lcd.demo(x++);

  // uint32_t x;
  // lcd.clear(x++ + (x << 7) + (x << 14));
}
