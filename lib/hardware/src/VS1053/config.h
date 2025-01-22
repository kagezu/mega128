#pragma once
#include "Arduino.h"

// HIGH = буфер свободен / команды выполнены
#define X_DREQ(func)   func ( D, 2 )
// Отправить команду (доступ к регистрам) буфер 32 байта
#define X_CS(func)     func ( D, 6 )
// Отправить данные (доступ к регистрам) буфер 2к байт
#define X_DCS(func)    func ( D, 7 )
// Аппаратный сброс
#define X_RESET(func)  func ( B, 0 )

// SC_MULT Множитель частоты  MAX SPI
// 0 0x0000 × 1.0   = 12,288  1,755
// 1 0x2000 × 2.0   = 24,576  3,510
// 2 0x4000 × 2.5   = 30,720  4,388
// 3 0x6000 × 3.0   = 36,864  5,266
// 4 0x8000 × 3.5   = 43,008  6,144
// 5 0xA000 × 4.0   = 49,152  7,021
// 6 0xC000 × 4.5   = 55,296  7,899   MAX CPU
// 7 0xE000 × 5.0   = 61,440  8,777

#define SC_MULT           0xA000

//   SC_ADD   XTALI
// 0 0x0000 + 0
// 1 0x0800 + 1.0
// 2 0x1000 + 1.5
// 3 0x1800 + 2.0

#define SC_ADD            0x0800
// Частота кварца в кГц
#define SC_FQ_CRYSTAL     12000
#define SC_FREQ           ((SC_FQ_CRYSTAL - 8000) >> 2)

// Инициализация SPI, в килогерцах
#define SCI_FQ_INIT       1000
#define SCI_FQ_MAX        2000


// -20 dB
#define SCI_VOL_DEFAULT   20
