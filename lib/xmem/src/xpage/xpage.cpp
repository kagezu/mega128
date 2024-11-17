#include "xpage.h"

XPage::XPage(word physicalAddress, byte bitMask, word start)
{
  _highAddress = highByte(physicalAddress);
  _lowAddress = lowByte(physicalAddress);
  _xmm = bitMask;
  _start = start;
  // Размер страницы 512 байт не поддерживается аппаратно
  _end = bitMask ?
    start + _BV(bitMask == XMEM_0K ? 0 : 8 - bitMask) * XMEM_MIN_SIZE - 1 :
    XMEM_END;
  reset();
  init();
}

byte *XPage::malloc(word memorySize)
{
  if (free() < memorySize) return 0;
  word result = _offset;
  _offset += memorySize;
  return (byte *)result;
}

void XPage::use()
{
  XMCRB = _xmm;
  if (_xmm) PORTC = _lowAddress;
#ifdef XMEM_A16
  bitWrite(XMEM_A16_PORT, XMEM_A16, _highAddress & _BV(0));
#endif
#ifdef XMEM_A17
  bitWrite(XMEM_A17_PORT, XMEM_A17, _highAddress & _BV(1));
#endif
#ifdef XMEM_A18
  bitWrite(XMEM_A18_PORT, XMEM_A18, _highAddress & _BV(2));
#endif
}

void XPage::init()
{
  if (bitRead(MCUCR, SRE)) return;
  PORTC = 0;
  DDRC = 0xff;
  bitSet(MCUCR, SRE);
#if XMEM_A16
  XMEM_A16_INIT;
#endif
#if XMEM_A17
  XMEM_A17_INIT;
#endif
#if XMEM_A18
  XMEM_A18_INIT;
#endif
}

