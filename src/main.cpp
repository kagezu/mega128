#include "init.h"
#include "ST7735S.h"
#include "font.h"

byte a[20][20];

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
  // byte **a = (unsigned char[5][5])0x1100;

  byte t = 1;
  for (byte y = 0; y < 20; y++)
    for (word x = 0; x < 20; x++)
      a[y][x] = t++;

  byte **p = (byte **)a;
  while (true)
  {
    lcd.clear(0);
    for (char i = 0; i < 19; i++)
    {
      micro.set_at(0, i * 8);
      micro.printHex((word)p);
      for (char j = 0; j < 4; j++)
        micro.printHex(p[i][j]);
      // micro.printHex(**p++);
    }

    for (word i = 0; i < 20000; i++)
      delayMicroseconds(50000);
  }

  //   byte x;
  //   while (true)
  //     lcd.demo(x++);

  //   word **memory = (word **)createDim2(160, 128, 2);
}
