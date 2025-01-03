#include <display/display.h>
#include "memory/memory.h"
#include "font/arial_14.h"
#include "font/standard_5x7.h"

#define BUFFER_SIZE   1280
#define FRAGMENTATION 128

byte *ptr[FRAGMENTATION];
byte test_block[BUFFER_SIZE];
Memory mem((uint16_t)test_block, BUFFER_SIZE);
Display lcd;

void fill(byte *ptr, byte size)
{
  byte filler = rand();
  size >>= 1;
  while (size--) {
    *ptr = filler;
    ptr++;
    *ptr = filler >> 8;
    ptr++;
  }
}

void view()
{
  uint16_t count = BUFFER_SIZE >> 1;

  DISPLAY_CONNECT;
  lcd.set_addr(0, 0, 63, 159);

  while (count--) {
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
    lcd.send_byte(test_block[count * 2]);
    lcd.send_byte(test_block[count * 2 + 1]);
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
  while (true) {
    byte size = (rand() >> 8);
    // mem.free(ptr[i]);
    ptr[i] = (byte *)mem.get(size);
    fill(ptr[i], size);

    lcd.at(70, 25);
    lcd.printf(PSTR("%u    "), i);
    lcd.at(70, 35);
    lcd.printf(PSTR("%u    "), size);
    lcd.at(70, 45);
    lcd.printf(PSTR("%2x    "), ptr[i]);

    lcd.at(70, 65);
    lcd.printf(PSTR("%2x    "), mem._stack.head());
    lcd.at(70, 75);
    lcd.printf(PSTR("%2x    "), mem._stack.head()->get_start());
    lcd.at(70, 85);
    lcd.printf(PSTR("%2x    "), mem._stack.head()->get_size());

    view();

    i++;
    if (i == FRAGMENTATION) i = 0;

  }

}
