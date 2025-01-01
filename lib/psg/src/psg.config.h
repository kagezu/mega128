#include <Arduino.h>
#include <timer.h>

#define   AY_PORT    PORTD
#define   AY_PIN     PIND
#define   AY_IN      DDRD = 0x00;
#define   AY_OUT     DDRD = 0xff;

// Для BDIR и BC1 используется один порт
#if __AVR_ATmega328P__

#define   AY_BC1    _BV(PB0)
#define   AY_BDIR   _BV(PB2)
#define   AY_FUN    PORTB
#define   AY_ACT    DDRB |= AY_BC1 |  AY_BDIR;

#define   AY_CLOCK_ON \
  T1_DIV_1;\
  T1_CTC;\
  T1_OC1A_ON;\
  OCR1A = 4; // 1.6 MHz (f_cpu = 16 MHz)

#elif __AVR_ATmega128__

#define   AY_BC1    _BV(PB0)
#define   AY_BDIR   _BV(PB2)
#define   AY_FUN    PORTB
#define   AY_ACT   DDRB |= AY_BC1 | AY_BDIR ;

#define   AY_CLOCK_ON \
  T1_DIV_1;\
  T1_CTC;\
  T1_OC1A_ON;\
  OCR1AH = 0;\
  OCR1AL = 5; // 1.8432 MHz (f_cpu = 22 MHz)

#endif
