#include "display/display.h"

Display lcd;

int main(void)
{
  byte x;
  while (true) {
    lcd.demo(x++);
  }
}
