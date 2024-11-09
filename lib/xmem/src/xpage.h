#ifndef XPAGE_H
#define XPAGE_H

#include "config.h"

#ifdef XMEM_USE
#define XMEM_START      RAMEND+1
#define XMEM_END        XRAMEND

#elif __AVR_ATmega328P__
// Профиль используется для тестирования и совместимости
#define XMEM_START      0x400
#define XMEM_END        RAMEND
#endif

// Физически возможный минимальный размер страницы
// Соответствует XMM[0-2] = 7 в регистре XMCRB
#define XMEM_MIN_SIZE 0x100

#define XMEM_60K  0
#define XMEM_32K  1
#define XMEM_16K  2
#define XMEM_8K   3
#define XMEM_4K   4
#define XMEM_2K   5
#define XMEM_1K   6
#define XMEM_0K   7   // 256 байт

class XPage {
private:
  byte _xmm; // Значение битов XMM[0-2] в регистре XMCRB
  word _start;
  word _end;
  word _offset;
  byte _highAddress;
  byte _lowAddress;

public:
  /**
    @param physicalAddress адрес физической памяти (старшие байты)
    @param bitMask маска для битов адреса, например: XMEM_32K
    @param start адрес начала eXternal SRAM
   */
  XPage(word physicalAddress = 0, byte bitMask = XMEM_60K, word start = XMEM_START);
  byte *create(word memorySize);

public:
  inline void reset() { _offset = _start; }
  inline word free() { return _end - _offset; }

public:
#ifdef XMEM_USE
  inline void use();
  static void init();
#else
  inline  void use() {};
  static  void init() {};
#endif
};
#endif
