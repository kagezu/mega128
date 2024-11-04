#include <Arduino.h>
#include <x_page.h>
#include <ST7735S.h>
#include <font.h>

byte m[32];
ST7735S lcd;
Font micro(&lcd);
XPage page;
byte(&a)[5][5] = (byte(&)[5][5]) * page.create(25);
byte *temp = page.create(25);

int main(void)
{
  XPage::init();
  lcd.clear(0);

  word w;
  micro.at(20, 10);
  micro.printPstr(PSTR("stack: "));
  w = SP;
  micro.printHex(w);

  micro.at(20, 20);
  micro.printPstr(PSTR("(word)m: "));
  micro.printHex((word)m);

  micro.at(20, 30);
  micro.printPstr(PSTR("micro: "));
  micro.printHex((word)&micro);

  micro.at(20, 40);
  micro.printPstr(PSTR("page: "));
  micro.printHex((word)&page);

  micro.at(20, 50);
  micro.printPstr(PSTR("temp: "));
  micro.printHex((word)temp);

  micro.at(20, 60);
  micro.printPstr(PSTR("(word)a: "));
  micro.printHex((word)&a);

  micro.at(20, 70);
  micro.printPstr(PSTR("a[][]: "));
  micro.printHex((word)a);

  /*
   lcd.clear(0);
   byte *ptr = (byte *)m;
   while (true) {
     for (byte k = 7; k >= 0; k--) {
       byte *p = (byte *)ptr;
       for (byte i = 0; i < 18; i++) {
         micro.at(0, i * 8 + k);
         micro.printHex((word)p);
         for (byte j = 0; j < 4; j++)
           micro.printHex(*p++);
       }
     }
     ptr += 4;
   }

     byte x;
     while (true)
       lcd.demo(x++);
   */
}
