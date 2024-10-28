#include <Arduino.h>

#ifndef init_h
#define init_h

// Цветовой формат дисплея ST7753S
// RGB_12 - 4x4x4 bit
// RGB_16 - 5x6x5 bit
// RGB_18 - 6x6x6 bit (24 bit transfer)

#define RGB_FORMAT RGB_16

// вспомогательные скрипты

#define SET_BIT(port, mask) port |= mask;
#define RES_BIT(port, mask) port &= ~mask;

// Команды ассемблера

// #define _SBI(port, bit) port |= _BV(bit);
// #define _CBI(port, bit) port &= ~_BV(bit);

#define _CLI __asm__ __volatile__("cli") // Отключает прерывания
#define _SEI __asm__ __volatile__("sei") // Включение прерываний

#endif
