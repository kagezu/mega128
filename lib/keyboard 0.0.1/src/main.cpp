#include "timer/timer.h"
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "keyboard.h"
#include "midi/midi.h"

Display lcd;
KEYBOARD(key, D, PD3, PD4, PD5, PD0);
VS1053 midi;

// Функция, возвращающая количество свободного ОЗУ (RAM)
byte memory_free()
{
  extern int __bss_end;
  uint16_t free = RAMEND - (uint16_t)&__bss_end;
  return  100 - ((25 * free) >> 9);
}

char piano[62];

void printKey(uint64_t x)
{
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

uint16_t time = 0;
uint16_t cpu = 0;
uint16_t fps = 10;
uint16_t time2 = F_SCAN;
const char *pgm_text;

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
  midi.pgm_change(0);
  sei();

#define AVERAGE_FACTOR  4

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + F_SCAN / time2) >> AVERAGE_FACTOR;
    cpu = ((cpu << AVERAGE_FACTOR) - cpu + (time * F_SCAN) / 156) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(F("\fcpu %u%%\t mem %u%% fps %u \n"), cpu, memory_free(), fps);

    printKey(key.get_on());
    printKey(key.get_off());
    lcd.printf(F("\n"));

    lcd.font(&standard_5x8);
    if (pgm_text != midi.get_pgm_text()) {
      lcd.print(F("                          \r"));
      pgm_text = midi.get_pgm_text();
    }

    lcd.print(midi.get_pgm_text());
    lcd.printf(
      F("\n\n  master: -%2u  \n  left: %i \n  right: %i \n"),
      midi.get_master() >> 1,
      -(midi.get_left() >> 1),
      -(midi.get_right()) >> 1
    );

  }
}


ISR(TIMER0_COMPA_vect)
{
  key.tick();

  time = TCNT0;
  time2++;
}
