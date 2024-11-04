#include <Arduino.h>
#include "config.h"

#ifndef ST7735S_H
#define ST7735S_H

#define RGB_12 0x03 // 4x4x4 bit
#define RGB_16 0x05 // 5x6x5 bit
#define RGB_18 0x06 // 6x6x6 bit (24 bit transfer)

#define MAX_X 127
#define MAX_Y 159

#define SET_BITS(target, mask) target |= mask;
#define RES_BITS(target, mask) target &= ~mask;

#define DISPLAY_DISCONNECT SET_BITS(LCD_PORT, LCD_CS) // Снять выбор дисплея
#define DATA_MODE SET_BITS(LCD_PORT, LCD_RS)          // Запись данных
#define SET_SDA SET_BITS(LCD_PORT, LCD_SDA)           // Данные
#define SET_SCK SET_BITS(LCD_PORT, LCD_SCK)           // Тактирование

#define DISPLAY_CONNECT RES_BITS(LCD_PORT, LCD_CS) // Выбор дисплея
#define COMMAND_MODE RES_BITS(LCD_PORT, LCD_RS)    // Запись команды
#define RES_SDA RES_BITS(LCD_PORT, LCD_SDA)        // Данные
#define RES_SCK RES_BITS(LCD_PORT, LCD_SCK)        // Тактирование

// Команды дисплея

#define NOP 0x00       // No Operation
#define SWRESET 0x01   // Software reset
#define RDDID 0x04     // Read Display ID
#define RDDST 0x09     // Read Display Status
#define RDDPM 0x0a     // Read Display Power
#define RDDMADCTL 0x0b // Read Display
#define RDDCOLMOD 0x0c // Read Display Pixel
#define RDDIM 0x0d     // Read Display Image
#define RDDSM 0x0e     // Read Display Signal
#define SLPIN 0x10     // Sleep in & booster off
#define SLPOUT 0x11    // Sleep out & booster on
#define PTLON 0x12     // Partial mode on
#define NORON 0x13     // Partial off (Normal)
#define INVOFF 0x20    // Display inversion off
#define INVON 0x21     // Display inversion on
#define GAMSET 0x26    // Gamma curve select              byte   [ GC3 GC2 GC1 GC0 ]
#define DISPOFF 0x28   // Display off
#define DISPON 0x29    // Display on
#define CASET 0x2a     // Column address set              start: dword      stop: dword
#define RASET 0x2b     // Row address set                 start: dword      stop: dword
#define RAMWR 0x2c     // Memory write
#define RAMRD 0x2e     // Memory read
#define PTLAR 0x30     // Partial start/end address set
#define TEOFF 0x34     // Tearing effect line off
#define TEON 0x35      // Tearing effect mode set & on
#define MADCTL 0x36    // Memory data access control
#define IDMOFF 0x38    // Idle mode off
#define IDMON 0x39     // Idle mode on
#define COLMOD 0x3a    // Interface pixel format
#define RDID1 0xda     // Read ID1
#define RDID2 0xdb     // Read ID2
#define RDID3 0xdc     // Read ID3

// Panel Function Command List and Description

#define FRMCTR1 0xb1  // In normal mode (Full colors)
#define FRMCTR2 0xb2  // In Idle mode (8-colors)
#define FRMCTR3 0xb3  // In partial mode + Full colors
#define INVCTR 0xb4   // Display inversion control        byte  [ NLA NLB NLC ] set inversion
#define DISSET5 0xb6  // Display function setting
#define PWCTR1 0xc0   // Power control setting
#define PWCTR2 0xc1   // Power control setting
#define PWCTR3 0xc2   // In normal mode (Full colors)
#define PWCTR4 0xc3   // In Idle mode (8-colors)
#define PWCTR5 0xc4   // In partial mode + Full colors
#define VMCTR1 0xc5   // VCOM control 1
#define VMOFCTR 0xc7  // Set VCOM offset control
#define WRID2 0xd1    // Set LCM version code
#define WRID3 0xd2    // Customer Project code
#define PWCTR6 0xfc   // In partial mode + Idle
#define NVCTR1 0xd9   // EEPROM control status
#define NVCTR2 0xde   // EEPROM Read Command
#define NVCTR3 0xdf   // EEPROM Write Command
#define GAMCTRP1 0xe0 // Set Gamma adjustment (+ polarity)
#define GAMCTRN1 0xe1 // Set Gamma adjustment (- polarity)
#define EXTCTRL 0xf0  // Extension Command Control
#define VCOM4L 0xff   // Vcom 4 Level control

class ST7735S {
public:
  ST7735S();
  void sendCommand(byte data);
  void setAddr(byte x1, byte y1, byte x2, byte y2);
  void sendZero();
  void sendByte(byte data);
  void sendRGB(byte r, byte g, byte b);

#if RGB_FORMAT == RGB_12
  void send12b(word data);
#elif RGB_FORMAT == RGB_16
  void sendWord(word data);
#endif

  void pixel(byte x, byte y, word color);
  void pixel(byte x, byte y, byte r, byte g, byte b);
  void rect(byte x1, byte y1, byte x2, byte y2, uint32_t color);

  inline void clear(uint32_t color) { rect(0, 0, MAX_X, MAX_Y, color); };

  void symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy);

  // Тесты
  void demo(byte d);
};

#endif
