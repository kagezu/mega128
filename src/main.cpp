#include "init.h"
#include "ST7735S.h"
#include "font.h"

ST7735S lcd;
Font micro(&lcd);

word offset = 0x1100;

void **createDim2(word countString, word countElement, char sizeElement)
{
  void **ptr = (void **)offset;
  offset += countString * 2;
  for (word i = 0; i < countString; i++)
  {
    ptr[i] = (void *)offset;
    offset += countElement * sizeElement;
  }
  return ptr;
}

int main(void)
{
  lcd.clear(0);

  micro.set_at(30, 50);
  micro.print("0xA000 ");
  micro.print(0xA000);

  micro.set_at(30, 60);
  micro.print("0x1100 ");
  micro.print(0x1100);

  //   byte x;
  //   while (true)
  //     lcd.demo(x++);

  //   word **memory = (word **)createDim2(160, 128, 2);
  }
