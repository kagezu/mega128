// #include <macros/helper.h>
#pragma once
#include <Arduino.h>

// Управление портами

#define CONCAT(x, y)  x ## y

#define DDR(x)  CONCAT(DDR  , x)
#define PORT(x) CONCAT(PORT , x)
#define PIN(x)  CONCAT(PIN  , x)

#define OUT |=
#define IN  &=~

#define USE_PIN(port, pin, mode)      DDR(port) mode _BV(pin)
#define SET_PIN(port, pin)            PORT(port) |= _BV(pin)
#define CLR_PIN(port, pin)            PORT(port) &=~ _BV(pin)
#define GET_PIN(port, pin)            (PIN(port) & _BV(pin))

#define SET_BITS(target, bits)        target |=  (bits)
#define CLR_BITS(target, bits)        target &= ~(bits)


// Прерывания

#define I_SAVE byte _sreg = SREG; __asm__ __volatile__ ("cli" :: )
#define I_REST SREG = _sreg
