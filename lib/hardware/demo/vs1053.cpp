#include "timer/timer.h"
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "keyboard/keyboard.h"
#include "midi/midi.h"

#define F_SCAN  300

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
  // lcd.font(&arial_14);
  lcd.font(&arial_14);
  lcd.printf(F("  %s\n"), piano);
}

uint16_t time = 0;
uint16_t fps = 10;
uint16_t time2 = F_SCAN;
uint16_t time3 = 0;

int main()
{
  T0_DIV_1024;
  T0_CTC;
  timer0(F_SCAN);
  T0_COMPA_ON;

  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  midi.init();
  sei();

  midi.pgm_change(1);


#define AVERAGE_FACTOR  2

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + F_SCAN / time2) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(F("\fcpu %u%%\t mem %u%% fps %u \n"), time, memoryFree(), fps);

    // lcd.font(&arial_14);
    // lcd.printf(F("   Keyboard  60-keys\n"));

    printKey(*(uint64_t *)key._on);
    printKey(*(uint64_t *)key._off);

    // lcd.font(&standard_5x8);
    // lcd.printf(F("MODE:   %2x \n"), midi.read_register(SCI_MODE));
    // lcd.printf(F("STATUS: %2x \n"), midi.read_register(SCI_STATUS));
    // lcd.printf(F("BASS:   %2x \n"), midi.read_register(SCI_BASS));
    // lcd.printf(F("CLOCKF: %2x \n"), midi.read_register(SCI_CLOCKF));
    // lcd.printf(F("VOL:    %2x \n"), midi.read_register(SCI_VOL));

    // for (int m = 0; m < 16; m++) {
    //   for (int n = 50; n < 90; n++) {
    //     midi.note_on(m, n, 90);
    //     delay_ms(100);
    //     midi.note_off(m, n, 0);
    //     delay_ms(100);
    //   }
    // }

    time3++;
    cli();
    midi.pgm_change((time3 >> 7) & 127);
    sei();
  }
}


ISR(TIMER0_COMPA_vect)
{
  char  k = key.tick();

  if (k + 1) {
    if (key._keys[(byte)k])
      midi.note_on(95 - k, 0, key._keys[(byte)k] << 2);
    else
      midi.note_off(95 - k, 0);
  }

  time = TCNT0;
  time2++;
}
