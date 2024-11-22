#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H
#include "Arduino.h"

// Цветовая модель
// RGB_12 4x4x4 bit / RGB_16 5x6x5 bit / RGB_18 6x6x6 bit

#define RGB_FORMAT RGB_18

// Таблица поворотов дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |   false   |   false   |   false   |
//  90°   |   false   |   true    |   true    |
// 180°   |   true    |   true    |   false   |
// 270°   |   true    |   false   |   true    |

#define FLIP_X    false
#define FLIP_Y    false
#define EX_X_Y    false

// Связь через SPI
// #define LCD_SPI   true

// LCD_PORT     Порт для SDA, SCK
// LCD_CONTROL  Порт для CS, RS
// LCD_CS       Выбор дисплея
// LCD_RS       0 = Запись команды / 1 = Запись данных
// INIT_LCD     Инициализация порта

#ifdef __AVR_ATmega128__
#define LCD_PORT PORTE
#define LCD_SCK     _BV(PE2)
#define LCD_SDA     _BV(PE3)

#define LCD_CONTROL PORTE
#define LCD_CS      PE0
#define LCD_RS      PE1

#define INIT_LCD                                            \
  DDRE  |=  LCD_SDA | LCD_SCK | _BV(LCD_CS) | _BV(LCD_RS);  \
  PORTE |=  LCD_SDA | LCD_SCK | _BV(LCD_CS) | _BV(LCD_RS);
#endif

#ifdef __AVR_ATmega328P__
#if LCD_SPI

#define LCD_CONTROL PORTC
#define LCD_CS      PC5
#define LCD_RS      PC4

#define INIT_LCD                                \
  DDRC   |= _BV(LCD_RS) | _BV(LCD_CS);          \
  PORTC  |= _BV(LCD_RS) | _BV(LCD_CS);          \
  DDRB   |= _BV(PB2)    | _BV(PB3) | _BV(PB5);  \
  SPCR    = _BV(SPE)    | _BV(MSTR);            \
  SPSR    = _BV(SPI2X);                         \
  TCCR0B |= _BV(CS00);                          \
  SPDR    = 0;

#else
// SDA, SCK должны быть на одном порту !
#define LCD_PORT    PORTB
#define LCD_SDA     _BV(PB3)
#define LCD_SCK     _BV(PB5)

#define LCD_CONTROL PORTC
#define LCD_CS      PC5
#define LCD_RS      PC4

#define INIT_LCD                          \
  DDRC  |= _BV(LCD_RS) | _BV(LCD_CS) ;    \
  PORTC |= _BV(LCD_RS) | _BV(LCD_CS);     \
  DDRB  |=  LCD_SDA    |  LCD_SCK ;       \
  PORTB |=  LCD_SDA    |  LCD_SCK ;
#endif

#endif

// размер дисплея
#define LCD_MAX_X 127
#define LCD_MAX_Y 159

// Команды управления вывода на дисплей
#define DISPLAY_DISCONNECT  bitSet(LCD_CONTROL, LCD_CS);      // Снять выбор дисплея
#define DATA_MODE           bitSet(LCD_CONTROL, LCD_RS);      // Запись данных
#define DISPLAY_CONNECT     bitClear(LCD_CONTROL, LCD_CS);    // Выбор дисплея
#define COMMAND_MODE        bitClear(LCD_CONTROL, LCD_RS) ;   // Запись команды

#if EX_X_Y
#define MAX_X     LCD_MAX_Y
#define MAX_Y     LCD_MAX_X
#else
#define MAX_X     LCD_MAX_X
#define MAX_Y     LCD_MAX_Y
#endif

// Interface pixel format
#define RGB_12 0x03 // 4x4x4 bit
#define RGB_16 0x05 // 5x6x5 bit
#define RGB_18 0x06 // 6x6x6 bit (24 bit transfer)

#endif
