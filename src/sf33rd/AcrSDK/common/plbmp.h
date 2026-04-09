#ifndef PLBMP_H
#define PLBMP_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "types.h"

s32 plBMPSetContextFromImage(plContext* dst, void* bmp);
u8* plBMPGetPixelAddressFromImage(void* bmp);

#endif
