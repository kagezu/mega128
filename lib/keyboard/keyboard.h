#include <Arduino.h>
#include "shift.h"

#define KEYS_OFFSET 4
#define KEYS_SIZE   8
#define KEYS_COUNT  60

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
  void scanKey()
  {
    load();
    readBytes(_on, sizeof(_on));
  }

  char tick()
  {
    byte mask = _BV(KEYS_OFFSET);
    byte *on = _on;
    byte *off = _off;

    _MMIO_BYTE(_port) |= _line;
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    load();
    readBytes(on, KEYS_SIZE);

    _MMIO_BYTE(_port) &= ~_line;
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    load();
    readBytes(off, KEYS_SIZE);

    for (byte i = 0; i < KEYS_COUNT; i++) {
      if (*off & mask) {
        if (_timer[i]) { _timer[i] = 0; _keys[i] = 0; }
      }
      else
        if (*on & mask) {
          if (_timer[i] < 255) {
            _keys[i] = 16 - (_timer[i] >> 4);
            if (_keys[i] < 6) _keys[i] = 6;
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

  word getKeyVolume()
  {
    for (byte i = 0; i < KEYS_COUNT; i++)
      if (_keys[i]) {
        byte t = _keys[i];
        _keys[i] = 0;
        return (t << 8) | i;
      }
    return 0;
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

};
