#include <display.h>
#include "memory/memory.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"

// Использовать класс Memory напрямую
#define MEM_USE

#define FRAGMENTATION 25
#define WEIGHT        48
#define TEXT_X        WEIGHT + 5

byte *ptr[FRAGMENTATION];
byte s[FRAGMENTATION];
Display lcd;

void fill(byte *ptr, byte size, byte filler)
{
  if (ptr) {
    size >>= 1;
    while (size--) {
      *ptr = filler;
      ptr++;
      *ptr = filler;
      ptr++;
    }
  }
}
extern int __bss_end;
const uint16_t buffer_size = RAMEND - 0x100;

void view()
{
  uint16_t count = buffer_size;
  byte *ptr = (byte *)0x100;
  byte bpl = WEIGHT * MAX_Y / (count >> 1);

  DISPLAY_CONNECT;
  lcd.set_addr(0, 0, WEIGHT - 1, 159);

  while (count--) {
    for (byte i = 0; i < bpl; i++) {
      lcd.send_byte(*ptr);
    }
    ptr++;
  }

  DISPLAY_DISCONNECT;
}

int main()
{
  lcd.font(&standard_5x8);
  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  byte i = 0;
  uint16_t n = 0;
  uint32_t total = 0;
  uint32_t deny = 0;

  while (true) {
    byte rnd = rand() >> 8;
    byte size = (rnd * rnd >> 8) + 1;
    byte filler = rand();
    i = rand() % FRAGMENTATION;

    lcd.at(TEXT_X, 15);
    lcd.printf(F("count:"));
    lcd.at(TEXT_X, 25);
    lcd.printf(F("%2u "), n++);
    lcd.at(TEXT_X, 35);
    lcd.printf(F("size: %2u "), size);

  #ifdef MEM_USE
    lcd.at(TEXT_X, 45);
    lcd.printf(F("free:"));
    lcd.at(TEXT_X, 55);
    lcd.printf(F("%2u  "), memory.heap());
    lcd.at(TEXT_X, 65);
    lcd.printf(F("free: %u %%  "), (byte)(((uint32_t)memory.heap() * 100) / buffer_size));
  #endif

    fill(ptr[i], s[i], 0);

  #ifdef MEM_USE
    if (1)
      memory.free((void **)&ptr[i]);
    else
      memory.free(ptr[i]);
  #else
    free(ptr[i]);
  #endif

  #ifdef MEM_USE
    if (1)
      memory.malloc((void **)&ptr[i], size);
    else
      ptr[i] = memory.malloc(size);
  #else
    ptr[i] = (byte *)malloc(size);
  #endif

    s[i] = size;
    fill(ptr[i], s[i], filler);

    total += size;
    lcd.at(TEXT_X, 85);
    lcd.printf(F("total:"));
    lcd.at(TEXT_X, 95);
    lcd.printf(F("%4u "), total);

    if (ptr[i] == nullptr) deny += size;
    lcd.at(TEXT_X, 110);
    lcd.printf(F("deny:"));
    lcd.at(TEXT_X, 120);
    lcd.printf(F("%u %% "), (byte)((deny * 100) / total));

    view();
  }
}
