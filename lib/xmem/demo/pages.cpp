#include "xpage.h"

XPage page0(0x000, XMEM_60K, 0x1000);
XPage page1(0x100, XMEM_60K, 0x1100);
XPage page2(0x000, XMEM_4K, 0x1100);
XPage page3(0x100, XMEM_4K, 0x1100);
XPage page4(0x010, XMEM_0K, 0x1100);
XPage page5(0x110, XMEM_0K, 0x1100);

typedef uint16_t(&DIM)[100][100];
DIM dim = (DIM)*page1.malloc(sizeof(DIM));

int main()
{
  page1.use();
  dim[40][50];

}
