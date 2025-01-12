#pragma once
#include "config.h"
#include "memory.h"

#define XMEM_START      RAMEND  + 1
#define XMEM_OVER       XRAMEND + 1

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

class XPage : public Memory {
private:
  byte _xmm;           // Значение битов XMM[0-2] в регистре XMCRB
  byte _highAddress;   // Биты A[23:16] физической памяти
  byte _lowAddress;    // Биты A[15:8]  физической памяти

public:
  /**
    @param physicalAddress адрес страницы в физической памяти (без младшего байта)
    @param bitMask маска для битов адреса (размер страницы), например: XMEM_32K
    @param start адрес начала eXternal SRAM в логическом пространстве
   */
  XPage(
    uint16_t physicalAddress = 0, // Начало физ. памяти
    byte bitMask = XMEM_60K,   // Всё доступное адресное пространство
    uint16_t start = XMEM_START   // Первый доступный адрес eXternal SRAM
  ) :
    _xmm(bitMask),
    _highAddress(highbyte(physicalAddress)),
    _lowAddress(lowbyte(physicalAddress))
  {
    init();
    use();
    Memory::init(start, XMEM_SIZE(bitMask, start));
  }

public:
  void use();
  void init();
};
