#include <macros/helper.h>

#ifdef __AVR_ATmega328P__

#define SPI_SS    _BV(PB2)
#define SPI_MOSI  _BV(PB3)
#define SPI_MISO  _BV(PB4)
#define SPI_SCK   _BV(PB5)

#define SPI_PORT  B
#endif

#ifdef __AVR_ATmega128__

#define SPI_SS    _BV(PB0)
#define SPI_SCK   _BV(PB1)
#define SPI_MOSI  _BV(PB2)
#define SPI_MISO  _BV(PB3)

#define SPI_PORT  B
#endif

#define SPI_MASTER_PIN                                    \
SET_BITS(DDR(SPI_PORT), SPI_SS | SPI_MOSI | SPI_SCK);     \
CLR_BITS(DDR(SPI_PORT), SPI_MISO);

#define SPI_SLAVE_PIN                                     \
SET_BITS(DDR(SPI_PORT), SPI_MISO );                       \
CLR_BITS(DDR(SPI_PORT), SPI_SS | SPI_MOSI | SPI_SCK );

#define SPI_STOP                                          \
CLR_BITS(DDR(SPI_PORT), SPI_MOSI | SPI_MISO | SPI_SCK);   \
SPCR = 0;
