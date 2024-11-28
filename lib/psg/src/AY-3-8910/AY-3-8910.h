#include "psg.config.h"

class AY_3_8910 {
public:

  byte mix = 56;

  AY_3_8910()
  {
    AY_INIT;

    write(6, 0); // шум
    write(7, mix);
    write(11, 0); //огибающая
    write(12, 16); //огибающая
    write(13, 0); //спад с удержанием
  }

  void write(byte address, byte data)
  {
    DDRD = 0xff;
    PORTD = address;
    AY_LATCH_ADR;
    AY_INACTIVE;
    PORTD = data;
    AY_WRITE;
    AY_INACTIVE;
  }

  byte volatile read(byte address)
  {
    PORTD = address;
    AY_LATCH_ADR;
    AY_INACTIVE;
    DDRD = 0;
    AY_READ;
    byte data = PIND;
    AY_INACTIVE;
    return data;
  }

  byte getKey()
  {
    return 0;
  }
};
