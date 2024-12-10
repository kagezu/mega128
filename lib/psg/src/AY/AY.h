#include "psg.config.h"

#define AY_MAX_FQ   100000

// +15
const uint16_t fdiv[] = {
  AY_MAX_FQ / 27,
  AY_MAX_FQ / 29,
  AY_MAX_FQ / 31,
  AY_MAX_FQ / 33,
  AY_MAX_FQ / 35,
  AY_MAX_FQ / 37,
  AY_MAX_FQ / 39,
  AY_MAX_FQ / 41,
  AY_MAX_FQ / 44,
  AY_MAX_FQ / 46,
  AY_MAX_FQ / 49,
  AY_MAX_FQ / 52,
  AY_MAX_FQ / 55,
  AY_MAX_FQ / 58,
  AY_MAX_FQ / 62,
  AY_MAX_FQ / 65, // C2
  AY_MAX_FQ / 69,
  AY_MAX_FQ / 73,
  AY_MAX_FQ / 78,
  AY_MAX_FQ / 82,
  AY_MAX_FQ / 87,
  AY_MAX_FQ / 92,
  AY_MAX_FQ / 98,
  AY_MAX_FQ / 104,
  AY_MAX_FQ / 110,
  AY_MAX_FQ / 117,
  AY_MAX_FQ / 123,
  AY_MAX_FQ / 131,
  AY_MAX_FQ / 139,
  AY_MAX_FQ / 147,
  AY_MAX_FQ / 156,
  AY_MAX_FQ / 165,
  AY_MAX_FQ / 175,
  AY_MAX_FQ / 185,
  AY_MAX_FQ / 196,
  AY_MAX_FQ / 208,
  AY_MAX_FQ / 220,
  AY_MAX_FQ / 233,
  AY_MAX_FQ / 247,
  AY_MAX_FQ / 262,
  AY_MAX_FQ / 277,
  AY_MAX_FQ / 294,
  AY_MAX_FQ / 311,
  AY_MAX_FQ / 330,
  AY_MAX_FQ / 349,
  AY_MAX_FQ / 370,
  AY_MAX_FQ / 392,
  AY_MAX_FQ / 415,
  AY_MAX_FQ / 440,
  AY_MAX_FQ / 466,
  AY_MAX_FQ / 494,
  AY_MAX_FQ / 523,
  AY_MAX_FQ / 554,
  AY_MAX_FQ / 587,
  AY_MAX_FQ / 622,
  AY_MAX_FQ / 659,
  AY_MAX_FQ / 698,
  AY_MAX_FQ / 740,
  AY_MAX_FQ / 784,
  AY_MAX_FQ / 831,
  AY_MAX_FQ / 880,
  AY_MAX_FQ / 932,
  AY_MAX_FQ / 988,
  AY_MAX_FQ / 1047,
  AY_MAX_FQ / 1109,
  AY_MAX_FQ / 1175,
  AY_MAX_FQ / 1245,
  AY_MAX_FQ / 1319,
  AY_MAX_FQ / 1397,
  AY_MAX_FQ / 1480,
  AY_MAX_FQ / 1568,
  AY_MAX_FQ / 1661,
  AY_MAX_FQ / 1760,
  AY_MAX_FQ / 1865,
  AY_MAX_FQ / 1976,
  AY_MAX_FQ / 2093,
  AY_MAX_FQ / 2217,
  AY_MAX_FQ / 2349,
  AY_MAX_FQ / 2489,
  AY_MAX_FQ / 2637,
  AY_MAX_FQ / 2794,
  AY_MAX_FQ / 2960,
  AY_MAX_FQ / 3136,
  AY_MAX_FQ / 3322,
  AY_MAX_FQ / 3520,
  AY_MAX_FQ / 3729,
  AY_MAX_FQ / 3951,
  AY_MAX_FQ / 4186
};

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

//спад с удержанием
#define _FALL_HOLD    0

#define _IOA          016 // R16
#define _IOB          017 // R17

class AY {
public:
  byte _mixio = _BV(_MIOB);

public:
  AY()
  {
    AY_IN;
    AY_ACT;
    AY_INACTIVE;
    AY_CLOCK_ON;

    write(_NOISE, 0); // шум
    write(_MIX, 070);
    write(_ENVL, 0); //огибающая
    write(_ENVH, 24); //огибающая
    write(_ENVC, 0);
    write(_AA, 0);
    write(_AB, 0);
    write(_AC, 0);
  }

  byte read(byte reg)
  {
    byte data;
    AY_OUT;
    AY_PORT = reg;
    AY_LATCH_ADR;
    AY_INACTIVE;
    AY_IN;
    AY_READ;
    delayMicroseconds(1);
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

  void writeW(byte reg, uint16_t data)
  {
    write(reg++, data);
    write(reg, data >> 8);
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

  char volume[3];

  void note(byte key, byte vol = 16)
  {
    uint16_t  f = fdiv[60 - key + 15];
    if (vol < 4) vol = 4;

    writeW(_TGA, f << 1);
    writeW(_TGB, f);
    writeW(_TGC, f >> 1);
    volume[0] = vol - 4;
    volume[1] = vol;
    volume[2] = vol - 2;
    // if (volume[0] < 0) volume[0] = 0;
    // if (volume[2] < 0) volume[2] = 0;
  }

  // Для полной ноты, один шаг = 0.1 с
  // DIV = 2 ~ 1/8,  4 ~ 1/4,  8 ~ 1/2,  16 ~ 1 
#define DIV 16
  byte counter = 0;

  void tick()
  {
    if (counter++ == DIV) {
      if (volume[0]) volume[0]--;
      if (volume[1]) volume[1]--;
      if (volume[2]) volume[2]--;

      write(_AA, volume[0]);
      write(_AB, volume[1]);
      write(_AC, volume[2]);

      counter = 1;
    }
  }
};
