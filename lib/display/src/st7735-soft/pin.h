#pragma once
#include <core.h>

//===================== Config =============================

// L_CS       Выбор дисплея
// L_RS       0 = Запись команды / 1 = Запись данных
// L_RST      0 = Reset
// L_SCK      Тактирование / low -> high
// L_SDA      Данные

#ifndef L_SCK

#ifdef __AVR_ATmega328P__

#define L_SCK(x)     x ( C, PC0)
#define L_SDA(x)     x ( C, PC1)
#define L_RST(x)     x ( C, PC3)
#define L_CS(x)      x ( C, PC4)
#define L_RS(x)      x ( C, PC2)

#endif

#endif
