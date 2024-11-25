#include <Arduino.h>

/*
  Настройка таймера 0:
--
  + T0_STOP Отключение таймера
  + T0_DIV_[1, 8, 32, 64, 128, 256, 1024] Предварительный делитель
  + T0_[NORMAL, CTC, FAST_PWM, PHASE_PWM] Режимы таймера: счётчик, шим
  + T0_OC0_[OFF, ON, PVM[ _INV] Режимы выводов: вкл, выкл, шим
  + OCR0 = 0...255; Регистры управлением делителем / заполненностью шим
  + T0_[OVF, COMP]_[ON, OFF] Установка прерываний
*/
#define T0


//------------------- Timer 0 ----------------------


// отключить тактирование
#define T0_STOP     TCCR0 &= ~B111;

// пред делитель N = [1, 8, 32, 64, 128, 256, 1024]
#define T0_DIV_1    TCCR0 = (TCCR0 & ~B111) | B001;
#define T0_DIV_8    TCCR0 = (TCCR0 & ~B111) | B010;
#define T0_DIV_32   TCCR0 = (TCCR0 & ~B111) | B011;
#define T0_DIV_64   TCCR0 = (TCCR0 & ~B111) | B100;
#define T0_DIV_128  TCCR0 = (TCCR0 & ~B111) | B101;
#define T0_DIV_256  TCCR0 = (TCCR0 & ~B111) | B110;
#define T0_DIV_1024 TCCR0 = (TCCR0 & ~B111) | B111;


// циклично 8 бит, top = 0xFF
// f = clk / N*256
// Вывод OC0: toggle (f/2)
#define T0_NORMAL   TCCR0 &= ~(_BV(WGM01) | _BV(WGM00));

// CTC, top = OCR0
// f = clk / N*(1+OCR0)
// Вывод OC0: toggle (f/2)
#define T0_CTC      TCCR0 &= ~_BV(WGM00); TCCR0 |= _BV(WGM01);

// шим 8 бит, top = 0xFF
// f = clk / N*256
// Вывод OC0: заполнение шим: OCR0
#define T0_FAST_PWM TCCR0 |= _BV(WGM01) | _BV(WGM00);

// симметричный шим 8 бит, top = 0xFF
// f = clk / N*512
// Выводы OC0: заполнение шим: OCR0
#define T0_PHASE_PWM TCCR0 &= ~_BV(WGM01);TCCR0 |= _BV(WGM00);

/* Режим работы OC0 */

// Вывод OC0: off
#define T0_OC0A_OFF\
  TCCR0 &= ~(_BV(COM01) | _BV(COM00));\
  DDRB &= ~_BV(PB4);
// Вывод OC0: toggle (f/2)
#define T0_OC0A_ON \
  TCCR0 &= ~_BV(COM01);\
  TCCR0 |= _BV(COM00);\
  TCCR0 |= _BV(FOC0);\
  DDRB |= _BV(PB4);
// Вывод OC0: шим
#define T0_OC0A_PWM \
  TCCR0 &= ~_BV(COM00);\
  TCCR0 |= _BV(COM01);\
  TCCR0 &= ~_BV(FOC0);\
  DDRB |= _BV(PB4);
// Вывод OC0: инвертированный шим
#define T0_OC0A_PWM_INV \
  TCCR0 |= (_BV(COM01) | _BV(COM00));\
  TCCR0 &= ~_BV(FOC0);\
  DDRB |= _BV(PB4);

/*функции прерываний по таймеру*/

// Использовать: ISR(TIMER0_COMP_vect)
#define T0_COMP_ON   TIMSK |= _BV(OCIE0);
// Использовать: ISR(TIMER0_OVF_vect)
#define T0_OVF_ON    TIMSK |= _BV(TOIE);
#define T0_COMP_OFF  TIMSK &=~_BV(OCIE0);
#define T0_OVF_OFF   TIMSK &=~_BV(TOIE);


//------------------- Timer 1 ----------------------

