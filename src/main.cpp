#include "init.h"
#include "ST7735S.h"
#include "font.h"
#include "xmem_page.h"

byte m[100];
ST7735S lcd;
Font micro(&lcd);
XmemPage page;

class App
{
public:
  App()
  {
    page.use();
    byte x;
    while (true)
      lcd.demo(x++);
  }
};

App app;

int main(void)
{
  lcd.clear(0);
  byte *ptr = (byte *)m;
  while (true)
  {
    for (char k = 7; k >= 0; k--)
    {
      byte *p = (byte *)ptr;
      for (char i = 0; i < 18; i++)
      {
        micro.set_at(0, i * 8 + k);
        micro.printHex((word)p);
        for (char j = 0; j < 4; j++)
          micro.printHex(*p++);
      }
    }
    ptr += 4;
  }
}
