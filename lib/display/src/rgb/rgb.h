// #include "rgb/rgb/h"
#pragma once

#if RGB_FORMAT == RGB_M
#include "rgbm.h"
#elif RGB_FORMAT == RGB_12
#include "rgb12.h"
#elif RGB_FORMAT == RGB_16
#include "rgb16.h"
#elif RGB_FORMAT == RGB_18
#include "rgb18.h"
#endif
