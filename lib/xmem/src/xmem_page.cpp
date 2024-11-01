#include "xmem_page.h"

void XmemPage::use()
{
  bitSet(PORTB, PB3);
}
