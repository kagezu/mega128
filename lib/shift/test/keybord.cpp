#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"
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

  text.font(micro_5x6);
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
          psg.note(0);
        }
        else
          if (x & in) // новая нота
          {
            active = x; // заменяем
            psg.note(x);
          }

      }

    // lcd.clear(RGB(0, 0, 64));
    text.printf(PSTR("\f\tКлавиатура 16-клавиш\n"));
    text.printf(PSTR("Key in: %2x    \n"), in);
    text.printf(PSTR("Key old: %2x    \n"), old);
    text.printf(PSTR("Key active: %2x   \n"), active);
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




