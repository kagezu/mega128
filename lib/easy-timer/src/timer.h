#pragma once

#ifdef __AVR_ATmega328P__
#include "timer_m328p.h"
#endif

#ifdef __AVR_ATmega128__
#include "timer_m128.h"

#define OCR0A               OCR0
#define T0_COMPA_ON         T0_COMP_ON
#define TIMER0_COMPA_vect   TIMER0_COMP_vect
#endif

// Установка частоты для таймера 0
void timer0(word freq)
{
  if (freq < F_CPU / 0x10000) { T0_DIV_1024; OCR0A = ((F_CPU / 0x400) / freq - 1); return; }
  if (freq < F_CPU / 0x4000) { T0_DIV_256; OCR0A = ((F_CPU / 0x100) / freq - 1); return; }
  if (freq < F_CPU / 0x800) { T0_DIV_64; OCR0A = ((F_CPU / 0x40) / freq - 1); return; }
  if (freq < F_CPU / 0x100) { T0_DIV_8; OCR0A = ((F_CPU / 8) / freq - 1); return; }
  T0_DIV_1; OCR0A = F_CPU / freq - 1; return;
}
