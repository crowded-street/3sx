#ifndef FLPS2DEBUG_H
#define FLPS2DEBUG_H

#include "structs.h"
#include "types.h"

void flPS2SystemError(s32 error_level, s8* format, ...);
s32 flPrintL(s32 posi_x, s32 posi_y, const s8* format, ...);
s32 flPrintColor(u32 col);

#endif // FLPS2DEBUG_H
