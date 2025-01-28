#pragma once
#include "display/default.h"

#if DISPLAY_NAME == _ST7735_SOFT
#define DISPLAY_MODEL ST7735_SOFT
#include <ST7735/ST7735_SOFT.h>
#endif

#define Display DISPLAY_MODEL
