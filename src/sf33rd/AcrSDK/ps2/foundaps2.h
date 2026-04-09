#ifndef FOUNDAPS2_H
#define FOUNDAPS2_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "structs.h"
#include "types.h"

#define VRAM_CONTROL_SIZE 1344
#define VRAM_BLOCK_HEADER_SIZE 3
#define FL_PALETTE_MAX 1088
#define FL_TEXTURE_MAX 256

extern u32 flDebugStrCtr;
extern u32 flDebugStrCol;
extern u32 flDebugStrHan;
extern s32 flVramStaticNum;
extern FL_FMS flFMS;
extern u32 flSystemRenderOperation;
extern s32 flHeight;
extern s32 flWidth;
extern FLTexture flPalette[FL_PALETTE_MAX];
extern FLTexture flTexture[FL_TEXTURE_MAX];
extern FLPS2State flPs2State;

s32 flInitialize();
s32 flFlip(u32 flag);
s32 flLogOut(s8* format, ...);

#endif
