#include "print-format.h"
#include <macros/accel.h>

void PrintFormat::printf(const __FlashStringHelper *p, ...)
{
  PGM_P string = reinterpret_cast<PGM_P>(p);
  char ch;
  va_list args;
  va_start(args, p);

  while ((ch = pgm_read_byte(string++))) {
    switch (ch) {
      case '%': {
          char arg = '0';
          byte pos = 0;
          ch = pgm_read_byte(string++);
          if (ch > '/' && ch < ':') {
            arg = ch;
            ch = pgm_read_byte(string++);
          }
          if (ch == '.') {
            pos = 1;
            ch = pgm_read_byte(string++);
          }
          switch (ch) {
            case 'c': write((char)va_arg(args, int16_t)); break;
            case 'i':
              switch (arg) {
                case '0':
                case '2': print((int16_t)va_arg(args, int16_t)); break;
                case '4': print((int32_t)va_arg(args, int32_t)); break;
              } break;
            case 's': print((char *)va_arg(args, char *)); break;
            case 'S': print_pstr((char *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '2': print((uint16_t)va_arg(args, uint16_t), pos); break;
                case '4': print((uint32_t)va_arg(args, uint32_t), pos); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  print_h((byte)va_arg(args, uint16_t)); break;
                case '2':  print_h((uint16_t)va_arg(args, uint16_t)); break;
                case '4':  print_h((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  print_h((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': print_h((uint16_t)va_arg(args, uint16_t)); break;
            case '%': write('%'); break;
          } break;
        }
      default: if ((byte)ch < 0xd0) write(ch);
    }
  }
  va_end(args);
}

void PrintFormat::print(const char *string, byte pos)
{
  if (pos)
    while (pos--) write('\b');
  while (char ch = *string++) if ((byte)ch < 0xd0) write(ch);
}

void PrintFormat::print_pstr(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((byte)ch < 0xd0) write(ch);
}

void PrintFormat::print(const __FlashStringHelper *string)
{
  PGM_P ptr = reinterpret_cast<PGM_P>(string);
  while (char ch = pgm_read_byte(ptr++)) if ((byte)ch < 0xd0) write(ch);
}

void PrintFormat::print(int32_t number)
{
  if (number < 0) { write('-'); number = -number; }
  print((uint32_t)number);
}

void PrintFormat::print(int16_t number)
{
  if (number < 0) { write('-'); number = -number; }
  print((uint16_t)number);
}

void PrintFormat::print(uint32_t number, byte pos)
{
  char str[11];
  char *ptr = &str[10];
  *ptr = 0;

  while (number > 9) {
    byte mod;

  #ifdef ACCEL
    byte tmp;
    div10_32bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif

    *--ptr = mod + '0';
    if (pos) pos++;
  }
  *--ptr = number + '0';
  print(ptr, pos);

}

void PrintFormat::print(uint16_t number, byte pos)
{
  char str[7];
  char *ptr = &str[6];
  *ptr = 0;

  while (number > 9) {
    byte mod;

  #ifdef ACCEL
    byte tmp;
    div10_16bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif

    *--ptr = mod + '0';
    if (pos) pos++;
  }
  *--ptr = number + '0';
  print(ptr, pos);
}

void PrintFormat::print_h(uint64_t number)
{
  union { uint64_t val; struct { byte a; byte b; byte c; byte d; byte e; byte f; byte g; byte h; }; } out;
  out.val = number;
  write('#');
  print_h(out.h);
  print_h(out.g);
  print_h(out.f);
  print_h(out.e);
  print_h(out.d);
  print_h(out.c);
  print_h(out.b);
  print_h(out.a);
}

void PrintFormat::print_h(uint32_t number)
{
  union { uint32_t val; struct { byte a; byte b; byte c; byte d; }; } out;
  out.val = number;
  write('#');
  print_h(out.d);
  print_h(out.c);
  print_h(out.b);
  print_h(out.a);
}

void PrintFormat::print_h(uint16_t number)
{
  union { uint16_t val; struct { byte low; byte high; }; } out;
  out.val = number;
  write('#');
  print_h(out.high);
  print_h(out.low);
}

void PrintFormat::print_h(byte number)
{
  byte low = number & 0xf;
  byte high = number >> 4;
  write(high > 9 ? high + '7' : high + '0');
  write(low > 9 ? low + '7' : low + '0');
}
