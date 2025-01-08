
#pragma once
#include <Arduino.h>

#ifdef  __AVR_ATmega328P__
#define TWI_SDA     _BV(PC4)
#define TWI_SCL     _BV(PC5)
#define TWI_DDR      DDRC
#endif

#define TWI_FREQ 400000
#define TWI_BUFFER_LENGTH 32

//==========================================

#define TWI_IDLE    0
#define TWI_BUSY    1
#define TWI_WRITE   2

#define TWI_ENABLE    _BV(TWEN)
#define TWI_ACK       _BV(TWEA)
#define TWI_ISR       _BV(TWIE)
#define TWI_START     _BV(TWSTA)
#define TWI_STOP      _BV(TWSTO)
#define TWI_INT       _BV(TWINT)

// Состояние СТАРТ
#define TWI_ST  		    0x08
// Повторный старт
#define TWI_REP    		  0x10
// SLA + W Успешно
#define TWI_ACK_W  	  	0x18
// SLA + W Провал
#define TWI_NACK      	0x20
// Данные переданы успешно
#define TWI_DATA_ACK  	0x28
// Данные переданы без подтверждения
#define TWI_DATA_NACK	  0x30
// Нет данных
#define TWI_EMPTY 		  0xF8
// Ошибка при старте или стопе
#define TWI_ERROR		    0x00
#define TWI_TWSR_MASK		~7

#define BUFFER_LENGTH 32
#define WIRE_HAS_END 1

class TWI_Master {
private:
  static volatile uint8_t *_master_buffer;
  static volatile uint16_t _master_index;
  static volatile uint16_t _master_buffer_length;
  static volatile byte _state;
  static volatile byte _master_address;

  byte _buffer[TWI_BUFFER_LENGTH];
  byte _index = 0;
  byte _address;

public:
  static void send_address();
  static void stop(void);
  static void send_next_byte(void);

public:
  void begin();
  void end();
  void set_freq(uint32_t freq);
  void write(byte *, uint16_t);
  void write(byte);
  void beginTransmission();

public:
  void set_address(byte address) { _address = address << 1; }
  void endTransmission(void);
};
