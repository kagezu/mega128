#include <event.h>
#include "display.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"

Display lcd;

class EventMidi : public Event {
public:
  byte key;
  byte chanel;
  byte velocity;

  EventMidi() { type = 0; }
};

int main()
{
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));
  lcd.clear();
  lcd.set_interline(3);
  lcd.font(&standard_5x8);

  EventMidi evt;

  evt.type = 1;
  evt.key = 2;



  lcd.printf(F("Event: %u byte  \n"), sizeof(EventMidi));
  lcd.printf(F("evt.type: %u  \n"), evt.type);
  lcd.printf(F("evt.key: %u  \n"), evt.key);
}
