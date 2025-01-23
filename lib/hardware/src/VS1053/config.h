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

// SC_MULT Множитель частоты  SPI READ / WRITE
// 0 0x0000 × 1.0   = 12  1,7 /  3
// 1 0x2000 × 2.0   = 24  3,4 /  6
// 2 0x4000 × 2.5   = 30  4,2 /  7,5
// 3 0x6000 × 3.0   = 36  5,1 /  9
// 4 0x8000 × 3.5   = 42  6,0 / 10,5
// 5 0xA000 × 4.0   = 48  6,8 / 12
// 6 0xC000 × 4.5   = 54  7,7 / 13,5   MAX CPU
// 7 0xE000 × 5.0   = 60  8,5 / 15

#define SC_MULT           0x8000

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
#define SCI_FQ_MAX        4000


// -20 dB
#define SCI_VOL_DEFAULT   20
