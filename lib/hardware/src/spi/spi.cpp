#include "spi.h"

#define SPI_DIV_4   0x00
#define SPI_DIV_16  0x01
#define SPI_DIV_64  0x02
#define SPI_DIV_128 0x03

// Частота в кГц
void SPI_Master::init(uint16_t fq, byte mode)
{
  byte sck, spi2x;
  sck = spi2x = 0;

  if (fq >= F_CPU / 2000) { sck = SPI_DIV_4; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 4000) sck = SPI_DIV_4;
  else if (fq >= F_CPU / 8000) { sck = SPI_DIV_16; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 16000) sck = SPI_DIV_16;
  else if (fq >= F_CPU / 32000) { sck = SPI_DIV_64; spi2x = _BV(SPI2X); }
  else if (fq >= F_CPU / 64000) sck = SPI_DIV_64;
  else sck = SPI_DIV_128;

  SPI_MASTER_PIN;
  SPCR = _BV(SPE) | _BV(MSTR) | mode | sck;
  SPSR = spi2x;
  // SPDR = 0;
}

void SPI_Master::send(byte data)
{
  SPDR = data;
  asm volatile("nop");
  while (!(SPSR & _BV(SPIF)));
}

byte SPI_Master::read(byte data)
{
  SPDR = data;
  asm volatile("nop");
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

uint16_t SPI_Master::transfer(uint16_t data)
{
  dbyte buf;
  buf.word = data;
  SPDR = buf.byte[1];
  asm volatile("nop");
  while (!(SPSR & _BV(SPIF)));
  buf.byte[1] = SPDR;
  SPDR = buf.byte[0];
  asm volatile("nop");
  while (!(SPSR & _BV(SPIF)));
  buf.byte[0] = SPDR;
  return buf.word;
}

SPI_Master spi;
