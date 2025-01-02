#pragma once
#include <macros/helper.h>

//===================== Config =============================

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

// #define FLIP_X    true
// #define FLIP_Y    false
// #define EX_X_Y    true


// #define LCD_SPI         // Подключает через SPI
#define LCD_DRAW        // Подключает класс для рисования
#define LCD_PRINT       // Подключает класс для вывода текста

//==========================================================

// LCD_DATA     Порт для SDA, SCK, RST ( 8 если не используется )
// LCD_CONTROL  Порт для CS, RS
// LCD_CS       Выбор дисплея
// LCD_RS       0 = Запись команды / 1 = Запись данных
// INIT_LCD     Инициализация порта
// INIT_SPI     Инициализация SPI

#ifdef __AVR_ATmega128__
#define LCD_DATA    E
#define LCD_SCK     _BV(PE2)
#define LCD_SDA     _BV(PE3)
#define LCD_RST     _BV(PE7)

#define LCD_CONTROL E
#define LCD_CS      _BV(PE0)
#define LCD_RS      _BV(PE1)
#endif

#ifdef __AVR_ATmega328P__
#ifdef LCD_SPI

// Пины зарезервированы SPI
#define LCD_DATA    B
#define LCD_SCK     _BV(PB5)
#define LCD_SDA     _BV(PB3)
#define LCD_RST     _BV(PB2)

#define LCD_CONTROL C
#define LCD_CS      _BV(PC5)
#define LCD_RS      _BV(PC4)

#define INIT_SPI                                \
  SPCR    = _BV(SPE)    | _BV(MSTR);            \
  SPSR    = _BV(SPI2X);                         \
  TCCR0B |= _BV(CS00);                          \
  SPDR    = 0;

#else
// SDA, SCK должны быть на одном порту !
#define LCD_DATA    C
#define LCD_SCK     _BV(PC0)
#define LCD_SDA     _BV(PC1)
#define LCD_RST     _BV(PC3)

#define LCD_CONTROL C
#define LCD_CS      _BV(PC4)
#define LCD_RS      _BV(PC2)
#endif

#define INIT_SPI  // Заглушка
#endif

///////////////////////////////////////////////////////////////////////////
// Вторичные макросы, зависящие от настроек выше

#define INIT_LCD                                          \
  SET_BITS(PORT(LCD_DATA), LCD_RST | LCD_SDA | LCD_SCK);  \
  SET_BITS(DDR(LCD_DATA), LCD_RST | LCD_SDA | LCD_SCK);   \
  SET_BITS(PORT(LCD_CONTROL), LCD_CS | LCD_RS);           \
  SET_BITS(DDR(LCD_CONTROL), LCD_CS | LCD_RS);

#define LCD_PORT    PORT(LCD_DATA)

// размер дисплея
#define LCD_MAX_X 127
#define LCD_MAX_Y 159

// Команды управления вывода на дисплей
#define DISPLAY_DISCONNECT  SET_BITS(PORT(LCD_CONTROL), LCD_CS);    // Снять выбор дисплея
#define DATA_MODE           SET_BITS(PORT(LCD_CONTROL), LCD_RS);    // Запись данных
#define DISPLAY_CONNECT     CLR_BITS(PORT(LCD_CONTROL), LCD_CS);    // Выбор дисплея
#define COMMAND_MODE        CLR_BITS(PORT(LCD_CONTROL), LCD_RS);    // Запись команды

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
