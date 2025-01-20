#include "core.h"
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "keyboard.h"
#include "VS1053/VS1053.h"

Display lcd;
Keyboard keyboard;
VS1053 midi;
Buffer<Key, byte> KeyBuffer(KEY_BUFFER_SIZE);

// Функция, возвращающая количество свободного ОЗУ (RAM)
// Вся память под стек считается занятой
byte memory_free()
{
  return  100 - ((25 * memory.heap()) >> 9);// Всего 2048 байт ОЗУ
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

void init_main()
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
  midi.pgm_change(54);
  sei();
}

int main()
{
  init_main();

  while (true) {
  #define AVERAGE_FACTOR  4
    fps = ((fps << AVERAGE_FACTOR) - fps + F_SCAN / time2) >> AVERAGE_FACTOR;
    cpu = ((cpu << AVERAGE_FACTOR) - cpu + (time * F_SCAN) / 156) >> AVERAGE_FACTOR;
    time2 = 0;

    lcd.font(&standard_5x8);
    lcd.printf(F("\fcpu %u%%\t mem %u%% fps %u  \n"), cpu, memory_free(), fps);

    printKey(keyboard.get_on());
    printKey(keyboard.get_off());
    lcd.printf(F("\n"));

    lcd.font(&standard_5x8);
    if (pgm_text != midi.get_pgm_text()) {
      lcd.print(F("                          \r"));
      pgm_text = midi.get_pgm_text();
    }

    lcd.printf(F("%u %S\n"), midi._pgm + 1, pgm_text);
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
  keyboard.scan();
  keyboard.key_detect();
  while (KeyBuffer.length()) {
    Key key = KeyBuffer.read();
    switch (key.num & KEY_MASK_PREFIX) {
      case KEY_ON_PREFIX:
        midi.note_on(key.num + KEY_FIRST, 0, key.value);
        vel = key.value;
        extra(key.num);
        break;

      case KEY_OFF_PREFIX:
        midi.note_off((key.num & ~KEY_MASK_PREFIX) + KEY_FIRST, 0, key.value);
        break;
    }
  }

  time = TCNT0;
  time2++;
}
