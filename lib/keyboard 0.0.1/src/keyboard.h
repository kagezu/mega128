#pragma once
#include "config.h"
#include "VS1053/VS1053.h"

extern VS1053 midi;

class Keyboard {
private:
  byte _on[KEY_SIZE] = {};
  byte _off[KEY_SIZE] = {};
  byte _last[KEY_SIZE] = {};
  byte _timer[KEY_COUNT] = {};

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
    CS0(CLR);
    read(on, KEY_SIZE);

    load();
    read(off, KEY_SIZE);
    CS0(SET);

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
  void load() { LD(CLR); LD(SET); }

  void read(byte *buffer, byte length)
  {
    byte data;
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
