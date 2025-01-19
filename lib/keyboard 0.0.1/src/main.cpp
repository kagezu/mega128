#include "core.h"
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "keyboard.h"
#include "VS1053/VS1053.h"

Display lcd;
Keyboard key;
VS1053 midi;

// Функция, возвращающая количество свободного ОЗУ (RAM)
// Вся память под стек считается занятой
byte memory_free()
{
  return  100 - ((25 * memory.heap()) >> 9);
}

char piano[62];

void printKey(uint64_t x)
{
  piano[0] = ' ';
  piano[61] = 0;
  x >>= 4;
  for (byte i = 60; i; i--) {
    if (x & 1) piano[i] = '!';
    else if (i % 8) piano[i] = '.';
    else piano[i] = ',';
    x >>= 1;
  }
  lcd.font(&arial_14);
  lcd.printf(F("  %s\n"), piano);
}

uint16_t time = 0;
uint16_t cpu = 0;
uint16_t fps = 10;
uint16_t time2 = F_SCAN / fps;
const char *pgm_text;
byte vel;

int main()
{
  T0_DIV_1024;
  T0_CTC;
  timer0(F_SCAN);
  T0_COMPA_ON;

  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  midi.init();
  midi.pgm_change(0);
  sei();

#define AVERAGE_FACTOR  4

  while (true) {
    fps = ((fps << AVERAGE_FACTOR) - fps + F_SCAN / time2) >> AVERAGE_FACTOR;
    cpu = ((cpu << AVERAGE_FACTOR) - cpu + (time * F_SCAN) / 156) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(F("\fcpu %u%%\t mem %u%% fps %u  \n"), cpu, memory_free(), fps);

    printKey(*(uint64_t *)key.get_on());
    printKey(*(uint64_t *)key.get_off());
    lcd.printf(F("\n"));

    lcd.font(&standard_5x8);
    if (pgm_text != midi.get_pgm_text()) {
      lcd.print(F("                          \r"));
      pgm_text = midi.get_pgm_text();
    }

    lcd.printf(F("%u %S\n"), midi._pgm, pgm_text);
    lcd.printf(
      F("\n  master: -%2u.%u dB  \n"),
      midi.get_master() >> 1,
      midi.get_master() & 1 ? 5 : 0
    );
    lcd.printf(F("\tvelocity: %u   \n"), vel);

  }
}

void extra(byte i)
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
      midi.set_master(midi.get_master() + 1);
      break;
    case 8:
      midi.set_master(midi.get_master() - 1);
      break;
  }
}

ISR(TIMER0_COMPA_vect)
{
  key.tick();

  volatile byte mask = _BV(KEY_OFFSET);
  byte *on = key.get_on();      // Все нажатые клавиши
  byte *off = key.get_off();    // Все отжатые клавиши
  byte *last = key.get_last();  // Последнее состояние клавиши

  for (byte i = KEY_COUNT - 1; i < KEY_COUNT; i--) { // Порядок соответствующий сканированию
    if (*off & mask) { // Клавиша отпущена
      if (*last & mask) { // Ранее клавиша была нажата
        midi.note_off(KEY_FIRST + i, 0, key.velocity(i));
        *last ^= mask; // Новое состояние: отжата
      }
      key._timer[i] = 0;
      // key.clear_timer(i); // Сбрасываем счётчик
    }
    else {
      if (*on & mask) { // Клавиша нажата
        if (!(*last & mask)) { // Ранее клавиша была отпущена
          midi.note_on(KEY_FIRST + i, 0, vel = key.velocity(i)); // test
          *last |= mask; // Новое состояние: нажата
          extra(i); // test
        }
        key._timer[i] = 0;
        // key.clear_timer(i); // Сбрасываем счётчик
      }
      else // Клавиша не прижата к контактам
        // key.increment_timer(i);
        if (key._timer[i] + 1) key._timer[i]++;
    }
    mask <<= 1; // Переходим к следующей клавише
    if (!mask) {// Переходим к следующему байту
      mask = 1;
      on++;
      off++;
      last++;
    }
  }

  time = TCNT0;
  time2++;
}
