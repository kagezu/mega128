#include <display/display.h>
#include "memory/memory.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

// Использовать класс Memory
// #define MEM_USE

#define FACTOR        11
#define BUFFER_SIZE   160 * FACTOR
#define FRAGMENTATION 35
#define TEXT_X        40
#define WEIGHT        30

#ifdef MEM_USE
byte test_block[BUFFER_SIZE];
Memory mem((uint16_t)test_block, BUFFER_SIZE);
#else
byte test_block[1];
#endif

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

void view()
{
  uint16_t count = BUFFER_SIZE;
  byte *ptr = test_block;

  DISPLAY_CONNECT;
  lcd.set_addr(0, 0, WEIGHT - 1, 159);

  while (count--) {
    for (byte i = 0; i < (WEIGHT << 1) / FACTOR; i++) {
      lcd.send_byte(*ptr);
    }
    ptr++;
  }

  DISPLAY_DISCONNECT;
}

int main()
{
  lcd.font(&standard_5x7);
  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  byte i = 0;
  uint32_t total = 0;
  uint32_t deny = 0;

  while (true) {
    byte rnd = rand() >> 8;
    byte size = (rnd * rnd >> 7) + 1;
    byte filler = rand();
    i = rand() % FRAGMENTATION;

    lcd.at(TEXT_X, 25);
    lcd.printf(PSTR("i: %u  "), i);
    lcd.at(TEXT_X, 35);
    lcd.printf(PSTR("size: %2u "), size);

  #ifdef MEM_USE
    lcd.at(TEXT_X, 45);
    lcd.printf(PSTR("free: %2u  "), mem.heap());
    lcd.at(TEXT_X, 55);
    lcd.printf(PSTR("free: %u %%  "), (byte)(((uint32_t)mem.heap() * 100) / (BUFFER_SIZE)));
  #endif

    fill(ptr[i], s[i], 0);

  #ifdef MEM_USE
    if (1)
      mem.free((void **)&ptr[i]);
    else
      mem.free(ptr[i]);
  #else
    free(ptr[i]);
  #endif

  #ifdef MEM_USE
    if (1)
      mem.malloc((void **)&ptr[i], size);
    else
      ptr[i] = mem.malloc(size);
  #else
    ptr[i] = (byte *)malloc(size);
  #endif

    s[i] = size;
    fill(ptr[i], s[i], filler);

    total += size;
    lcd.at(TEXT_X, 85);
    lcd.printf(PSTR("total: %4u "), total);

    if (ptr[i] == nullptr) deny += size;
    lcd.at(TEXT_X, 95);
    lcd.printf(PSTR("deny: %4u "), deny);

    view();

    i++;
    if (i == FRAGMENTATION) i = 0;

  }

}
