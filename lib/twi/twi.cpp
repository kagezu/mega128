#include "twi.h"

volatile uint8_t TWI_Master::_state;
volatile uint8_t TWI_Master::_master_address;
volatile uint16_t TWI_Master::_master_index;

volatile uint8_t *TWI_Master::_master_buffer;
volatile uint16_t TWI_Master::_master_buffer_length;


void TWI_Master::begin()
{
  _state = TWI_IDLE;
  // установка частоты F = F_CPU/(( 4 ^ TWSR[0:1] ) * TWBR * 2 + 16 )
  TWSR &= ~3; // Предполагается частота выше 30 кГц
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
  TWI_DDR |= TWI_SDA | TWI_SCL; // Устанавливаем пины как output
  TWCR = TWI_ENABLE | TWI_ACK | TWI_ISR; // Включаем модуль TWI
}

void TWI_Master::end(void)
{
  TWCR &= ~(TWI_ENABLE | TWI_ISR | TWI_ACK); // Отключаем модуль TWI
  TWI_DDR &= ~(TWI_SDA | TWI_SCL); // Устанавливаем пины как input
}

void TWI_Master::set_freq(uint32_t freq)
{
  TWBR = ((F_CPU / freq) - 16) / 2;
}

void TWI_Master::write(byte *data, uint16_t length)
{
  while (_state & TWI_BUSY);
  if (_index) {
    _state |= TWI_BUSY;
    _master_index = 0;
    _master_buffer_length = _index;
    _master_buffer = _buffer;
    _master_address = _address;
    _index = 0;
    if (_state & TWI_ST)
      TWCR = TWI_INT | TWI_ACK | TWI_ENABLE | TWI_ISR;
    else
      TWCR = TWI_INT | TWI_ACK | TWI_START | TWI_ENABLE | TWI_ISR;
  }

  if (_buffer != data) {
    while (_state & TWI_BUSY);
    _state |= TWI_BUSY;
    _master_index = 0;
    _master_buffer_length = length;
    _master_buffer = data;
    _master_address = _address;
    if (_state & TWI_ST)
      TWCR = TWI_INT | TWI_ACK | TWI_ENABLE | TWI_ISR;
    else
      TWCR = TWI_INT | TWI_ACK | TWI_START | TWI_ENABLE | TWI_ISR;
  }
}

void TWI_Master::write(byte data)
{
  if (_index >= BUFFER_LENGTH) {
    write(_buffer, _index);
    _index = 0;
  }
  _buffer[_index++] = data;
}

void TWI_Master::beginTransmission()
{
  while (_state);
}

void TWI_Master::endTransmission(void)
{
  _state |= TWI_STOP;
  write(_buffer, _index);
  _index = 0;
}

void TWI_Master::send_address()
{
  _state |= TWI_ST;
  TWDR = _master_address;
  TWCR = TWI_INT | TWI_ACK | TWI_ENABLE | TWI_ISR;
}

void TWI_Master::stop(void)
{
  while (_state & TWI_BUSY);
  TWCR = TWI_INT | TWI_ACK | TWI_STOP | TWI_ENABLE | TWI_ISR;
  while (TWCR & _BV(TWSTO));
  _state = TWI_IDLE;
}

void TWI_Master::send_next_byte(void)
{
  if (_master_index < _master_buffer_length) {
    TWDR = _master_buffer[_master_index++];
    TWCR = TWI_INT | TWI_ACK | TWI_ENABLE | TWI_ISR;
  }
  else {
    _state &= ~TWI_BUSY;
    if (_state & TWI_STOP) stop();
  }
}

ISR(TWI_vect)
{
  switch (TWSR & TWI_TWSR_MASK) {

    case TWI_ST:
    case TWI_REP:
      TWI_Master::send_address();
      break;
    case TWI_ACK_W:
    case TWI_DATA_ACK:
      TWI_Master::send_next_byte();
      break;
    case TWI_NACK:
      TWI_Master::stop();
      break;
    case TWI_DATA_NACK:
      TWI_Master::stop();
      break;
    case TWI_EMPTY:
      break;
    case TWI_ERROR:
      TWI_Master::stop();
      break;
  }
}
