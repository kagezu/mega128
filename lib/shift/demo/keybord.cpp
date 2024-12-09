#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"
#include "font/standard_5x7.h"
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
  PC2,
  PC3
);

void printKey(uint64_t x)
{
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
}

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

  word n, k;

  while (true) {

    n = key.getKeyVolume();
    while (n) {
      psg.note((byte)n, n >> 8);
      k = n;
      n = key.getKeyVolume();
    }

    text.font(arial_14);
    text.printf(PSTR("\f \tKeyboard  60-keys\n\n"));
    // text.font(micro_5x6);
    // text.printf(PSTR("SCAN: %8x \n\n"), *(uint64_t *)key._on);

    printKey(*(uint64_t *)key._on);
    printKey(*(uint64_t *)key._off);

    char v[] = "!!!!!!!!!!!!!!!!";
    for (byte i = 0; i < 3; i++)
      text.printf(PSTR("\t%s          \n"), &v[16 - psg.volume[i]]);

    text.font(standard_5x7);
    text.printf(PSTR("Key: %u %u     "), (byte)k + 1, k >> 8);
  }
}
// DIV = 12 ~ 1/8,  25 ~ 1/4,  50 ~ 1/2,  100 ~ 1 
#define DIV 50
byte counter = 0;

ISR(TIMER0_COMPA_vect)
{
  key.tick();
  if (counter++ == DIV) {
    psg.tick();
    counter = 1;
  }
}
