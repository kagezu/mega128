#include "display/display.h"

Display lcd;

int main(void)
{
  lcd.clear(0U);
  lcd.fat(5);
  lcd.circleFat(63, 79, 50);

}
