#include <avr.h>
#include "shift.h"

#define KEYS_OFFSET         4
#define KEYS_SIZE           8
#define KEYS_COUNT          60
#define KEYS_LOAD_DELAY     2
#define KEYS_MAX_SPEED      16
#define KEYS_SPEED_FACTOR   0
#define KEYS_MAX_DELAY      ( KEYS_MAX_SPEED << KEYS_SPEED_FACTOR ) - 1

class Keyboard : public Shift {
private:
  byte _line;

public:
  byte _on[KEYS_SIZE];
  byte _off[KEYS_SIZE];
  byte _timer[KEYS_COUNT];
  byte _keys[KEYS_COUNT];

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
    read_bytes(on, KEYS_SIZE);
    _MMIO_BYTE(_port) &= ~_line;
    delayMicroseconds(KEYS_LOAD_DELAY);

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
          if (_timer[i] < 255) {
            _keys[i] = KEYS_MAX_SPEED - (_timer[i] >> KEYS_SPEED_FACTOR);
            _timer[i] = 255;
            key = i;
          }
        }
        else
          if (_timer[i] < KEYS_MAX_DELAY) _timer[i]++;

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
