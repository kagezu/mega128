#pragma once
#include <macros/helper.h>
#ifdef _ST7789_

//===================== Config =============================

#ifdef __AVR_ATmega128__
#endif

#ifdef __AVR_ATmega328P__

#define D_RD(x)   x ( C, 0 )
#define D_WR(x)   x ( C, 1 )
#define D_RS(x)   x ( C, 2 )
#define D_CS(x)   x ( C, 3 )
#define D_RST(x)  x ( C, 4 )

#define D_DATA    D

#endif

///////////////////////////////////////////////////////////////////////////
// Вторичные макросы, зависящие от настроек выше

#define INIT_LCD                                          \
D_RD (OUT); D_WR(OUT); D_RS(OUT); D_CS(OUT); D_RST(OUT);  \
D_RD (SET); D_WR(CLR); D_RS(CLR); D_CS(SET); D_RST(CLR);  \
DDR(D_DATA) = 0xFF;

#define D_WRITE  D_RD(SET); DDR(D_DATA) = 0xFF;
#define D_READ   DDR(D_DATA) = 0x00; D_RD(CLR);

// размер дисплея
#define LCD_MAX_X 239
#define LCD_MAX_Y 319


#endif
