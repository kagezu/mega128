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


// Функция, возвращающая количество свободного ОЗУ (RAM)
byte memoryFree()
{
  extern int __bss_end;
  word freeValue = ((word)&freeValue) - ((word)&__bss_end);
  return  100 - ((25 * freeValue) >> 9);
}


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
  text.printf(PSTR("  %s\n"), piano);
}

byte time, time2, fps;

int main()
{
  T0_DIV_1024;
  T0_CTC;
  byte div = 100; // 16 Mhz -> 155 Hz
  OCR0A = div;
  T0_COMPA_ON;
  sei();

  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

#define AVERAGE_FACTOR  2

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + 155 / time2) >> AVERAGE_FACTOR;
    time2 = 0;

    text.font(standard_5x7);
    text.printf(PSTR("\fcpu %u%% | mem %u%% | fps %u \n\n"), time, memoryFree(), fps);

    text.font(arial_14);
    text.printf(PSTR("   Keyboard  60-keys\n"));

    printKey(*(uint64_t *)key._on);
    printKey(*(uint64_t *)key._off);

    char v[] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    for (byte i = 0; i < 3; i++)
      text.printf(PSTR("   %s         \n"), &v[32 - (psg.volume[i] << 1)]);
  }

}

ISR(TIMER0_COMPA_vect)
{
  char k = key.tick();
  if (k + 1) {
    if (key._keys[(byte)k]) {
      psg.div = 16;
      psg.note(k, key._keys[(byte)k]);
    }
    else psg.div = 8;
  }
  psg.tick();
  time = TCNT0;
  time2++;
}
