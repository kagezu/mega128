#include <Arduino.h>

#ifndef init_h
#define init_h

// вспомогательные скрипты

#define SET_BITS(port, mask) port |= mask;
#define RES_BITS(port, mask) port &= ~mask;

// Команды ассемблера

// #define _SBI(port, bit) port |= _BV(bit);
// #define _CBI(port, bit) port &= ~_BV(bit);

#define _CLI __asm__ __volatile__("cli") // Отключает прерывания
#define _SEI __asm__ __volatile__("sei") // Включение прерываний

#endif
