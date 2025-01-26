#include "timer/timer.h"
#include "AY/AY.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "config.h"
#include "keyboard.h"


Display lcd;
AY psg;
Keyboard key;

// Функция, возвращающая количество свободного ОЗУ (RAM)
byte memoryFree()
{
  extern int __bss_end;
  uint16_t freeValue = ((uint16_t)&freeValue) - ((uint16_t)&__bss_end);
  return  100 - ((25 * freeValue) >> 9);
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
  lcd.printf(P("  %s\n"), piano);
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

  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

#define AVERAGE_FACTOR  2

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + 155 / time2) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(P("\fcpu %u%% | mem %u%% | fps %u \n\n"), time, memoryFree(), fps);

    lcd.font(&arial_14);
    lcd.printf(P("   Keyboard  60-keys\n"));

    printKey(key.get_on());
    printKey(key.get_off());
    lcd.printf(P("\n"));

    char v[] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    for (byte i = 0; i < 3; i++)
      lcd.printf(F("   %s         \n"), &v[32 - (psg.volume[i] << 1)]);
  }

}

ISR(TIMER0_COMPA_vect)
{
  keyboard.scan();
  keyboard.key_detect();
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
