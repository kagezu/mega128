#include "timer/timer.h"
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "keyboard/keyboard.h"
#include "midi/midi.h"

#define F_SCAN  200

Display lcd;
KEYBOARD(key, D, PD3, PD4, PD5, PD0);
MIDI midi;

// Функция, возвращающая количество свободного ОЗУ (RAM)
byte memoryFree()
{
  // extern int __bss_end;
  uint16_t freeValue = (RAMEND - (uint16_t)&freeValue);
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
  lcd.font(&arial_14);
  lcd.printf(F("  %s\n"), piano);
}

byte time = 0;
byte fps = 10;
byte time2 = F_SCAN;

int main()
{
  T0_DIV_1024;
  T0_CTC;
  timer0(F_SCAN);
  T0_COMPA_ON;
  midi.begin();
  sei();

  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

#define AVERAGE_FACTOR  2

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + F_SCAN / time2) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(F("\fcpu %u%% | mem %u%% | fps %u \n\n"), time, memoryFree(), fps);

    lcd.font(&arial_14);
    lcd.printf(F("   Keyboard  60-keys\n"));

    printKey(*(uint64_t *)key._on);
    printKey(*(uint64_t *)key._off);
  }

}

ISR(TIMER0_COMPA_vect)
{
  char k = key.tick();

  if (k + 1) {
    if (key._keys[(byte)k])
      midi.note_on(k);//, 0, key._keys[(byte)k] << 2);
    else
      midi.note_off(k);//, 0);
  }

  time = TCNT0;
  time2++;
}
