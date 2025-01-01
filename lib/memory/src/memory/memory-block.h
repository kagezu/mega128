#include <Arduino.h>

#define MEM_USED      128
#define MEM_FIXED     64
#define MEM_LOCK      32
#define MEM_FREE      0

class MemoryBlock {
protected:
  uint16_t _link_to_ptr;
  uint16_t _start;
  uint16_t _size;
  volatile byte _status;
  // byte _reserve;

public:
  MemoryBlock() {}
  void init(uint16_t start, uint16_t size = 0)
  {
    _start = start;
    _size = size;
    _status = MEM_FREE;
  }

public:
  void used() { _status = MEM_USED | MEM_LOCK; }
  void used(uint16_t link_to_ptr)
  {
    _link_to_ptr = link_to_ptr;
    *(uint16_t *)link_to_ptr = _start;
    _status = MEM_USED;
  }
  void free()
  {
    if (!(_status & MEM_FIXED)) *(uint16_t *)_link_to_ptr = 0;
    _status = MEM_FREE;
  }

  void set_start(uint16_t start) { _start = start; }
  void set_size(uint16_t size) { _size = size; }

  uint16_t get_link() { return _link_to_ptr; }
  uint16_t get_start() { return _start; }
  uint16_t get_size() { return _size; }

  bool is_locked() { return _status & (MEM_LOCK | MEM_FIXED); }
  void lock() { _status |= MEM_LOCK; }
  void unlock() { _status &= ~MEM_LOCK; }
  void fixed() { _status |= MEM_FIXED; }
};
