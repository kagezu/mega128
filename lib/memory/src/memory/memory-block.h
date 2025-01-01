#include <Arduino.h>

#define MEM_USED    128
#define MEM_FIXED   64
#define MEM_LOCK    32
#define MEM_FREE    0

class MemoryBlock {
protected:
  uint16_t _link_to_index;
  uint16_t _start;
  uint16_t _size;
  volatile byte _status;
  byte _reserve;

public:
  MemoryBlock() {}
  void init(uint16_t start, uint16_t size = 0)
  {
    _link_to_index = 0;
    _start = start;
    _size = size;
    _status = MEM_FREE;
  }

public:
  inline void free() { *(uint16_t *)_link_to_index = 0; _link_to_index = 0; }

  inline void set_link(uint16_t linkToIndex) { _link_to_index = linkToIndex; *(uint16_t *)linkToIndex = _start; }
  inline void set_size(uint16_t size) { _size = size; }
  inline void set_start(uint16_t start) { _start = start; }

  inline void lock() { _status |= MEM_LOCK; }
  inline void unlock() { _status &= ~MEM_LOCK; }
  inline void fixed() { _status |= MEM_FIXED; }

  inline uint16_t getLink() { return _link_to_index; }
  inline uint16_t getSize() { return _size; }
  inline uint16_t getStart() { return _start; }
  inline byte getStatus() { return _status; }
};
