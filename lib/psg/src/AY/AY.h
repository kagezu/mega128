#include "psg.config.h"

// Таблица функций ПГЗ
//  FUNCTION  |  BDIR  |  BC1  |
// ===============================
//  INACTIVE  |   0    |   0   |
//  READ      |   0    |   1   |
//  WRITE     |   1    |   0   |
//  LATCH ADR |   1    |   1   |

#define AY_INACTIVE   AY_FUN &= ~(AY_BC1 | AY_BDIR);
#define AY_READ       AY_FUN |= AY_BC1;
#define AY_WRITE      AY_FUN |= AY_BDIR;
#define AY_LATCH_ADR  AY_FUN |= AY_BC1 | AY_BDIR;

// Номера регистров в восьмеричном формате
#define _TGA          000 // R0
#define _TGAL         000 // R0
#define _TGAH         001 // R1

#define _TGB          002 // R2
#define _TGBL         002 // R2
#define _TGBH         003 // R3

#define _TGC          004 // R4
#define _TGCL         004 // R4
#define _TGCH         005 // R5

#define _NOISE        006 // R6

#define _MIX          007 // R7

#define _MTA          0
#define _MTB          1
#define _MTC          2
#define _MNA          3
#define _MNB          4
#define _MNC          5
#define _MIOA         6
#define _MIOB         7

#define _AA           010 // R10
#define _AB           011 // R11
#define _AC           012 // R12

#define _AL           0x0f
#define _AM           4

#define _ENV
#define _ENVL         013 // R13
#define _ENVH         014 // R14
#define _ENVC         015 // R15

#define _EHOLD        0
#define _EALT         1
#define _EATT         2
#define _ECONT        3

#define _IOA          016 // R16
#define _IOB          017 // R17

class AY {
public:
  uint8_t _mixio = _BV(_MIOB);

public:
  AY()
  {
    AY_IN;
    AY_ACT;
    AY_INACTIVE;
    AY_CLOCK_ON;
  }

  byte read(byte reg)
  {
    byte data, tick;
    AY_OUT;
    AY_PORT = reg;
    AY_LATCH_ADR;
    AY_INACTIVE;
    AY_IN;
    AY_READ;
    tick = (F_CPU / 2) / 2000000; // 2 МГц максимальная частота работы ПГЗ
    while (tick--) asm volatile ("nop");
    data = AY_PIN;
    AY_INACTIVE;
    return data;
  }

  void write(byte reg, byte data)
  {
    AY_OUT;
    AY_PORT = reg;
    AY_LATCH_ADR;
    AY_INACTIVE;
    if (reg == _MIX) data |= _mixio;
    AY_PORT = data;
    AY_WRITE;
    AY_INACTIVE;
    AY_IN;
  }

  byte getKey()
  {
    byte key = 0;
    cli();
    for (byte a = 0; a < 4; a++) {
      write(_IOB, (1 << a) ^ 0xff);
      byte d = (read(_IOA) ^ 0xff) >> 3;
      if (d & 0x1) key |= (4 - a) << 5;
      d &= 0x1e;
      if (d) {
        key |= (3 - a) << 3;
        if (d & 0x2)  key |= 1;
        if (d & 0x4)  key |= 2;
        if (d & 0x8)  key |= 3;
        if (d & 0x10) key |= 4;
      }
    }
    sei();
    return key;
  }
};
