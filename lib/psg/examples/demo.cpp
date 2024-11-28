#include "data.h"
#include "timer.h"
#include "AY-3-8910/AY-3-8910.h"
#include "display/display.h"
#include "text/text.h"
#include "font/system_5x7.h"

#define COMP(a,b) (a & b) == b

Display lcd;
AY_3_8910 psg;
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
    for (int i = 0; i < 14; i++) psg.write(i, 0);
    psg.write(7, 255);
  }
}

int main()
{
#ifdef __AVR_ATmega328P__
  T0_DIV_1024;
  T0_CTC;
  OCR0A = F_CPU / 1024 / 100 - 1; // 100 Hz
  T0_COMPA_ON;

  T1_DIV_1;
  T1_CTC;
  T1_OC1A_ON;
  OCR1A = F_CPU / 3579545;
#elif __AVR_ATmega128__
  T0_DIV_1024;
  T0_CTC;
  OCR0 = F_CPU / 1024 / 100 - 1; // 100 Hz
  T0_COMP_ON;

  T1_DIV_1;
  T1_CTC;
  T1_OC1A_ON;
  OCR1AH = 0;
  OCR1AL = F_CPU / 3579545 - 1;
#endif

  sei();

  text.font(system_5x7);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 0));

  byte oldKey = 0;

  while (true) {
    text.at(0, 0);
    text.printR(PSTR("Регистры AY-3-8910"));
    text.printPstr(PSTR("R0: ")); text.printHex(psg.read(0)); text.printPstr(PSTR("  R1: ")); text.printHex(psg.read(1)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R2: ")); text.printHex(psg.read(2)); text.printPstr(PSTR("  R3: ")); text.printHex(psg.read(3)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R4: ")); text.printHex(psg.read(4)); text.printPstr(PSTR("  R5: ")); text.printHex(psg.read(5)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R6: ")); text.printHex(psg.read(6)); text.printPstr(PSTR("  R7: ")); text.printHex(psg.read(7)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R8: ")); text.printHex(psg.read(8)); text.printPstr(PSTR("  R9: ")); text.printHex(psg.read(9));
    text.printPstr(PSTR("  R10: ")); text.printHex(psg.read(10)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R11: ")); text.printHex(psg.read(11)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R12: ")); text.printHex(psg.read(12)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R13: ")); text.printHex(psg.read(13)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R14: ")); text.printHex(psg.read(14)); text.printR(PSTR(" "));
    text.printPstr(PSTR("R15: ")); text.printHex(psg.read(15)); text.printR(PSTR(" "));
    byte key = psg.getKey();
    text.printPstr(PSTR("Key: "));
    text.printHex(key);
    text.printR(PSTR(" "));

    if (COMP(key, 0x70) && !oldKey) {
      oldKey = key;
      cb -= 100;
    }
    if (COMP(key, 0x30) && !oldKey) {
      oldKey = key;
      cb += 100;
    }
    if (COMP(key, 0x08) && !oldKey) {
      oldKey = key;
      cb = 0;
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
