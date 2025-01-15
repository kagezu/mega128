#pragma once
#include "Arduino.h"

#define X_DREQ(func)   func ( D, 2 )
#define X_CS(func)     func ( D, 6 )
#define X_DCS(func)    func ( D, 7 )
#define X_RESET(func)  func ( B, 0 )

//  SC_MULT   XTALI
// 0 0x0000 × 1.0
// 1 0x2000 × 2.0
// 2 0x4000 × 2.5
// 3 0x6000 × 3.0
// 4 0x8000 × 3.5
// 5 0xA000 × 4.0
// 6 0xC000 × 4.5
// 7 0xE000 × 5.0

#define SC_MULT           0xA000

//   SC_ADD   XTALI
// 0 0x0000 + 0
// 1 0x0800 + 1.0
// 2 0x1000 + 1.5
// 3 0x1800 + 2.0

#define SC_ADD            0x0800

// Инициализируем на частоте не выше 12/7 MHz
#define SCI_FQ_INIT       1000
#define SCI_FQ_MAX        6000


// -20 dB
#define SCI_VOL_DEFAULT   40
