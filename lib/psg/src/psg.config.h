#include <Arduino.h>

#if __AVR_ATmega328P__

#define  AY_BC1       _BV(PB0)
#define  AY_BDIR      _BV(PB2)
#define  AY_PORT_CTRL PORTB

#define AY_INACTIVE   AY_PORT_CTRL &= ~(AY_BC1 | AY_BDIR);
#define AY_READ_C     AY_PORT_CTRL |= AY_BC1;
#define AY_WRITE      AY_PORT_CTRL |= AY_BDIR;
#define AY_LATCH_ADR  AY_PORT_CTRL |= AY_BC1 | AY_BDIR;

#define  AY_INIT \
  DDRB |= AY_BDIR | AY_BC1;\
  PORTB &= ~(AY_BDIR | AY_BC1);\
  DDRD = 0xFF;

#elif __AVR_ATmega128__

#define  AY_BC1       _BV(PB0)
#define  AY_BDIR      _BV(PB2)
#define  AY_PORT_CTRL PORTB

#define AY_INACTIVE   AY_PORT_CTRL &= ~(AY_BC1 | AY_BDIR);
#define AY_READ       AY_PORT_CTRL |= AY_BC1;
#define AY_WRITE      AY_PORT_CTRL |= AY_BDIR;
#define AY_LATCH_ADR  AY_PORT_CTRL |= AY_BC1 | AY_BDIR;

#define  AY_INIT \
  DDRB |= AY_BDIR | AY_BC1;\
  PORTB &= ~(AY_BDIR | AY_BC1);\
  DDRD = 0xFF;

#endif

#if F_CPU <= 4000000
#define AY_READ       AY_READ_C
#elif F_CPU <= 8000000
#define AY_READ       AY_READ_C asm volatile ("nop");asm volatile ("nop");
#elif F_CPU <= 12000000
#define AY_READ       AY_READ_C asm volatile ("nop");asm volatile ("nop");asm volatile ("nop");
#elif F_CPU <= 16000000
#define AY_READ       AY_READ_C \
asm volatile ("nop");asm volatile ("nop");\
asm volatile ("nop");asm volatile ("nop");
#else
#define AY_READ AY_READ_C \
asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");\
asm volatile ("nop"); asm volatile ("nop");asm volatile ("nop");
#endif
