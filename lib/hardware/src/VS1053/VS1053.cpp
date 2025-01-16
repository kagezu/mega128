#include "VS1053.h"
#include <macros/helper.h>
#include "rtmidi.h"

#define SCI_BEGIN           \
                 \
X_DCS(CLR);

#define SCI_END             \
while (!X_DREQ(GET));       \
X_DCS(SET);                 \

void VS1053::init()
{
  X_DREQ(IN);
  X_CS(OUT);
  X_DCS(OUT);
  X_RESET(OUT);

  X_CS(SET);
  X_DCS(SET);
  X_RESET(CLR); // Сброс
  delay_ms(100);
  X_RESET(SET); // Отпускаем сброс
  spi.init(SCI_FQ_INIT);
  delay_ms(100);
  write_register(SCI_CLOCKF, SC_MULT | SC_ADD);
  // spi.init(SCI_FQ_MAX); // Ускоряем SPI
  set_volume(SCI_VOL_DEFAULT, SCI_VOL_DEFAULT);
  load_patch(rtmidi);
}

void VS1053::load_patch(const uint16_t *data)
{
  uint16_t addr, val, rep;

  while (true) {
    addr = pgm_read_word(data++);
    if (addr == END_PATCH) return;
    rep = pgm_read_word(data++);

    if (rep & 0x8000U) { // Заполняем одним значением
      rep &= 0x7FFF;
      val = pgm_read_word(data++);
      while (rep--) {
        write_register(addr, val);
      }
    }
    else {              // Копируем последовательность
      while (rep--) {
        val = pgm_read_word(data++);
        write_register(addr, val);
      }
    }
  }
}

void VS1053::set_volume(uint16_t data)
{
  dbyte val;
  val.word = data;
  set_volume(val.byte[1], val.byte[0]);
}

void VS1053::set_volume(uint8_t data)
{
  set_volume(data, data);
}

void VS1053::set_volume(uint8_t left_channel, uint8_t right_channel)
{
  _left_channel = left_channel;
  _right_channel = right_channel;
  write_register(SCI_VOL, left_channel, right_channel);
}

uint16_t VS1053::get_volume()
{
  return read_register(SCI_VOL);
}

// Трансфер функции
//////////////////////////////////////////////////////////////////////////

uint16_t VS1053::read_register(uint8_t addr)
{
  cli();
  X_CS(CLR);
  uint16_t result;
  SCI_BEGIN;
  spi.send(SCI_READ);
  spi.send(addr);
  result = spi.transfer(0xFFFF);
  SCI_END;
  X_CS(SET);
  sei();
  return result;
}

void VS1053::write_register(uint8_t addr, uint8_t high, uint8_t low)
{
  SCI_BEGIN; X_CS(CLR);
  spi.send(SCI_WRITE);
  spi.send(addr);
  spi.send(high);
  spi.send(low);
  SCI_END;
  X_CS(SET);
}

void VS1053::write_register(uint8_t addr, uint16_t data)
{
  dbyte val;
  val.word = data;
  write_register(addr, val.byte[1], val.byte[0]);
}

void VS1053::send_midi(byte msg)
{
  SCI_BEGIN;
  spi.send(SCI_MIDI);
  spi.send(msg);
  SCI_END;
}

void VS1053::send_midi(byte msg, byte data)
{
  SCI_BEGIN;
  spi.send(SCI_MIDI);
  spi.send(msg);
  spi.send(SCI_MIDI);
  spi.send(data);
  SCI_END;
}

void VS1053::send_midi(byte msg, byte data1, byte data2)
{
  SCI_BEGIN;
  spi.send(SCI_MIDI);
  spi.send(msg);
  spi.send(SCI_MIDI);
  spi.send(data1);
  spi.send(SCI_MIDI);
  spi.send(data2);
  SCI_END;
}
