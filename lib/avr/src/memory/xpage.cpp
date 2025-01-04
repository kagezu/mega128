#ifdef __AVR_ATmega128__
#include "xpage.h"

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

#endif
