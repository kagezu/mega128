#include <Arduino.h>

/*
  Настройка таймера 0:

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

// аппаратный шим настраиваемый, top = OCR0A
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

// симметричный настраиваемый, top = OCR0A
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


//------------------- Timer 1 ----------------------


// отключить тактирование
#define T1_STOP       TCCR1B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
// внешний источник - T0, по спаду
#define T1_EXT_FALL   TCCR1B = (TCCR1B & ~_BV(CS00)) | _BV(CS02) | _BV(CS01);
// внешний источник - T0, по фронту
#define T1_EXT_RISE   TCCR1B |= (_BV(CS02) | _BV(CS01) | _BV(CS00));

// пред делитель f=clk/ 2*N, N = [1, 8, 64, 256, 1024]
#define T1_DIV_1      TCCR1B = (TCCR1B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00);
#define T1_DIV_8      TCCR1B = (TCCR1B & ~(_BV(CS02) | _BV(CS00))) | _BV(CS01);
#define T1_DIV_64     TCCR1B = (TCCR1B & ~_BV(CS02)) | _BV(CS01)   | _BV(CS00);
#define T1_DIV_256    TCCR1B = (TCCR1B & ~(_BV(CS01) | _BV(CS00))) | _BV(CS02);
#define T1_DIV_1024   TCCR1B = (TCCR1B & ~_BV(CS01)) | _BV(CS02)   | _BV(CS00);


// обычный счет, top = 0xFFFF
// f = clk / 2*N*65536                            toggle
#define T1_NORMAL                                                 \
                      TCCR1A &= ~(_BV( WGM11) | _BV( WGM10));     \
                      TCCR1B &= ~(_BV( WGM13) | _BV( WGM12));

// CTC top = OCR1A
// OC1A/OC1B: f = clk / 2*N*(1+OCR1A)             toggle
#define T1_CTC                                                    \
                      TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));\
                      TCCR1B &= ~_BV(WGM13);\
                      TCCR1B |= _BV(WGM12);

// аппаратный шим 8 бит, top = 0xFF
// OC1A/OC1B: f = clk / N*256     ширина OCR1A/OCR1B  direct/invert
#define T1_FAST_PWM_8                                             \
                      TCCR1A &= ~_BV(WGM11);                      \
                      TCCR1B &= ~_BV(WGM13);                      \
                      TCCR1A |= _BV(WGM10);                       \
                      TCCR1B |= _BV(WGM12);

// аппаратный шим 9 бит, top = 0x1FF
// OC1A/OC1B:: f = clk / N*512    ширина OCR1A/OCR1B      direct/invert
#define T1_FAST_PWM_9                                             \
                      TCCR1A &= ~_BV(WGM10);                      \
                      TCCR1B &= ~_BV(WGM13);                      \
                      TCCR1A |= _BV(WGM11);                       \
                      TCCR1B |= _BV(WGM12);

// аппаратный шим 9 бит, top = 0x3FF
// OC1A/OC1B: f = clk / N*1024     ширина OCR1A/OCR1B     direct/invert
#define T1_FAST_PWM_10                                            \
                      TCCR1A |= _BV(WGM10) | _BV(WGM11);          \
                      TCCR1B &= ~_BV(WGM13);                      \
                      TCCR1B |= _BV(WGM12);

// аппаратный шим настраиваемый, top = ICR1 [ ICR1H:ICR1L ]
// OC1A/OC1B: f = clk / (1+ICR1)   ширина OCR1A/OCR1B     direct/invert
#define T1_FAST_PWM_CUSTOM                                        \
                      TCCR1B |= (_BV(WGM13) | _BV(WGM12));        \
                      TCCR1A &= ~_BV(WGM10);                      \
                      TCCR1A |= _BV(WGM11);

// симметричный шим 8 бит, top = 0xFF
// OC1A/OC1B:: f = clk / N*512      ширина OCR1A/OCR1B    direct/invert
#define T1_PHASE_PWM_8                                            \
                      TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));       \
                      TCCR1A &= ~_BV(WGM11);                      \
                      TCCR1A |= _BV(WGM10);

// симметричный шим 9 бит, top = 0x1FF
// OC1A/OC1B: f = clk / N*1024       ширина OCR1A/OCR1B   direct/invert
#define T1_PHASE_PWM_9                                            \
                      TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));       \
                      TCCR1A &= ~_BV(WGM10);                      \
                      TCCR1A |= _BV(WGM11);

// симметричный шим 10 бит, top = 0x3FF
// OC1A/OC1B: f = clk / N*2048        ширина OCR1A/OCR1B  direct/invert
#define T1_PHASE_PWM_10                                           \
                      TCCR1A |= (_BV(WGM11) | _BV(WGM10));        \
                      TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));

// симметричный настраиваемый, top = ICR1 [ ICR1H:ICR1L ]
// OC1A/OC1B:: f = clk / 2*N*(ICR1)   ширина OCR1A/OCR1B  direct/invert
#define T1_PHASE_PWM_CUSTOM                                       \
                      TCCR1B &= ~_BV(WGM12);                      \
                      TCCR1A &= ~_BV(WGM10);                      \
                      TCCR1B |= _BV(WGM13);                       \
                      TCCR1A |= _BV(WGM11);

/* Режим работы OC1A */

#define T1_OC1A_OFF   TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0));
#define T1_OC1A_TOGGLE                                            \
                      TCCR1A &= ~_BV(COM1A1);                     \
                      TCCR1A |= _BV(COM1A0);                      \
                      TCCR1C |= _BV(FOC1A);
#define T1_OC1A_DIRECT                                            \
                      TCCR1A &= ~_BV(COM1A0);                     \
                      TCCR1A |= _BV(COM1A1);                      \
                      TCCR1C &= ~_BV(FOC1A);
#define T1_OC1A_INVERT                                            \
                      TCCR1A |= (_BV(COM1A1) | _BV(COM1A0));      \
                      TCCR1C &= ~_BV(FOC1A);

/* Режим работы OC1B */

#define T1_OC1B_OFF   TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));
#define T1_OC1B_TOGGLE                                            \
                      TCCR1A &= ~_BV(COM1B1);                     \
                      TCCR1A |= _BV(COM1B0);                      \
                      TCCR1C |= _BV(FOC1B);
#define T1_OC1B_DIRECT                                            \
                      TCCR1A &= ~_BV(COM1B0);                     \
                      TCCR1A |= _BV(COM1B1);                      \
                      TCCR1C &=~_BV(FOC1B);
#define T1_OC1B_INVERT                                            \
                      TCCR1A |= _BV(COM1B1) | _BV(COM1B0);        \
                      TCCR1C &=~_BV(FOC1B);
