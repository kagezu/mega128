// #include <macros/helper.h>
#pragma once
#include <Arduino.h>

// Управление портами

#define CONCAT(x, y)  x ## y

#define DDR(x)  CONCAT(DDR  , x)
#define PORT(x) CONCAT(PORT , x)
#define PIN(x)  CONCAT(PIN  , x)

#define IN(port, pin)         DDR(port)  &=~ _BV(pin)
#define OUT(port, pin)        DDR(port)  |=  _BV(pin)
#define SET(port, pin)        PORT(port) |=  _BV(pin)
#define CLR(port, pin)        PORT(port) &=~ _BV(pin)
#define GET(port, pin)        (PIN(port)  &  _BV(pin))
#define MASK(port, pin)                      _BV(pin)

#define SET_BITS(target, bits)        target |=  (bits)
#define CLR_BITS(target, bits)        target &= ~(bits)


// Прерывания

#define I_SAVE byte _sreg = SREG; __asm__ __volatile__ ("cli" :: )
#define I_REST SREG = _sreg

// Типы

#define to_byte(w,x)  (((byte *)&w)[x])

union dbyte {
  uint16_t word;
  uint8_t  byte[2];
};

union dword {
  uint32_t dword;
  uint16_t word;
  uint8_t  byte[4];
};


// Функции

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
