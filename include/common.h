// TODO: Remove this header

#ifndef COMMON_H
#define COMMON_H

#include "types.h"

#define BOOL(_expr) ((_expr) ? 1 : 0)

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

#define LO_16_BITS(val) (val & 0xFFFF)
#define HI_16_BITS(val) ((val & 0xFFFF0000) >> 16)

#endif
