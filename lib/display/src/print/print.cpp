#include "print.h"
#include <macros/context.h>

void Print::printf(const char *string, ...)
{
  char ch;
  va_list args;
  va_start(args, string);

  while ((ch = pgm_read_byte(string++))) {
    switch (ch) {
      case '%': {
          char arg = '0';
          ch = pgm_read_byte(string++);
          if (ch > '/' && ch < ':') {
            arg = ch;
            ch = pgm_read_byte(string++);
          }
          switch (ch) {
            case 'c': write((char)va_arg(args, int16_t)); break;
            case 'd': print((int8_t)va_arg(args, int16_t)); break;
            case 'i': print((int16_t)va_arg(args, int16_t)); break;
            case 's': print((char *)va_arg(args, char *)); break;
            case 'S': print_pstr((char *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '1': print((byte)va_arg(args, word)); break;
                case '2': print((word)va_arg(args, word)); break;
                case '4': print((uint32_t)va_arg(args, uint32_t)); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  print_h((byte)va_arg(args, word)); break;
                case '2':  print_h((word)va_arg(args, word)); break;
                case '4':  print_h((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  print_h((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': print_h((word)va_arg(args, word)); break;
            case '%': write('%'); break;
          } break;
        }
      default: if ((byte)ch < 0xd0) write(ch);
    }
  }
  va_end(args);
}

void Print::print(const char *string)
{
  while (char ch = *string++) if ((byte)ch < 0xd0) write(ch);
}

void Print::print_pstr(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((byte)ch < 0xd0) write(ch);
}

void Print::print(int32_t number)
{
  if (number < 0) { write('-'); number = -number; }
  print((uint32_t)number);
}

void Print::print(int16_t number)
{
  if (number < 0) { write('-'); number = -number; }
  print((word)number);
}

void Print::print(int8_t number)
{
  if (number < 0) { write('-'); number = -number; }
  print((byte)number);
}

void Print::print(uint32_t number)
{
  static const uint32_t mult[] PROGMEM = { 1000000000,100000000,10000000,1000000,100000,10000,1000,100,10,1 };
  byte out = 0;

  for (byte j = 0; j < 10; j++) {
    byte m = pgm_read_byte(mult + j);
    byte n = number / m;
    number %= m;
    // number -= m * n;
    if (n) out = 1;
    if (out) write('0' + n);
  }
}

void Print::print(word number)
{
  static const word mult[] PROGMEM = { 10000,1000,100,10,1 };
  byte out = 0;

  for (byte j = 0; j < 5; j++) {
    byte m = pgm_read_byte(mult + j);
    byte n = number / m;
    number %= m;
    // number -= m * n;
    if (n) out = 1;
    if (out) write('0' + n);
  }
}

void Print::print(byte number)
{
  static const byte mult[] PROGMEM = { 100,10,1 };
  byte out = 0;

  for (byte j = 0; j < 3; j++) {
    byte m = pgm_read_byte(mult + j);
    byte n = number / m;
    number %= m;
    // number -= m * n;
    if (n) out = 1;
    if (out) write('0' + n);
  }
}

void Print::print_h(uint64_t number)
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

void Print::print_h(uint32_t number)
{
  union { uint32_t val; struct { byte a; byte b; byte c; byte d; }; } out;
  out.val = number;
  write('#');
  print_h(out.d);
  print_h(out.c);
  print_h(out.b);
  print_h(out.a);
}

void Print::print_h(word number)
{
  union { word val; struct { byte low; byte high; }; } out;
  out.val = number;
  write('#');
  print_h(out.high);
  print_h(out.low);
}

void Print::print_h(byte number)
{
  byte low = number & 0xf;
  byte high = number >> 4;
  write(high > 9 ? high + '7' : high + '0');
  write(low > 9 ? low + '7' : low + '0');
}
