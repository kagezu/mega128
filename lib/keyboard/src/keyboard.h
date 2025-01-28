#pragma once
#include "type/buffer.h"

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

  uint64_t get_on() { return *(uint64_t *)_on; }
  uint64_t get_off() { return *(uint64_t *)_off; }

  void scan()
  {
    load();
    CS0(CLR);
    read(_on, KEY_SIZE);

    load();
    read(_off, KEY_SIZE);
    CS0(SET);
  }

  void key_detect()
  {
    uint8_t mask = _BV(KEY_OFFSET);
    uint8_t *on = _on;      // Все нажатые клавиши
    uint8_t *off = _off;    // Все отжатые клавиши
    uint8_t *last = _last;  // Последнее состояние клавиши
    uint8_t *timer = _timer;
    Key key;

    for (char i = KEY_COUNT - 1; i >= 0; i--) { // Порядок соответствующий сканированию
      if (*off & mask) { // Клавиша отпущена
        if (*last & mask) { // Ранее клавиша была нажата
          key.num = i | KEY_OFF_PREFIX;
          // key.value = get_velocity(*timer);
          key.value = *timer;
          KeyBuffer.write(key);
          *last ^= mask; // Новое состояние: отжата
        }
        *timer = 0;
      }
      else {
        if (*on & mask) { // Клавиша нажата
          if (!(*last & mask)) { // Ранее клавиша была отпущена
            key.num = i;
            // key.value = get_velocity(*timer);
            key.value = *timer;
            KeyBuffer.write(key);
            *last |= mask; // Новое состояние: нажата
          }
          // else {
          //   key.num = i;
          //   key.value = *timer;
          //   KeyBuffer.write(key);
          // }
          *timer = 0;
        }
        else // Клавиша не прижата к контактам
          if (*timer + 1) (*timer)++;
      }
      // Переходим к следующей клавише
      timer++;
      mask <<= 1;
      // Переходим к следующему байту
      if (!mask) {
        mask = 1;
        on++;
        off++;
        last++;
      }
    }
  }

private:
  uint8_t _on[KEY_SIZE] = {};    // Все нажатые клавиши
  uint8_t _off[KEY_SIZE] = {};   // Все отжатые клавиши
  uint8_t _last[KEY_SIZE] = {};  // Последнее состояние клавиши
  uint8_t _timer[KEY_COUNT] = {};

  void load() { LD(CLR); LD(SET); }
  void read(uint8_t *buffer, uint8_t length)
  {
    uint8_t data = 0;
    while (length--) {
      uint8_t i = 8;
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
