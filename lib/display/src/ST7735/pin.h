#pragma once
#include <core.h>

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование
// L_SDA      Данные

#ifndef L_SCK

#ifdef __AVR_ATmega328P__
#ifdef LCD_SPI

#define L_SCK(x)    x ( B, PB5)
#define L_SDA(x)    x ( B, PB3)
#define L_RST(x)    x ( B, PB2)
#define L_CS(x)     x ( B, PC5)
#define L_RS(x)     x ( B, PC4)

#endif
#ifdef LCD_SOFT

#define L_SCK(x)     x ( C, PC0)
#define L_SDA(x)     x ( C, PC1)
#define L_RST(x)     x ( C, PC3)
#define L_CS(x)      x ( C, PC4)
#define L_RS(x)      x ( C, PC2)

#endif
#endif

#endif
