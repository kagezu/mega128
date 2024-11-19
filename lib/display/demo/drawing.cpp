#include "screen/screen.h"
#include "display/display.h"

XPage page0(0, XMEM_60K, 0x1000);
XPage page1(0x0100, XMEM_60K, 0x1000);
Screen scr(&page0);
Display lcd;

void draw()
{
  static RGB c = 0xffffffu;
  for (byte i = 0; i < 128; i++) {
    scr.color(c);
    scr.line(63, 79, i, 0);
    c.b++;
    c.g++;
    c.r++;
  }
  for (byte i = 0; i < 159; i++) {
    scr.color(c);
    scr.line(63, 79, 127, i);
    c.b++;
    c.g++;
    c.r--;
  }
  for (byte i = 127; i < 128; i--) {
    scr.color(c);
    scr.line(63, 79, i, 159);
    c.b++;
    c.g--;
    c.r++;
  }
  for (byte i = 159; i < 160; i--) {
    scr.color(c);
    scr.line(63, 79, 0, i);
    c.b--;
    c.g++;
    c.r--;
  }
}

void initTimer1(uint8_t f)
{
  TCCR1A = 0; // Normal
  TCCR1B |= 5; // N = 1024
  TCCR1B |= _BV(WGM12); // CTC режим для OCRnA
  OCR1AH = 84 / f;
  OCR1AL = 0;
  TIMSK |= _BV(OCIE1A);
  _SEI;
}

ISR(TIMER1_COMPA_vect)
{
  // cli();
  // if ((byte)random() > 127)  page0.use();
  // else page1.use();
  lcd.scanBitmap(scr.getSource());
  TIFR |= _BV(OCF1A);
  // sei();
}

int main()
{
  page0.use();
  scr.clear(RGB(0, 255, 0));
  initTimer1(30);

  // scr.fat(3);

  while (true) {
    draw();
  }
}
