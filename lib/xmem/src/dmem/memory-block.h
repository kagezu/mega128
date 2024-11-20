#include <Arduino.h>

class MemoryBlock {
protected:
  void *_linkToIndex;
  uint16_t _start;
  uint16_t _size;

public:
  MemoryBlock(uint16_t start, uint16_t length = 0, void *linkToIndex = 0)
    :_linkToIndex(linkToIndex), _start(start), _size(length)
  {}

public:
  void setLink(void *linkToIndex) { _linkToIndex = linkToIndex; *(uint16_t *)linkToIndex = _start; }
  void newSize(uint16_t size) { _size = size; }

  void *getLink() { return _linkToIndex; }
  uint16_t getSize() { return _size; }
};
