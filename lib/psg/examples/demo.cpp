#include "data.h"
#include "timer.h"
#include "AY-3-8910/AY-3-8910.h"
#include "display/display.h"

Display lcd;
AY_3_8910 psg;

unsigned int cb = 0;
unsigned char counter = 0;
#define DIV 2


void pseudoInterrupt()
{
  unsigned int adr = pgm_read_word(&strem[cb++]);
  byte d = pgm_read_word(&raw[adr++]);

  while (d != 15) {
    if (d >= 240) {
      psg.load(d & 15, pgm_read_byte(&raw[adr++]));
      break;
    }
    else {
      psg.load(d & 15, pgm_read_byte(&raw[adr++]));
      psg.load(d >> 4, pgm_read_byte(&raw[adr++]));
    }

    d = pgm_read_word(&raw[adr++]);
  }

  if (cb > 4454 - 725) {
    cb = 0;
    for (int i = 0; i < 14; i++) psg.load(i, 0);
    psg.load(7, 255);
  }
}

int main()
{
  T0_DIV_1024;
  T0_CTC;
  OCR0A = 155;
  T0_COMPA_ON;

  T1_DIV_1;
  T1_CTC;
  T1_OC1A_ON;
  OCR1A = 4;

  sei();

  byte x = 0;
  while (true) {
    lcd.demo(x++);
  }
}

ISR(TIMER0_COMPA_vect)
{
  if (counter++ == DIV) {
    pseudoInterrupt();
    counter = 1;
  }
}
