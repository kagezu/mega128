#define RGB           RGB32
#define LCD_DRIVER    ST7735_SOFT
// #define LCD_FLIP      EX_X_Y | FLIP_X
#include "display.h"

Display lcd;

int main(void)
{
  lcd.init();
  uint8_t x;
  while (true) {
    lcd.demo(x++);
  }
  // while (1) {
  //   lcd.clear(RGB(100, 200, 255));
  //   lcd.clear(RGB(255, 100, 0));
  //   lcd.clear();
  // }
}
