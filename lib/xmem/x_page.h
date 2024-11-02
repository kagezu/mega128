#include "setup.h"

#ifndef X_PAGE_H
#define X_PAGE_H

#if defined(XMEM_1K_X1)
#define XMEM_BEGIN      0x400
#define XMEM_END        0x800
#define XMEM_INIT
#define XMEM_PAGE(page)

#elif defined(XMEM_60K_X2)
#define XMEM_BEGIN      0x1100
#define XMEM_END        0x10000
#define XMEM_INIT             \
          bitSet(MCUCR, SRE)  \
          XMEM_A16_INIT
#define XMEM_PAGE(page) bitWrite(XMEM_A16_PORT, XMEM_A16, page)

#elif defined(XMEM_32K_X4)
#define XMEM_BEGIN      0x1100
#define XMEM_END        0x9100
#define XMEM_INIT             \
          bitSet(MCUCR, SRE)  \
          bitSet(XMCRB, XMM0) \
          XMEM_A16_INIT       \
          XMEM_A15_INIT
#define XMEM_PAGE(page)                                    \
          bitWrite(XMEM_A16_PORT, XMEM_A16, page & _BV(1)) \
          bitWrite(XMEM_A15_PORT, XMEM_A15, page & _BV(0)) 
#endif

class XPage {
private:
  byte _nPage;
  word _offset;

public:
  XPage(byte numberPage = 0, word offset = XMEM_BEGIN);
  inline void init(word offset = XMEM_BEGIN);
  inline word free();
  void use();
  void *create(word memorySize);

};

#endif
