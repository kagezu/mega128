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

  uint16_t in = 0;
  uint16_t old = 0;
  uint16_t active = 0;

  while (true) {
    key.reset();
    key.readBytes((byte *)&in, sizeof(in));

    if (in != old) {
      uint16_t x = in ^ old;// разница
      old = in;
      if (x & active) // нота играла
      {
        active ^= x; // вычёркиваем
      }
      else
        if (x & in) // новая нота
        {
          active = x; // заменяем
          byte n = 0;
          for (byte i = 16; i; i--)
            if (x & 1) break;
            else { x >>= 1; n++; }
          psg.note(n);
        }
    }

    // lcd.clear(RGB(0, 0, 64));
    text.printf(PSTR("\f  Keyboard  16-keys\n"));
    text.printf(PSTR("Key in: %2x    \n"), in);
    text.printf(PSTR("Key active: %2x   \n\n"), active);
    word x = active;
    for (byte i = 16; i; i--) {
      if (x & 0x8000)
        text.printf(PSTR("#"));
      else
        text.printf(PSTR("_"));
      x <<= 1;
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




