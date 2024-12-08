#include <Arduino.h>
#include "shift.h"

#define KEYS_OFFSET 4
#define KEYS_SIZE   8
#define KEYS_COUNT  60

class Keyboard : public Shift {
public:
  byte _on[KEYS_SIZE];
  byte _off[KEYS_SIZE];
  byte _timer[KEYS_COUNT];
  byte _keys[KEYS_COUNT];

  byte _old[KEYS_SIZE];

public:
  Keyboard(byte port, byte ddr, byte pin, byte dat, byte clk, byte ld) :
    Shift(port, ddr, pin, dat, clk, ld)
  {}

public:
  void scanKey()
  {
    load();
    readBytes((byte *)&_on, sizeof(_on));
  }

  char tick()
  {
    byte mask = _BV(KEYS_OFFSET);
    byte *on = (byte *)&_on;
    byte *off = (byte *)&_off;

    load();
    readBytes(on, KEYS_SIZE);
    load();
    readBytes(off, KEYS_SIZE);

    for (char i = 0; i < KEYS_COUNT; i++) {
      if (*off & mask)
        if (_timer[i]) { _timer[i] = 0; _keys[i] = 0; }
        else
          if (*on & mask) {
            if (_timer[i] < 255) {
              _keys[i] = 16 - (_timer[i] >> 4);
              _timer[i] = 255;
            }
          }
          else
            if (_timer[i] < 254) _timer[i]++;

      mask <<= 1;
      if (!mask) { mask = 1; on++; off++; }
    }

    return 0;
  }

  byte getKeyVolume(byte key)
  {
    return _keys[key];
  }

  char getKey()
  {
    byte mask = _BV(KEYS_OFFSET);
    byte *on = _on;
    byte *old = _old;
    byte diff = *on ^ *old;

    for (char i = 0; i < KEYS_COUNT; i++) {
      if (diff & mask) {
        *old ^= mask;
        if (*on & mask) return i;
      }
      mask <<= 1;
      if (!mask) {
        mask = 1;
        diff = *++on ^ *++old;
      }
    }
    return -1;
  }

  /*
  char getKey()
  {
    uint64_t mask = 1;
    uint64_t diff = _on ^ _old;

    for (char i = 1; i < 65; i++) {
      if (diff & mask) {
        _old ^= mask;
        if (_on & mask) return i - 4;
      }
      mask <<= 1;
    }
    return 0;
  }
  */

};
