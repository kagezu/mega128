#pragma once
#include <Arduino.h>

// макрос конкатенации макросов
#define CONCAT(x, y)      x ## y

#define DDR(x)            CONCAT(DDR, x)
#define PORT(x)           CONCAT(PORT,x)
#define PIN(x)            CONCAT(PIN, x)

#define OUT   |=
#define IN    &=~
#define UP    |=
#define DOWN  &=~

#define USE_PIN(port, pin, mode)      DDR(port) mode _BV(pin)
#define SET_PIN(port, pin, mode)      PORT(port) mode _BV(pin)
#define GET_PIN(port, pin)            PIN(port) & _BV(pin)

#define MAKE_BITS(target, set, clear) target = (target &~ clear) | set;
#define SET_BITS(target, bits)        target |= bits;
#define CLR_BITS(target, bits)        target &= ~bits;
