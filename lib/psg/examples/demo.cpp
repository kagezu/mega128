#include "data.h"
#include "timer.h"
#include "AY-3-8910/AY-3-8910.h"

AY_3_8910 psg;

unsigned int cb = 0;

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

  if (cb > 4454) {
    cb = 0;
    for (int i = 0; i < 14; i++) psg.load(i, 0);
    psg.load(7, 255);
    delay(1000);
  }
}

void setup()
{
  TCCR1A = 64;
  TCCR1B = 9;
  TCNT1 = 0;
  OCR1A = 4;
  DDRB |= B1;
}

void loop()
{
  delay(20);
  pseudoInterrupt();
}

/*
int main()
{
  init();
  while (true) {
    delay(20);
    pseudoInterrupt();
  }
}
*/
