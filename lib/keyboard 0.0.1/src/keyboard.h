#pragma once
#include "config.h"

class Keyboard {

public:
  Keyboard()
  {
    CS0(OUT);
    CS1(OUT);
    QH(IN);
    CLK(OUT);
    LD(OUT);

    CS0(SET);
    CS1(SET);
    CLK(CLR);
    LD(SET);
  }

  void clear_timer(byte i) { _timer[i] = 0; }
  void increment_timer(byte i) { if (_timer[i] + 1) _timer[i]++; }

  byte *get_on() { return _on; }
  byte *get_off() { return _off; }
  byte *get_last() { return _off; }

  void tick()
  {
    load();
    CS0(CLR);
    read(_on, KEY_SIZE);

    load();
    read(_off, KEY_SIZE);
    CS0(SET);
  }

  byte velocity(byte i)
  {
    uint16_t speed = (KEY_MAX_VELOCITY << KEY_FACTOR) / _timer[i];
    return speed > KEY_MAX_VELOCITY ? KEY_MAX_VELOCITY : speed;
  }

  // private:
  byte _on[KEY_SIZE] = {};    // Все нажатые клавиши
  byte _off[KEY_SIZE] = {};   // Все отжатые клавиши
  byte _last[KEY_SIZE] = {};  // Последнее состояние клавиши
  byte _timer[KEY_COUNT] = {};

private:

  void load() { LD(CLR); LD(SET); }
  void read(byte *buffer, byte length)
  {
    byte data = 0;
    while (length--) {
      byte i = 8;
      while (i--) {
        data >>= 1;
        if (QH(GET)) data |= 0x80;
        CLK(SET);
        CLK(CLR);
      }
      *buffer++ = data;
    }
  }
};
