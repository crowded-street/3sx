#ifndef PLAPX_H
#define PLAPX_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "types.h"

s32 plAPXGetMipmapTextureNum(void* lpbas);
s32 plAPXGetPaletteNum(void* lpbas);
s32 plAPXSetContextFromImage(plContext* dst, void* lpbas);
s32 plAPXSetPaletteContextFromImage(plContext* dst, void* lpbas);
u8* plAPXGetPixelAddressFromImage(void* lpbas, s32 Mipmap);
u8* plAPXGetPaletteAddressFromImage(void* lpbas, s32 index);

#endif
