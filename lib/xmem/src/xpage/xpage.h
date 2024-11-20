#ifndef XPAGE_H
#define XPAGE_H

#include "../xmem.config.h"
#include "../dmem/dyn-memory.h"

#if __AVR_ATmega128__

#define XMEM_START      RAMEND  + 1
#define XMEM_OVER       XRAMEND + 1

#elif __AVR_ATmega328P__
// Профиль используется для тестирования и совместимости
#define XMEM_START      0x400
#define XMEM_OVER       RAMEND  + 1
#endif

// Физически возможный минимальный размер страницы
// Соответствует XMM[0-2] = 7 в регистре XMCRB
#define XMEM_MIN_SIZE 0x100

#define XMEM_60K  0   // full 16 bit address
#define XMEM_32K  1   // 32768 байт
#define XMEM_16K  2   // 16384 байт
#define XMEM_8K   3   //  8192 байт
#define XMEM_4K   4   //  4096 байт
#define XMEM_2K   5   //  2048 байт
#define XMEM_1K   6   //  1024 байт
#define XMEM_0K   7   //   256 байт

#define XMEM_SIZE(bitMask, start)                                  \
        bitMask ?                                                  \
        _BV(bitMask == XMEM_0K ? 0 : 8 - bitMask) * XMEM_MIN_SIZE  \
        : XMEM_OVER - start

class XPage : public DynMemory {
private:
  uint8_t _xmm;           // Значение битов XMM[0-2] в регистре XMCRB
  uint8_t _highAddress;   // Биты A[23:16] физической памяти
  uint8_t _lowAddress;    // Биты A[15:8]  физической памяти

public:
  /**
    @param physicalAddress адрес страницы в физической памяти (без младшего байта)
    @param bitMask маска для битов адреса (размер страницы), например: XMEM_32K
    @param start адрес начала eXternal SRAM в логическом пространстве
   */
  XPage(
    uint16_t physicalAddress = 0, // Начало физ. памяти
    uint8_t bitMask = XMEM_60K,   // Всё доступное адресное пространство
    uint16_t start = XMEM_START   // Первый доступный адрес eXternal SRAM
  )
    :DynMemory(
      start,
      XMEM_SIZE(bitMask, start)
    ),
    _xmm(bitMask),
    _highAddress(highByte(physicalAddress)),
    _lowAddress(lowByte(physicalAddress))
  {}

public:
  uint8_t *malloc(word memorySize);

public:
  void use();
  static void init();
};
#endif
