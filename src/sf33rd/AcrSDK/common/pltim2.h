#ifndef PLTIM2_H
#define PLTIM2_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "types.h"

s32 plTIM2GetMipmapTextureNum(void* lpbas);
s32 plTIM2SetContextFromImage(plContext* dst, void* lpbas);
s32 plTIM2SetPaletteContextFromImage(plContext* dst, void* lpbas);
u8* plTIM2GetPixelAddressFromImage(void* lpbas, s32 Mipmap);
u8* plTIM2GetPaletteAddressFromImage(void* lpbas);

#endif
