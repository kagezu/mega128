#include <display.h>

Display lcd;

#define W (MAX_X + 1)
#define H (MAX_Y + 1)
#define W2 W >> 1
#define H2 H >> 1

int main(void)
{
  byte i;

  lcd.clear(0);
  lcd.init();

  RGB c = 0x000;
  while (true) {
    for (i = 0; i < W; i++) {
      lcd.color(c);
      lcd.line(W2, H2, i, 0);
      c.b(1);
      c.g(1);
      c.r(1);
      lcd.update();
    }
    for (i = 0; i < H; i++) {
      lcd.color(c);
      lcd.line(W2, H2, W - 1, i);
      c.b(1);
      c.g(1);
      c.r(-1);
      lcd.update();
    }
    for (i = W - 1; i > 0; i--) {
      lcd.color(c);
      lcd.line(W2, H2, i, H - 1);
      c.b(1);
      c.g(-1);
      c.r(1);
      lcd.update();
    }
    for (i = H - 1; i > 0; i--) {
      lcd.color(c);
      lcd.line(W2, H2, 0, i);
      c.b(1);
      c.g(1);
      c.r(1);
      lcd.update();
    }

    c.b(5);
    c.g(5);
    c.r(5);
  }
}
