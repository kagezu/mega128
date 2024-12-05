#include "data.h"
#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"

#define COMP(a,b) (a & b) == b

Display lcd;
AY psg;
Text text(&lcd);

unsigned int cb = 0;
unsigned char counter = 0;
#define DIV 2


void pseudoInterrupt()
{
  unsigned int adr = pgm_read_word(&strem[cb++]);
  byte d = pgm_read_word(&raw[adr++]);

  while (d != 15) {
    if (d >= 240) {
      psg.write(d & 15, pgm_read_byte(&raw[adr++]));
      break;
    }
    else {
      psg.write(d & 15, pgm_read_byte(&raw[adr++]));
      psg.write(d >> 4, pgm_read_byte(&raw[adr++]));
    }

    d = pgm_read_word(&raw[adr++]);
  }

  if (cb > 4454 - 1125) {
    cb = 0;
    // for (int i = 0; i < 13; i++) psg.write(i, 0);
    // psg.write(7, 255);
  }
}

int main()
{
#ifdef __AVR_ATmega328P__
  T0_DIV_1024;
  T0_CTC;
  OCR0A = F_CPU / 1024 / 100 - 1; // 100 Hz
  T0_COMPA_ON;
#elif __AVR_ATmega128__
  T0_DIV_1024;
  T0_CTC;
  OCR0 = F_CPU / 1024 / 100 - 1; // 100 Hz
  T0_COMP_ON;
#endif

  sei();

  text.font(micro_5x6);
  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 0));

  byte oldKey = 0;
  byte key;

  while (true) {
    text.printf(PSTR("Регистры\tAY-3-8910\n"));
    text.printf(PSTR("R0:\t%x\tR1:\t%x\n"), psg.read(0), psg.read(1));
    text.printf(PSTR("R2:\t%x\tR3:\t%x\n"), psg.read(2), psg.read(3));
    text.printf(PSTR("R4:\t%x\tR5:\t%x\n"), psg.read(4), psg.read(5));
    text.printf(PSTR("R6:\t%x\tR7:\t%x\n"), psg.read(6), psg.read(7));
    text.printf(PSTR("R10:\t%x\tR11:\t%x  R12:\t%x\n"), psg.read(010), psg.read(011), psg.read(012));
    text.printf(PSTR("R13:\t%x\tR14:\t%x\n"), psg.read(013), psg.read(014));
    text.printf(PSTR("R15:\t%x\n"), psg.read(015));
    text.printf(PSTR("R16\t%x\tR17:\t%x\n"), psg.read(016), psg.read(017));
    key = psg.getKey();
    text.printf(PSTR("Key:\t%x\t%u     \f"), key, key);

    if (key == 0x04 && !oldKey) {
      oldKey = key;
      cb -= 100;
    }
    if (key == 0x14 && !oldKey) {
      oldKey = key;
      cb += 100;
    }
    if (key == 0x1b && !oldKey) {
      oldKey = key;
      cb = 0;
    }
    if (key == 0xff && !oldKey) {
      // return 0;
    }

    if (key == 0) oldKey = 0;

  }

  /*
    byte x = 0;
    while (true) {
      lcd.demo(x++);
    }
  */

}

#ifdef __AVR_ATmega328P__
ISR(TIMER0_COMPA_vect)
#elif __AVR_ATmega128__
ISR(TIMER0_COMP_vect)
#endif
{
  if (counter++ == DIV) {
    pseudoInterrupt();
    counter = 1;
  }
}