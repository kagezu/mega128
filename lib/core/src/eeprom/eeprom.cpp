#include "eeprom.h"
#include "type/buffer.h"
#include "macros/attribute.h"

struct GCC_PACKED EEPROM_Frame {
  uint16_t address;
  byte value;
};

static Buffer<EEPROM_Frame, byte> _eep_buffer(EEP_BUFFER_SIZE);

void eeprom_write_buffer()
{
  if (_eep_buffer.length()) {// Если буфер не пустой, то продолжаем
    EEPROM_Frame frame = _eep_buffer.read();
    EEAR = frame.address;
    EECR |= _BV(EERE);
    byte test = EEDR;
    if (test != frame.value) { // Запись только нового значения
      EEDR = frame.value;
      EECR |= _BV(EEMPE);
      EECR |= _BV(EEPE);
    }
  }
}

void eeprom_write_byte(uint8_t *__p, uint8_t __value)
{
  byte _sreg = SREG;
  __asm__ __volatile__("cli" ::);
  if (!_eep_buffer.heap()) { // Полный буфер, прийдётся ждать
    while (EECR & _BV(EEPE));
    eeprom_write_buffer();
  }
  _eep_buffer.write(EEPROM_Frame { (uint16_t)__p,__value });
  if (!(EECR & _BV(EEPE))) {
    eeprom_write_buffer();
  }
  SREG = _sreg;
}

uint8_t eeprom_read_byte(const uint8_t *__p)
{
  byte value, _sreg = SREG;
  __asm__ __volatile__("cli" ::);
  while (EECR & _BV(EEPE));
  EEAR = (uint16_t)__p;
  EECR |= _BV(EERE);
  value = EEDR;
  eeprom_write_buffer();
  SREG = _sreg;
  return value;
}

ISR(EE_READY_vect)
{
  eeprom_write_buffer();
}
