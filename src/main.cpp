#include <init.h>

ST7735S lcd;
Font micro(&lcd);

int main(void)
{
  byte x;
  while (true) {
    lcd.demo(x++);
  }

  lcd.clear(0);

  byte b = 0xee;
  word w = 0xff99;
  uint32_t u = 0xaabbccdd;
  // RGB rgb;


  micro.at(20, 10);
  micro.printPstr(PSTR("b: "));
  micro.printHex(b);

  micro.at(20, 20);
  micro.printPstr(PSTR("(w: "));
  micro.printHex(w);

  micro.at(20, 30);
  micro.printPstr(PSTR("u: "));
  micro.printHex(u);

  /*
  micro.at(20, 40);
  micro.printPstr(PSTR("RGB: "));
  micro.printHex((uint32_t)(RGB)u);

    micro.at(20, 50);
    micro.printPstr(PSTR("temp: "));
    micro.printHex((word)temp);

    micro.at(20, 60);
    micro.printPstr(PSTR("(word)a: "));
    micro.printHex((word)&a);

    micro.at(20, 70);
    micro.printPstr(PSTR("a[][]: "));
    micro.printHex((word)a);
  */

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
   */
}
