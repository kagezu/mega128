#include "psg.config.h"

class AY_3_8910 {
public:

  byte mix = 56;

  AY_3_8910()
  {
    AY_INIT;

    load(6, 0); // шум
    load(7, mix);
    load(11, 0); //огибающая
    load(12, 16); //огибающая
    load(13, 0); //спад с удержанием
  }

  void load(byte address, byte data)
  {
    PORTD = address;
    PORTB |= AY_BDIR | AY_BC1; // fix address
    PORTB ^= AY_BDIR | AY_BC1; // inactive
    PORTD = data;
    PORTB |= AY_BDIR; // write data
    PORTB ^= AY_BDIR; // inactive
  }
};
