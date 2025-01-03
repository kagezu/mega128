#include <display/display.h>
#include "memory/memory.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

// Использовать класс Memory
#define MEM_USE

#define FACTOR        11
#define BUFFER_SIZE   160 * FACTOR
#define FRAGMENTATION 25
#define TEXT_X        40
#define WEIGHT        32

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
  size >>= 1;
  while (size--) {
    *ptr = filler;
    ptr++;
    *ptr = filler;
    ptr++;
  }
}

void view()
{
  uint16_t count = BUFFER_SIZE >> 1;
  byte *ptr = test_block;

  DISPLAY_CONNECT;
  lcd.set_addr(0, 0, WEIGHT - 1, 159);

  while (count--) {
    for (byte i = 0; i < (WEIGHT << 1) / FACTOR; i++) {
      lcd.send_byte(*ptr); lcd.send_byte(*(ptr + 1));
    }
    ptr++;
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
  while (true) {
    byte size = rand() >> 9;
    byte filler = rand();

    lcd.at(TEXT_X, 25);
    lcd.printf(PSTR("i: %u  "), i);
    lcd.at(TEXT_X, 35);
    lcd.printf(PSTR("size: %2u "), s[i]);

  #ifdef MEM_USE
    lcd.at(TEXT_X, 45);
    lcd.printf(PSTR("free: %2u "), mem.get_heap());
  #endif

  #ifdef MEM_USE
    mem.free(ptr[i]);
  #else
    free(ptr[i]);
  #endif
    fill(ptr[i], s[i], 0);

  #ifdef MEM_USE
    ptr[i] = (byte *)mem.get(size);
  #else
    ptr[i] = (byte *)malloc(size);
  #endif
    s[i] = size;
    fill(ptr[i], s[i], filler);

    total += size;
    lcd.at(TEXT_X, 85);
    lcd.printf(PSTR("total: %4u "), total);

    view();

    i++;
    if (i == FRAGMENTATION) i = 0;

  }

}
