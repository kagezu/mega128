#pragma once
#include "font.h"

// offset symbol
static const uint8_t offset_arial_14[] PROGMEM = {
0,0,10,0,12,0,18,0,34,0,48,0,68,0,84,0,86,0,92,0,98,0,108,0,122,0,124,0,132,0,134,0,142,0,154,0,160,0,172,0,184,0,198,0,210,0,222,0,234,0,246,0,2,1,4,1,6,1,18,1,30,1,42,1,54,1,80,1,98,1,112,1,128,1,144,1,158,1,172,1,190,1,204,1,206,1,216,1,232,1,246,1,8,2,22,2,40,2,54,2,72,2,88,2,102,2,116,2,130,2,148,2,174,2,190,2,208,2,224,2,228,2,236,2,240,2,250,2,10,3,14,3,26,3,38,3,48,3,60,3,72,3,80,3,92,3,104,3,106,3,110,3,122,3,124,3,142,3,154,3,166,3,178,3,190,3,198,3,208,3,216,3,228,3,242,3,4,4,16,4,30,4,42,4,48,4,50,4,56,4,70,4,84,4
};

// font data
static const uint8_t data_arial_14[] PROGMEM = {
0,0,0,0,0,0,0,0,0,0,
254,5,
30,0,30,0,0,0,
144,144,248,150,144,248,150,144,0,7,0,0,7,0,0,0,
24,36,34,255,66,66,132,2,4,4,15,4,2,1,
28,34,34,28,192,48,140,66,64,128,0,0,4,3,0,0,3,4,4,3,
128,92,34,98,146,12,128,0,3,4,4,4,4,3,2,4,
30,0,
240,12,2,7,24,32,
2,12,240,32,24,7,
32,160,112,160,32,0,0,0,0,0,
64,64,64,248,64,64,64,0,0,0,3,0,0,0,0,28,128,128,128,128,0,0,0,0,0,4,0,192,56,6,6,1,0,0,252,2,2,2,2,252,3,4,4,4,4,3,8,4,254,0,0,7,12,2,2,130,66,60,4,6,5,4,4,4,12,2,34,34,34,220,3,4,4,4,4,3,128,64,48,8,4,254,0,1,1,1,1,1,7,1,56,22,18,18,18,226,3,4,4,4,6,1,248,68,34,34,34,196,3,4,4,4,4,3,2,2,2,226,26,6,0,0,7,0,0,0,220,34,34,34,34,220,3,4,4,4,4,3,60,66,66,66,34,252,2,4,4,4,2,1,8,4,8,28,64,160,160,16,16,8,0,0,0,1,1,2,144,144,144,144,144,144,0,0,0,0,0,0,8,16,16,160,160,64,2,1,1,0,0,0,12,2,130,66,34,28,0,0,5,0,0,0,224,24,4,196,34,18,18,18,162,114,4,8,240,3,12,16,19,36,36,36,34,39,36,20,19,8,0,128,224,156,130,156,224,128,0,6,1,0,0,0,0,0,1,6,254,34,34,34,34,34,220,7,4,4,4,4,4,3,248,4,2,2,2,2,4,8,1,2,4,4,4,4,2,1,254,2,2,2,2,2,4,248,7,4,4,4,4,4,2,1,254,34,34,34,34,34,34,7,4,4,4,4,4,4,254,34,34,34,34,34,2,7,0,0,0,0,0,0,248,4,2,2,2,66,66,68,200,1,2,4,4,4,4,4,2,1,254,32,32,32,32,32,254,7,0,0,0,0,0,7,254,7,0,0,0,0,254,3,4,4,4,3,254,128,64,32,80,136,4,2,7,0,0,0,0,1,2,4,254,0,0,0,0,0,0,7,4,4,4,4,4,4,254,12,48,192,0,192,48,12,254,7,0,0,1,6,1,0,0,7,254,4,24,96,128,0,254,7,0,0,0,1,2,7,248,4,2,2,2,2,2,4,248,1,2,4,4,4,4,4,2,1,254,66,66,66,66,66,60,7,0,0,0,0,0,0,248,4,2,2,2,2,2,4,248,1,2,4,4,4,5,2,7,4,254,66,66,66,194,66,66,60,7,0,0,0,0,1,2,4,28,34,34,34,66,66,140,3,4,4,4,4,4,3,2,2,2,254,2,2,2,0,0,0,7,0,0,0,254,0,0,0,0,0,254,1,2,4,4,4,2,1,6,24,96,128,0,128,96,24,6,0,0,0,1,6,1,0,0,0,6,56,192,0,192,60,2,60,192,0,192,56,6,0,0,1,6,1,0,0,0,1,6,1,0,0,2,12,144,96,96,144,12,2,4,3,0,0,0,1,3,4,2,4,24,32,192,32,24,4,2,0,0,0,0,7,0,0,0,0,0,2,130,66,34,26,6,2,4,6,5,4,4,4,4,4,254,2,63,32,6,56,192,0,0,0,1,6,2,254,32,63,32,28,2,28,32,0,0,0,0,0,0,0,0,0,0,0,0,0,32,32,32,32,32,32,32,32,2,4,0,0,16,136,72,72,72,240,3,4,4,4,2,7,254,16,8,8,8,240,7,2,4,4,4,3,240,8,8,8,16,3,4,4,4,2,240,8,8,8,16,254,3,4,4,4,2,7,240,72,72,72,72,112,3,4,4,4,4,2,8,252,10,10,0,7,0,0,240,8,8,8,16,248,19,36,36,36,34,31,254,16,8,8,8,240,7,0,0,0,0,7,250,7,0,250,32,31,254,128,64,160,16,8,7,0,0,0,3,4,254,7,248,16,8,8,240,16,8,8,240,7,0,0,0,7,0,0,0,7,248,16,8,8,8,240,7,0,0,0,0,7,240,8,8,8,8,240,3,4,4,4,4,3,248,16,8,8,8,240,63,2,4,4,4,3,240,8,8,8,16,248,3,4,4,4,2,63,248,16,8,8,7,0,0,0,48,72,72,72,144,2,4,4,4,3,8,254,8,8,0,7,4,4,248,0,0,0,0,248,3,4,4,4,2,7,24,96,128,0,128,96,24,0,0,1,6,1,0,0,24,224,0,224,24,224,0,224,24,0,1,6,1,0,1,6,1,0,8,48,192,192,48,8,4,3,0,0,3,4,24,96,128,0,128,96,24,0,32,35,28,3,0,0,8,8,136,104,24,8,4,6,5,4,4,4,128,124,2,0,31,32,254,63,2,124,128,32,31,0,64,32,32,96,64,64,32,0,0,0,0,0,0,0,252,4,4,4,4,4,252,7,4,4,4,4,4,7
};

static const Font arial_14 PROGMEM = {
96,                         // count_char
32,                         // first_char
7,                          // weight
14,                         // height
(uint16_t)offset_arial_14,  // offset symbol
(uint16_t)data_arial_14     // font data
};
