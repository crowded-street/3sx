#ifndef PPGFILE_H
#define PPGFILE_H

#include "structs.h"
#include "types.h"

extern s16* dctex_linear;

void ppg_Initialize(void* lcmAdrs, s32 lcmSize);
void ppgSourceDataReleased(PPGDataList* dlist);
void ppgSetupCurrentDataList(PPGDataList* dlist);
void ppgSetupCurrentPaletteNumber(Palette* pal, s32 num);
s32 ppgWriteQuadWithST_B(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix);
s32 ppgWriteQuadWithST_B2(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix);
s32 ppgSetupPalChunk(Palette* pch, u8* adrs, s32 size, s32 ixNum1st, s32 num, s32 /* unused */);
void ppgRenewDotDataSeqs(Texture* tch, u32 gix, u32* srcRam, u32 code, u32 size);
void ppgMakeConvTableTexDC();
s32 ppgSetupTexChunk_1st(Texture* tch, u8* adrs, ssize_t size, s32 ixNum1st, s32 ixNums, s32 ar, s32 arcnt);
s32 ppgSetupTexChunk_1st_Accnum(Texture* tch, u16 accnum);
s32 ppgSetupTexChunk_2nd(Texture* tch, s32 ixNum);
s32 ppgSetupTexChunk_3rd(Texture* tch, s32 ixNum, u32 attribute);
s32 ppgWriteQuadUseTrans(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix, s32 flip, s32 pal);
s32 ppgGetUsingTextureHandle(Texture* tch, s32 ixNums);
s32 ppgGetUsingPaletteHandle(Palette* pch, s32 ixNums);
s32 ppgCheckTextureNumber(Texture* tex, s32 num);
s32 ppgReleasePaletteHandle(Palette* pch, s32 ixNum);
s32 ppgReleaseTextureHandle(Texture* tch, s32 ixNum);
s32 ppgSetupTexChunkSeqs(Texture* tch, PPGFileHeader* ppg, u8* adrs, s32 ixNum1st, s32 ixNums, u32 attribute);
s32 ppgRenewTexChunkSeqs(Texture* tch);
s32 ppgSetupCmpChunk(u8* srcAdrs, s32 num, u8* dstAdrs);
s32 ppgSetupPalChunkDir(Palette* pch, PPLFileHeader* ppl, u8* adrs, s32 ixNum1st, s32 /* unused */);
s32 ppgCheckTextureDataBe(Texture* tch);

#endif // PPGFILE_H
