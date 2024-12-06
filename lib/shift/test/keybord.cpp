#include "keybord.h"

char n = 0;
char k;
uint8_t keys[8];
uint8_t old[8];

void copy64(uint8_t *t, uint8_t *s)
{
  uint8_t i = 8;
  while (i--) *t++ = *s++;
}

char comp64(uint8_t *t, uint8_t *s)
{
  for (uint8_t i = 0; i < 8; i++, t++, s++) {
    uint8_t d = *t ^ *s;
    if (d) {
      char sign = -1;
      if (d & *s) sign = 1;
      for (uint8_t j = 1; j < 9; j++)
        if (d & 1) return sign * ((i << 3) + j);
        else d >>= 1;
    }
  }
  return 0;
}

int main()
{
  inits();


  while (true) {
    key.reset();
    key.readBytes(keys, sizeof(keys));

    n = comp64(old, keys);
    copy64(old, keys);
    if (n > 0) {
      psg.note(n);
      k = n;
    }

    // lcd.clear(RGB(0, 0, 64));
    text.printf(PSTR("\f  Keyboard  16-keys\n"));
    text.printf(PSTR("i: %x%x%x%x%x%x%x%x \n"), keys[7], keys[6], keys[5], keys[4], keys[3], keys[2], keys[1], keys[0]);
    text.printf(
      PSTR("a: %x%x%x%x%x%x%x%x \n\n"),
      old[7], old[6], old[5], old[4], old[3], old[2], old[1], old[0]);

    uint64_t x = *(uint64_t *)keys;
    char piano[66];
    piano[0] = ' ';
    piano[65] = 0;
    for (byte i = 64; i; i--) {
      if (x & 1) piano[i] = '!';
      else if (i % 8) piano[i] = '.';
      else piano[i] = ',';
      x >>= 1;
    }

    text.printf(PSTR("%s\n"), piano);
    text.printf(PSTR("Key number: %u    \n"), k);
  }

}
