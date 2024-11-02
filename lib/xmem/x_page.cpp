#include "x_page.h"

XPage::XPage(byte numberPage, word offset) { _nPage = numberPage;  init(offset); }
inline void XPage::init(word offset) { _offset = offset; }
inline word XPage::free() { return XMEM_END - _offset; }
void XPage::use() { XMEM_PAGE(_nPage); }

void *XPage::create(word memorySize)
{
  if (free() < memorySize) return 0;
  word result = _offset;
  _offset += memorySize;
  return (void *)result;
}
