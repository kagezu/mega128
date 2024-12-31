#pragma once

#include <Arduino.h>

class PrintFormat {
public:
  PrintFormat() {};

  void printf(const char *string, ...);
  void print(const char *);
  void print_pstr(const char *);

  void print(byte);
  void print(word);
  void print(uint32_t);
  void print(int8_t);
  void print(int16_t);
  void print(int32_t);

  void print_h(uint64_t);
  void print_h(uint32_t);
  void print_h(word);
  void print_h(byte);

  template <typename T>
  inline void println(T arg) { print(arg); write('\n'); }

public:
  virtual void write(byte);

};
