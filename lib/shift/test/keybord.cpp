#include "timer.h"
#include "AY/AY.h"
#include "display/display.h"
#include "text/text.h"
#include "font/micro_5x6.h"

#define KEY_HQ    _BV(0)
#define KEY_CLK   _BV(1)
#define KEY_LD    _BV(2)
#define KEY_L0    _BV(3)
#define DIV 2

Display lcd;
AY psg;
Text text(&lcd);

void inits()
{
  DDRC |= KEY_LD | KEY_CLK | KEY_L0;
  DDRC &= ~KEY_HQ;
  PORTC &= ~KEY_CLK;
  PORTC |= KEY_LD;


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
    PORTC ^= KEY_LD;
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
    PORTC ^= KEY_LD;

    for (byte i = 16; i; i--) {
      in = in << 1;
      if (PINC & KEY_HQ) in++;

      PORTC ^= KEY_CLK;
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      PORTC ^= KEY_CLK;
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
    }

    if (in != old) {
      byte x = in ^ old;// разница
      old = in;

      if (x & active) // нота играла
      {
        active ^= x; // вычёркиваем
        // psg.note(0);
      }
      else
        if (x & in) // новая нота
        {
          active = x; // заменяем
          // psg.note(x);
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




