#pragma once

#include <Arduino.h>

class PrintFormat {
public:
  PrintFormat() {};

  void printf(const  __FlashStringHelper *, ...);
  void print(const char *, byte pos = 0);
  void print_pstr(const char *);

  void print(const __FlashStringHelper *);
  void print(uint16_t, byte pos = 0);
  void print(uint32_t, byte pos = 0);
  void print(int16_t);
  void print(int32_t);
  void print() {}

  void print_h(uint64_t);
  void print_h(uint32_t);
  void print_h(uint16_t);
  void print_h(byte);

  template <typename T>
  inline void println(T arg) { print(arg); write('\n'); }

public:
  virtual void write(byte) = 0;
  virtual void write(byte *, byte) = 0;

};
