// #include <macros/helper.h>
#pragma once
#include <Arduino.h>

// Управление портами

#define CONCAT(x, y)  x ## y

#define PORT(x) CONCAT(PORT , x)
#define DDR(x)  CONCAT(DDR  , x)
#define PIN(x)  CONCAT(PIN  , x)

#define IN(port, pin)         DDR(port)  &=~ _BV(pin)
#define OUT(port, pin)        DDR(port)  |=  _BV(pin)
#define SET(port, pin)        PORT(port) |=  _BV(pin)
#define RES(port, pin)        PORT(port) &=~ _BV(pin)
#define CLR(port, pin)        PORT(port) &=~ _BV(pin)
#define GET(port, pin)        (PIN(port)  &  _BV(pin))
#define MASK(port, pin)                      _BV(pin)
#define MMO(port, pin)        PORT(port)
#define MMI(port, pin)        PIN(port)

#define SET_BITS(target, bits)        target |=  (bits)
#define CLR_BITS(target, bits)        target &= ~(bits)


// Доступ к байтам

#define to_byte(w,x)  (((uint8_t *)&w)[x])

union dbyte {
  uint16_t word;
  uint8_t  byte[2];
  struct {
    uint8_t low;
    uint8_t high;
  };
};

union dword {
  uint32_t dword;
  uint16_t word[2];
  uint8_t  byte[4];
  struct {
    uint16_t low;
    uint16_t high;
  };
};

// Функции

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

// Типы

typedef uint8_t reg;

// Прочее

#define P(x) PSTR(x)

#define I_SAVE uint8_t _sreg = SREG; __asm__ __volatile__ ("cli" :: )
#define I_REST SREG = _sreg

#define SWAP(x, y)    { x^=y; y^=x; x^=y; }
#define swap8(a, b)   { uint8_t _t = a; a = b; b = _t; }
#define swap16(a, b)  { uint16_t _t = a; a = b; b = _t; }
