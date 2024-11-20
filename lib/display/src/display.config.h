#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H
#include "Arduino.h"

// Цветовая модель
// RGB_12 4x4x4 bit / RGB_16 5x6x5 bit / RGB_18 6x6x6 bit

#define RGB_FORMAT RGB_16

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

// LCD_PORT   Порт управления дисплеем
// LCD_CS     0 = Выбор дисплея / 1 = Снять выбор дисплея
// LCD_RS     0 = Запись команды / 1 = Запись данных
// LCD_SDA    0/1 Данные
// LCD_SCK    Тактирование
// INIT_LCD_PORT Инициализация порта

#ifdef __AVR_ATmega128__
#define LCD_PORT PORTE
#define LCD_CS _BV(PE0)
#define LCD_RS _BV(PE1)
#define LCD_SCK _BV(PE2)
#define LCD_SDA _BV(PE3)

#define INIT_LCD                         \
  DDRE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS; \
  PORTE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS;
#endif

#ifdef __AVR_ATmega328P__
#ifdef LCD_SPI
#define LCD_PORT PORTC
#define LCD_CS _BV(PC5)
#define LCD_RESET _BV(PC4)
#define LCD_RS _BV(PC3)
#define LCD_SDA 0
#define LCD_SCK 0

#define INIT_LCD                                        \
  DDRC |= LCD_RS | LCD_CS | LCD_RESET;                  \
  PORTC |= LCD_RS | LCD_CS | LCD_RESET;                 \
  DDRB |= _BV(PB2)  | _BV(PB3) | _BV(PB5);              \
  SPCR = _BV(SPE) | _BV(MSTR);                          \
  SPSR = _BV(SPI2X);                                    \
  TCCR0B |= _BV(CS00);                                  \
  SPDR = 0;

#else
#define LCD_PORT PORTC
#define LCD_CS _BV(PC4)
#define LCD_RESET _BV(PC3)
#define LCD_RS _BV(PC2)
#define LCD_SDA _BV(PC1)
#define LCD_SCK _BV(PC0)


#define INIT_LCD                                     \
  DDRC |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS | LCD_RESET; \
  PORTC |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS | LCD_RESET;
#endif
#endif

#define LCD_MAX_X 127
#define LCD_MAX_Y 159

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
