#include <Arduino.h>

/*
  Пример настройки таймера:

  T0_DIV_[1, 8, 64, 256, 1024]
  T0_[ NORMAL, CTC, FAST_PWM[_CUSTOM], PHASE_PWM[_CUSTOM] ]
  T0_OC0[A, B]_[ OFF, TOGGLE, DIRECT, INVERT ]
  OCR0[A, B] = 0...255; делитель / ширина шим
*/

//------------------- Timer 0 ----------------------

// отключить тактирование
#define T0_STOP       TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
// внешний источник - T0, по спаду
#define T0_EXT_FALL   TCCR0B = (TCCR0B & ~_BV(CS00)) | _BV(CS02) | _BV(CS01);
// внешний источник - T0, по фронту
#define T0_EXT_RISE   TCCR0B |= (_BV(CS02) | _BV(CS01) | _BV(CS00));

// пред делитель f=clk/ 2*N, N = [1, 8, 64, 256, 1024]
#define T0_DIV_1      TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
#define T0_DIV_8      TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS00))) | _BV(CS01);
#define T0_DIV_64     TCCR0B = (TCCR0B & ~_BV(CS02)) | _BV(CS01)   | _BV(CS00);
#define T0_DIV_256    TCCR0B = (TCCR0B & ~(_BV(CS01) | _BV(CS00))) | _BV(CS02);
#define T0_DIV_1024   TCCR0B = (TCCR0B & ~_BV(CS01)) | _BV(CS02)   | _BV(CS00);


// обычный счет, top = 0xFF
// f = clk / N*512                                toggle
#define T0_NORMAL                                                 \
                      TCCR0B &= ~_BV(WGM02);                      \
                      TCCR0A &= ~(_BV(WGM01) | _BV(WGM00));
// CTC top = OCR0A
// OC0A/OC0B: f = clk / 2*N*(1+OCR0A)             toggle
#define T0_CTC                                                    \
                      TCCR0B &= ~_BV(WGM02);                      \
                      TCCR0A &= ~_BV(WGM00);                      \
                      TCCR0A |= _BV(WGM01);
// аппаратный шим 8 бит, top = 0xFF
// OC0A: f = clk / N*256          ширина OCR0A    direct/invert
// OC0B: f = clk / N*256          ширина OCR0B    direct/invert
#define T0_FAST_PWM                                               \
                      TCCR0B &= ~_BV(WGM02);                      \
                      TCCR0A |= (_BV(WGM01) | _BV(WGM00));
// аппаратный шим 8 бит, top = OCR0A
// OC0A: f = clk / 2*N*(1+OCR0A)  ширина 50%      toggle
// OC0A: f = clk /   N*(1+OCR0A)  ширина OCR0B    direct/invert
#define T0_FAST_PWM_CUSTOM                                        \
                      TCCR0B |= _BV(WGM02);                       \
                      TCCR0A |= (_BV(WGM01) | _BV(WGM00));
// симметричный шим 8 бит, top = 0xFF
// OC0A: f = clk / N*512          ширина = OCR0A  direct/invert
// OC0B: f = clk / N*512          ширина = OCR0B  direct/invert
#define T0_PHASE_PWM                                              \
                      TCCR0B &= ~_BV(WGM02);                      \
                      TCCR0A &= ~_BV(WGM01);                      \
                      TCCR0A |= _BV(WGM00);
// симметричный 8 бит, top = OCR0A
// OC0A: f = clk / 4*N*(OCR0A)    ширина 50%      toggle
// OC0B: f = clk / 2*N*(OCR0A)    ширина OCR0B    direct/invert
#define T0_PHASE_PWM_CUSTOM                                       \
                      TCCR0B |= _BV(WGM02);                       \
                      TCCR0A &= ~_BV(WGM01);                      \
                      TCCR0A |= _BV(WGM00);

/* Режим работы OC0A */

#define T0_OC0A_OFF   TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0));
#define T0_OC0A_TOGGLE                                            \
                      TCCR0A &= ~_BV(COM0A1);                     \
                      TCCR0A |= _BV(COM0A0);                      \
                      TCCR0B |= _BV(FOC0A);
#define T0_OC0A_DIRECT                                            \
                      TCCR0A &= ~_BV(COM0A0);                     \
                      TCCR0A |= _BV(COM0A1);                      \
                      TCCR0B &= ~_BV(FOC0A);
#define T0_OC0A_INVERT                                            \
                      TCCR0A |= (_BV(COM0A1) | _BV(COM0A0));      \
                      TCCR0B &= ~_BV(FOC0A);

/* Режим работы OC0B */

#define T0_OC0B_OFF   TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0));
#define T0_OC0B_TOGGLE                                            \
                      TCCR0A &= ~_BV(COM0B1);                     \
                      TCCR0A |= _BV(COM0B0);                      \
                      TCCR0B |= _BV(FOC0B);
#define T0_OC0B_DIRECT                                            \
                      TCCR0A &= ~_BV(COM0B0);                     \
                      TCCR0A |= _BV(COM0B1);                      \
                      TCCR0B &= ~_BV(FOC0B);
#define T0_OC0B_INVERT                                            \
                      TCCR0A |= (_BV(COM0B1) | _BV(COM0B0));      \
                      TCCR0B &= ~_BV(FOC0B);

/*функции прерываний по таймеру*/

#define T0_COMPA_ON   TIMSK0 |= _BV(OCIE0A);
#define T0_COMPB_ON   TIMSK0 |= _BV(OCIE0B);
#define T0_OVF_ON     TIMSK0 |= _BV(TOIE);
#define T0_COMPA_OFF  TIMSK0 &=~_BV(OCIE0A);
#define T0_COMPB_OFF  TIMSK0 &=~_BV(OCIE0B);
#define T0_OVF_OFF    TIMSK0 &=~_BV(TOIE);
