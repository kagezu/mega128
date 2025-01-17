#pragma once
#include <core.h>
#include "shift/shift.h"
#include "VS1053/VS1053.h"

#define F_SCAN              400
#define KEY_FIRST           36
#define KEY_OFFSET          4
#define KEY_SIZE            8
#define KEY_COUNT           60
#define KEY_MAX_VELOCITY    127
#define KEY_FACTOR          3

#define KEY_OFF             0
#define KEY_ON              1

#define KEYBOARD(name, port, dat, clk, ld,  line)     \
Keyboard name ( _SFR_MEM_ADDR(PORT(port)),            \
                _SFR_MEM_ADDR(DDR(port)),             \
                _SFR_MEM_ADDR(PIN(port)),             \
                dat, clk, ld, line );

extern VS1053 midi;

class Keyboard : public Shift {
private:
  byte _line;
  byte _on[KEY_SIZE] = {};
  byte _off[KEY_SIZE] = {};
  byte _last[KEY_SIZE] = {};
  byte _timer[KEY_COUNT] = {};

public:
  Keyboard(byte port, byte ddr, byte pin, byte dat, byte clk, byte ld, byte line) :
    Shift(port, ddr, pin, dat, clk, ld), _line(_BV(line))
  {
    _MMIO_BYTE(port) |= _BV(line);
    _MMIO_BYTE(ddr) |= _BV(line);
  }

public:
  uint64_t get_on() { return *(uint64_t *)_on; }
  uint64_t get_off() { return *(uint64_t *)_off; }


  void tick()
  {
    byte mask = _BV(KEY_OFFSET);  // Смещение первой клавиши
    byte *on = _on;               // Все нажатые клавиши
    byte *off = _off;             // Все отжатые клавиши
    byte *last = _last;           // Последнее состояние клавиши

    load();
    _MMIO_BYTE(_port) &= ~_line;
    read_bytes(on, KEY_SIZE);

    load();
    read_bytes(off, KEY_SIZE);
    _MMIO_BYTE(_port) |= _line;

    for (byte i = KEY_COUNT - 1; i < KEY_COUNT; i--) {
      if (*off & mask) { // Клавиша отпущена
        if (*last & mask) { // Ранее клавиша была нажата
          midi.note_off(KEY_FIRST + i, 0, velocity(_timer[i]));
          *last ^= mask; // Новое состояние: отжата
        }
        _timer[i] = 0; // Сбрасываем счётчик
      }
      else
        if (*on & mask) { // Клавиша нажата
          if (!(*last & mask)) { // Ранее клавиша была отпущена
            midi.note_on(KEY_FIRST + i, 0, velocity(_timer[i]));
            *last |= mask; // Новое состояние: нажата
            extra(i);
          }
          _timer[i] = 0; // Сбрасываем счётчик
        }
        else // Клавиша не прижата к контактам
          if (_timer[i] + 1) _timer[i]++;

      mask <<= 1; // Переходим к следующей клавише
      if (!mask) {// Переходим к следующему байту
        mask = 1;
        on++;
        off++;
        last++;
      }
    }
  }

private:
  byte velocity(byte time)
  {
    uint16_t speed = (KEY_MAX_VELOCITY << KEY_FACTOR) / time;
    return speed > KEY_MAX_VELOCITY ? KEY_MAX_VELOCITY : speed;
  }

  void extra(char i)
  {
    switch (i) {
      case 1:
        if (midi._pgm > 0)
          midi.pgm_change(midi._pgm - 1);
        break;
      case 3:
        if (midi._pgm < 128)
          midi.pgm_change(midi._pgm + 1);
        break;
      case 6:
        midi.set_master(midi.get_master() + 2);
        break;
      case 8:
        midi.set_master(midi.get_master() - 2);
        break;
    }
  }
};
