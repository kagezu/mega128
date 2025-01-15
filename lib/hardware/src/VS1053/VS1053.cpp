#include "VS1053.h"
#include <macros/helper.h>
#include "rtmidi.h"

#define SCI_BEGIN           \
if (X_RESET(GET)) return;   \
while (!X_DREQ(GET));       \
X_CS(CLR);                  \
X_DCS(CLR);

#define SCI_END             \
while (!X_DREQ(GET));       \
X_DCS(SET);                 \
X_CS(SET);

void VS1053::begin()
{
  X_DREQ(IN);
  X_CS(OUT);
  X_DCS(OUT);
  X_RESET(OUT);

  X_CS(SET);
  X_DCS(SET);
  X_RESET(CLR);

  init();
}

void VS1053::end()
{
  X_CS(SET);
  X_DCS(SET);
  X_RESET(CLR);
}

void VS1053::init()
{
  X_RESET(CLR); // Сброс
  delay_ms(100);
  X_RESET(SET); // Отпускаем сброс
  spi.init(SCI_FQ_INIT);
  delay_ms(100);
  // uint16_t data = read_register(SCI_MODE);
  // if (data != (SM_LINE1 | SM_SDINEW)) return; // Статус отличен от начального
  write_register(SCI_CLOCKF, SC_MULT | SC_ADD);
  delay_us(200); // Длительность установки множителя 1200 XTALI
  // data = read_register(SCI_CLOCKF);
  // if (data != SC_MULT | SC_ADD) return; // Не удалось установить множитель
  spi.init(SCI_FQ_MAX); // Ускоряем SPI
  set_volume(SCI_VOL_DEFAULT, SCI_VOL_DEFAULT);
  load_patch(rtmidi);
  delay_ms(100);
}

void VS1053::load_patch(const uint16_t *data)
{
  uint16_t addr, val, rep, size = pgm_read_word(data++);
  if (X_RESET(GET)) return;

  while (size--) {
    addr = pgm_read_word(data++);
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
  dbyte result;
  SCI_BEGIN;
  spi.transfer(SCI_READ);
  spi.transfer(addr);
  result.byte[1] = spi.transfer(0xFF);
  while (!X_DREQ(GET));
  result.byte[0] = spi.transfer(0xFF);
  SCI_END;
  return result.word;
}

void VS1053::write_register(uint8_t addr, uint8_t high, uint8_t low)
{
  SCI_BEGIN;
  spi.transfer(SCI_WRITE);
  spi.transfer(addr);
  spi.transfer(high);
  spi.transfer(low);
  SCI_END;
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
  spi.transfer(SCI_MIDI);
  spi.transfer(msg);
  SCI_END;
}

void VS1053::send_midi(byte msg, byte data)
{
  SCI_BEGIN;
  spi.transfer(SCI_MIDI);
  spi.transfer(msg);
  spi.transfer(SCI_MIDI);
  spi.transfer(data);
  SCI_END;
}

void VS1053::send_midi(byte msg, byte data1, byte data2)
{
  SCI_BEGIN;
  spi.transfer(SCI_MIDI);
  spi.transfer(msg);
  spi.transfer(SCI_MIDI);
  spi.transfer(data1);
  spi.transfer(SCI_MIDI);
  spi.transfer(data2);
  SCI_END;
}
