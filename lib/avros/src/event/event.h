#include <Arduino.h>

#define EVENT_BUFFER_SIZE   100

class Event {
public:
  byte type;
  byte data;
};

class Buffer {
  byte first;
  byte last;
};

class EventManager : public Buffer {
public:
  Event buffer[EVENT_BUFFER_SIZE];
};

