#include <Arduino.h>

#if defined(__AVR_ATmega328P__)

// Профиль используется для тестирования и совместимости
#define XMEM_1K_X1   1  // 1k SRAM

#elif defined(__AVR_ATmega128__)

// Используется всё адресное пространство,
// используется 60к на страницу
#define XMEM_60K_X2    1  // 60k 2 page

// Используется страницы в 32к,
// за пределами области циклически дублируются адреса
// #define XMEM_32K_X4    1  // 32k 4 page

// Управляет 16м битом физического адреса памяти
#define XMEM_A16        PE6
#define XMEM_A16_PORT   PORTE
#define XMEM_A16_INIT   bitSet(DDRE, XMEM_A16)

// Управляет 15м битом физического адреса памяти,
// при условии XMM[0-2] > 0 в регистре XMCRB 
#define XMEM_A15        PC7
#define XMEM_A15_PORT   PORTC
#define XMEM_A15_INIT   bitSet(DDRC, XMEM_A15)

#endif
