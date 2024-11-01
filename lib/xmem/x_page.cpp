#include "x_page.h"

XPage::XPage(byte numberPage) { _nPage = numberPage;  init(); }
inline void XPage::init() { _offset = XMEM_BEGIN; }
inline word XPage::free() { return XMEM_END - _offset; }
void XPage::use() { XMEM_PAGE(_nPage); }

void *XPage::create(word memorySize)
{
  if (free() < memorySize) return 0;
  word result = _offset;
  _offset += memorySize;
  return (void *)result;
}
