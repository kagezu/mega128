#include "VS1053.h"
#include <macros/helper.h>
#include "rtmidi.h"

#define WAIT_DREQ     while (!X_DREQ(GET));

// Инициализация
//////////////////////////////////////////////////////////////
void VS1053::init()
{
  X_DREQ(IN);
  X_CS(OUT);
  X_DCS(OUT);
  X_RESET(OUT);

  X_CS(SET);
  X_DCS(SET);
  X_RESET(CLR); // Сброс
  delay_ms(10);
  X_RESET(SET); // Отпускаем сброс
  spi.init(SCI_FQ_INIT);
  delay_ms(100);
  write_register(SCI_CLOCKF, SC_MULT | SC_ADD | SC_FREQ);
  spi.init(SCI_FQ_MAX); // Ускоряем SPI
  set_master(SCI_VOL_DEFAULT);
  load_patch(rtmidi);
}



// Управление
////////////////////////////////////////////////////////////////////////








// Трансфер функции
//////////////////////////////////////////////////////////////////////////

uint16_t VS1053::read_register(uint8_t addr)
{
  uint16_t result;
  cli();
  X_CS(CLR);
  spi.send(SCI_READ);
  spi.send(addr);
  result = spi.transfer(0xFFFF);
  WAIT_DREQ;
  X_CS(SET);
  sei();
  return result;
}

void VS1053::write_register(uint8_t addr, uint16_t data)
{
  X_CS(CLR);
  spi.send(SCI_WRITE);
  spi.send(addr);
  spi.send(to_byte(data, 1));
  spi.send(to_byte(data, 0));
  WAIT_DREQ;
  X_CS(SET);
}

void VS1053::send_midi(byte msg)
{
  X_DCS(CLR);
  spi.send(SCI_MIDI);
  spi.send(msg);
  X_DCS(SET);
}

void VS1053::send_midi(byte msg, byte data)
{
  X_DCS(CLR);
  spi.send(SCI_MIDI);
  spi.send(msg);
  spi.send(SCI_MIDI);
  spi.send(data);
  X_DCS(SET);
}

void VS1053::send_midi(byte msg, byte data1, byte data2)
{
  X_DCS(CLR);
  spi.send(SCI_MIDI);
  spi.send(msg);
  spi.send(SCI_MIDI);
  spi.send(data1);
  spi.send(SCI_MIDI);
  spi.send(data2);
  X_DCS(SET);
}

// Приватные функции
/////////////////////////////////////////////////////////////

void VS1053::load_patch(const uint16_t *data)
{
  uint16_t addr, val, rep;
  while (true) {
    addr = pgm_read_word(data++);
    if (addr == END_PATCH) return;
    rep = pgm_read_word(data++);

    if (rep & 0x8000U) {  // Заполняем одним значением
      rep &= 0x7FFF;
      val = pgm_read_word(data++);
      while (rep--) write_register(addr, val);
    }
    else                  // Копируем последовательность
      while (rep--) write_register(addr, pgm_read_word(data++));
  }
}

byte VS1053::sum_vol(char vol_ch)
{
  int16_t vol = (int16_t)_vol_master + vol_ch;
  return vol < 0 ? 0 : vol>0xfe ? 0xfe : vol;
}

void VS1053::set_volume()
{
  dbyte volume;
  volume.low = sum_vol(_vol_right);
  volume.high = sum_vol(_vol_left);
  write_register(SCI_VOL, volume.word);
}

void VS1053::get_volume()
{
  dbyte vol;
  vol.word = read_register(SCI_VOL);
  _vol_master = ((uint16_t)vol.low + vol.high) >> 2;
  _vol_right = _vol_master - vol.low;
  _vol_left = _vol_master - vol.high;
}
