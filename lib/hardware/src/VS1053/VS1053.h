// #include <VS1053.h>
#pragma once
#include "config.h"
#include "error.h"
#include <spi/spi.h>

// Константы
////////////////////////////////////////////////////////////////////////////////

#define SCI_MIDI          0x0
#define SCI_WRITE         0x2
#define SCI_READ          0x3

// datasheet стр 37

#define SCI_MODE          0x0
#define SCI_STATUS        0x1
#define SCI_BASS          0x2
#define SCI_CLOCKF        0x3
#define SCI_DECODE_TIME   0x4
#define SCI_AUDATA        0x5
#define SCI_WRAM          0x6
#define SCI_WRAMADDR      0x7
#define SCI_HDAT0         0x8
#define SCI_HDAT1         0x9
#define SCI_AIADDR        0xA
#define SCI_VOL           0xB
#define SCI_AICTRL0       0xC
#define SCI_AICTRL1       0xD
#define SCI_AICTRL2       0xE
#define SCI_AICTRL3       0xF

#define SM_DIFF           0x0001
#define SM_LAYER12        0x0002
#define SM_RESET          0x0004
#define SM_CANCEL         0x0008
#define SM_EARSPEAKER_LO  0x0010
#define SM_TESTS          0x0020
#define SM_STREAM         0x0040
#define SM_EARSPEAKER_HI  0x0080
#define SM_DACT           0x0100
#define SM_SDIORD         0x0200
#define SM_SDISHARE       0x0400
#define SM_SDINEW         0x0800
#define SM_ADPCM          0x1000
#define SM_PAUSE          0x2000
#define SM_LINE1          0x4000
#define SM_CLK_RANGE      0x8000


#define END_PATCH         0xFFFF

class VS1053 {
public:
  void init();
  void set_volume(uint8_t, uint8_t);
  void set_volume(uint16_t);
  void set_volume(uint8_t);
  uint16_t get_volume();

  uint16_t read_register(uint8_t);

  void send_midi(byte);
  void send_midi(byte, byte);
  void send_midi(byte, byte, byte);


protected:
  byte _left_channel;
  byte _right_channel;

protected:
  void load_patch(const uint16_t *);


private:
  void write_register(uint8_t, uint8_t, uint8_t);
  void write_register(uint8_t, uint16_t);
};
