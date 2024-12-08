#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"
#include "font/arial_14.h"
#include "keyboard.h"

Display lcd;
AY psg;
Text text(&lcd);
Keyboard key(
  _SFR_MEM_ADDR(PORTC),
  _SFR_MEM_ADDR(DDRC),
  _SFR_MEM_ADDR(PINC),
  PC0,
  PC1,
  PC2
);

int main()
{
  T0_DIV_1024;
  T0_CTC;
  OCR0A = F_CPU / 1024 / 1000; // 16 Mhz -> 976 Hz 
  T0_COMPA_ON;
  sei();

  text.font(micro_5x6);
  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 0));


  while (true) {

    char n;
    key.scanKey();
    while (n >= 0) {
      psg.note(n);
      n = key.getKey();
    }

    text.font(arial_14);
    text.printf(PSTR("\f \tKeyboard  60-keys\n\n"));
    text.font(micro_5x6);
    text.printf(PSTR("SCAN: %8x \n\n"), *(uint64_t *)key._on);

    uint64_t x = *(uint64_t *)key._on;
    char piano[62];
    piano[0] = ' ';
    piano[61] = 0;
    x >>= 4;
    for (byte i = 60; i; i--) {
      if (x & 1) piano[i] = '!';
      else if (i % 8) piano[i] = '.';
      else piano[i] = ',';
      x >>= 1;
    }

    text.font(arial_14);
    text.printf(PSTR("%s\n"), piano);
    char v[] = "!!!!!!!!!!!!!!!!";
    for (byte i = 0; i < 3; i++)
      text.printf(PSTR("\t%s          \n"), &v[16 - psg.volume[i]]);
  }
}
// DIV = 12 ~ 1/8,  25 ~ 1/4,  50 ~ 1/2,  100 ~ 1 
#define DIV 100
byte counter = 0;

ISR(TIMER0_COMPA_vect)
{
  if (counter++ == DIV) {
    psg.tick();
    counter = 1;
  }
}
