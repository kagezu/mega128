#pragma once
#include "shift/shift.h"
#include <macros/helper.h>

#define F_SCAN              400
#define KEYS_OFFSET         4
#define KEYS_SIZE           8
#define KEYS_COUNT          60
#define KEYS_MAX_VELOCITY   127
#define KEYS_FACTOR         3

#define KEYBOARD(name, port, dat, clk, ld,  line)     \
Keyboard name ( _SFR_MEM_ADDR(PORT(port)),            \
                _SFR_MEM_ADDR(DDR(port)),             \
                _SFR_MEM_ADDR(PIN(port)),             \
                dat, clk, ld, line );

class Keyboard : public Shift {
private:
  byte _line;

public:
  byte _on[KEYS_SIZE];
  byte _off[KEYS_SIZE];
  byte _keys[KEYS_COUNT];
  int16_t _timer[KEYS_COUNT];

  byte _old[KEYS_SIZE];

public:
  Keyboard(byte port, byte ddr, byte pin, byte dat, byte clk, byte ld, byte line) :
    Shift(port, ddr, pin, dat, clk, ld), _line(_BV(line))
  {
    _MMIO_BYTE(port) |= _BV(line);
    _MMIO_BYTE(ddr) |= _BV(line);
  }

public:

  char tick()
  {
    byte mask = _BV(KEYS_OFFSET);
    byte *on = _on;
    byte *off = _off;
    char key = -1;

    load();
    _MMIO_BYTE(_port) &= ~_line;
    read_bytes(on, KEYS_SIZE);

    load();
    read_bytes(off, KEYS_SIZE);
    _MMIO_BYTE(_port) |= _line;

    for (byte i = 0; i < KEYS_COUNT; i++) {
      if (*off & mask) {
        _timer[i] = 0;
        if (_keys[i]) { _keys[i] = 0; key = i; }
      }
      else
        if (*on & mask) {
          if (_timer[i] > 0) {
            uint16_t speed = (KEYS_MAX_VELOCITY << KEYS_FACTOR) / _timer[i];
            _keys[i] = speed > KEYS_MAX_VELOCITY ? KEYS_MAX_VELOCITY : speed;
            _timer[i] = -1;
            key = i;
          }
        }
        else
          if (_timer[i] + 1) _timer[i]++;

      mask <<= 1;
      if (!mask) { mask = 1; on++; off++; }
    }

    return key;
  }

  char get_key()
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

};
