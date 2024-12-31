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

#define F_CPU_DIV256    (F_CPU >> 8)

// Установка частоты для таймера 0
void timer0(word freq)
{
  if (freq < (F_CPU_DIV256 >> 8)) { T0_DIV_1024; OCR0A = ((F_CPU_DIV256 >> 2) / freq - 1); return; }
  if (freq < (F_CPU_DIV256 >> 6)) { T0_DIV_256; OCR0A = (F_CPU_DIV256 / freq - 1); return; }
  if (freq < (F_CPU_DIV256 >> 3)) { T0_DIV_64; OCR0A = ((F_CPU / 0x40) / freq - 1); return; }
  if (freq < (F_CPU_DIV256 >> 0)) { T0_DIV_8; OCR0A = ((F_CPU / 8) / freq - 1); return; }
  T0_DIV_1; OCR0A = F_CPU / freq - 1; return;
}
