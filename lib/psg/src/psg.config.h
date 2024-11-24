#include <Arduino.h>

#define  AY_BC1       _BV(PB0)
#define  AY_BDIR      _BV(PB2)
#define  AY_PORT_CTRL PORTB


#define AY_INACTIVE   AY_PORT_CTRL &= ~(AY_BC1 | AY_BDIR);
#define AY_READ       AY_PORT_CTRL |= AY_BC1;
#define AY_WRITE      AY_PORT_CTRL |= AY_BDIR;
#define AY_LATCH_ADR  AY_PORT_CTRL |= AY_BC1 | AY_BDIR;

#define  AY_INIT                          \
  DDRB |= AY_BDIR | AY_BC1;    \
  PORTB &= ~(AY_BDIR | AY_BC1);           \
  DDRD = 0xFF;

//2 ~ 2,6(6) MHz
//3 ~ 2 MHz
//4 ~ 1.6 MHz
//5 ~ 1.3(3) MHz
//6 ~ 1.1415 MHz
//7 ~ 1 MHz
