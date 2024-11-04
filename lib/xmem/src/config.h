#include <Arduino.h>

#ifdef __AVR_ATmega128__
#define XMEM_USE

// Определение старших битов физического адреса памяти
// 128k SRAM
#define XMEM_A16        PE6
#define XMEM_A16_PORT   PORTE
#define XMEM_A16_INIT   bitSet(DDRE, XMEM_A16);

// 256k SRAM
// #define XMEM_A17        PE7
// #define XMEM_A17_PORT   PORTE
// #define XMEM_A17_INIT   bitSet(DDRE, XMEM_A17);

// 512k SRAM
// #define XMEM_A18        PB7
// #define XMEM_A18_PORT   PORTB
// #define XMEM_A18_INIT   bitSet(DDRB, XMEM_A18);

#endif

#ifdef __AVR_ATmega328P__
// Профиль используется для тестирования и совместимости
#define XMEM_START      0x400
#define XMEM_END        RAMEND
#endif
