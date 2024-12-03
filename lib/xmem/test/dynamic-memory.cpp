#include "dmem/dyn-memory.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"

Display lcd;
Text text(&lcd);
uint16_t error = 0;

int main()
{
  lcd.clear(RGB(0x40u));
  lcd.background(RGB(0x40u));
  lcd.color(RGB(0xffff00u));
  text.font(micro_5x6);

  for (;;) {
    text.at(0, 0);
    text.printR(PSTR("Тестирование выделения памяти"));
    text.printR(PSTR(" "));
    text.printR(PSTR(" "));
    text.print("Total errors: ");
    text.print(error);

  }
}

