// pgmspace.h - fake Arduino header for ESP-IDF compatibility
#pragma once

#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned int *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))
