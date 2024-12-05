#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/arial_14.h"
#include "shift.h"

#define DIV 2

Display lcd;
AY psg;
Text text(&lcd);
Shift key(
  _SFR_MEM_ADDR(PORTC),
  _SFR_MEM_ADDR(DDRC),
  _SFR_MEM_ADDR(PINC),
  PC0,
  PC1,
  PC2
);

void inits()
{
#ifdef __AVR_ATmega328P__
  T0_DIV_1024;
  T0_CTC;
  OCR0A = F_CPU / 1024 / 100 - 1; // 100 Hz
  // T0_COMPA_ON;
#elif __AVR_ATmega128__
  T0_DIV_1024;
  T0_CTC;
  OCR0 = F_CPU / 1024 / 100 - 1; // 100 Hz
  T0_COMP_ON;
#endif
  sei();

  text.font(arial_14);
  text.setInterline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 0));
}

void interrupt()
{

}

int main()
{
  inits();

  uint8_t in[8];
  uint8_t old[8];
  uint8_t active[0];
  uint64_t &inX = (uint64_t &)in[0];
  uint64_t &oldX = (uint64_t &)old[0];
  uint64_t &activeX = (uint64_t &)active[0];

  while (true) {
    key.reset();
    key.readBytes(in, sizeof(in));

    if (inX != oldX) {
      uint64_t x = inX ^ oldX;// разница
      oldX = inX;
      if (x & activeX) // нота играла
      {
        activeX ^= x; // вычёркиваем
      }
      else
        if (x & inX) // новая нота
        {
          activeX = x; // заменяем
          byte n = 0;
          for (byte i = 64; i; i--)
            if (x & 1) break;
            else { x >>= 1; n++; }
          psg.note(n);
        }
    }

    // lcd.clear(RGB(0, 0, 64));
    text.printf(PSTR("\f  Keyboard  16-keys\n"));
    text.printf(PSTR("i: %x%x%x%x%x%x%x%x \n"), in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7]);
    text.printf(
      PSTR("a: %x%x%x%x%x%x%x%x \n\n"),
      active[0], active[1], active[2], active[3], active[4], active[5], active[6], active[7]);
    uint64_t x = activeX;
    for (byte i = 64; i; i--) {
      if (x & 1)
        text.printf(PSTR("!"));
      else
        text.printf(PSTR("."));
      x >>= 1;
    }
  }

}

#ifdef __AVR_ATmega328P__
ISR(TIMER0_COMPA_vect)
#elif __AVR_ATmega128__
ISR(TIMER0_COMP_vect)
#endif
{
  static unsigned char counter = 0;
  if (counter++ == DIV) {
    interrupt();
    counter = 1;
  }
}




