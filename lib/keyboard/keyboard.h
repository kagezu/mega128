#include <Arduino.h>
#include "shift.h"

class Keyboard : public Shift {
public:
  uint64_t _keys;
  uint64_t _old;

public:
  Keyboard(byte port, byte ddr, byte pin, byte dat, byte clk, byte ld) :
    Shift(port, ddr, pin, dat, clk, ld)
  {}

public:
  void scanKey()
  {
    load();
    readBytes((byte *)&_keys, sizeof(_keys));
  }

  char getKey()
  {
    uint64_t mask = 1;
    uint64_t diff = _keys ^ _old;

    for (char i = 1; i < 65; i++) {
      if (diff & mask) {
        _old ^= mask;
        if (_keys & mask) return i - 4;
      }
      mask <<= 1;
    }
    return 0;
  }

};
