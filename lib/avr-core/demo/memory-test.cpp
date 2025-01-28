#include <memory/allocate.h>
#include <display.h>
#include "font/arial_14.h"
#include "font/standard_5x8.h"

// Использовать класс Memory напрямую
#define MEM_USE

#define FRAGMENTATION 35
#define WEIGHT        48
#define TEXT_X        WEIGHT + 5

uint8_t *ptr[FRAGMENTATION];
uint8_t s[FRAGMENTATION];
Display lcd;

void fill(uint8_t *ptr, uint8_t size, uint8_t filler)
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
  uint8_t *ptr = (uint8_t *)0x100;
  uint8_t w = count / MAX_Y;

  lcd.bitmap(ptr, 0, 0, w, MAX_Y);
}

int main()
{
  lcd.font(&standard_5x8);
  lcd.set_interline(3);
  lcd.clear(RGB(0, 0, 64));
  lcd.background(RGB(0, 0, 64));
  lcd.color(RGB(255, 255, 127));

  uint8_t i = 0;
  uint16_t n = 0;
  uint32_t total = 0;
  uint32_t deny = 0;

  while (true) {
    uint8_t rnd = rand() >> 8;
    uint8_t size = (rnd * rnd >> 8) + 1;
    uint8_t filler = rand();
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
    lcd.printf(F("free: %u %%  "), (uint8_t)(((uint32_t)memory.heap() * 100) / buffer_size));
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
    ptr[i] = (uint8_t *)malloc(size);
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
    lcd.printf(F("%u %% "), (uint8_t)((deny * 100) / total));

    view();
  }
}
