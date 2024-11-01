#include "xmem_page.h"

XmemPage::XmemPage()
{

}

void XmemPage::use()
{
  bitSet(PORTB, PB3);
}
