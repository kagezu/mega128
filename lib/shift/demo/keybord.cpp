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
  OCR0A = F_CPU / 1024 / 64 - 1; // 64 Hz 
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
    do {
      n = key.getKey();
      if (n) psg.note(n - 1);
    } while (n);

    text.font(arial_14);
    text.printf(PSTR("\f \tKeyboard  60-keys\n\n"));
    text.font(micro_5x6);
    text.printf(PSTR("SCAN: %8x \n\n"), key._keys);

    uint64_t x = key._keys;
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
      text.printf(PSTR("%x \t%s          \n"),psg.v[i], &v[16 - psg.volume[i]]);
  }
}
// DIV = 1 ~ 1/8
#define DIV 6
byte counter = 0;

ISR(TIMER0_COMPA_vect)
{
  if (counter++ == DIV) {
    psg.tick();
    counter = 1;
  }
}
