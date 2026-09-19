#ifndef PPGFILE_H
#define PPGFILE_H

#include "structs.h"
#include "types.h"

extern s16* dctex_linear;

/* A quad drawn from a data list: its colour and the texture and palette it uses. */
typedef struct {
    u32 col;
    PPGDataList* tb;
    s32 tix;
    s32 cix;
} PPGQuadArgs;

/* The same, with the flip and palette overrides the transparent path takes. */
typedef struct {
    u32 col;
    PPGDataList* tb;
    s32 tix;
    s32 cix;
    s32 flip;
    s32 pal;
} PPGQuadTransArgs;

/* Source and destination for one decompression. */
typedef struct {
    void* srcAdrs;
    s32 srcSize;
    void* dstAdrs;
    s32 dstSize;
} PPGDecompressArgs;

/* A palette chunk read straight from memory. The last field was an unnamed
 * unused parameter and keeps its position. */
typedef struct {
    u8* adrs;
    s32 size;
    s32 ixNum1st;
    s32 num;
    s32 unused;
} PPGPalChunkArgs;

/* A palette chunk read through a PPL header. The last field was an unnamed
 * unused parameter and keeps its position. */
typedef struct {
    PPLFileHeader* ppl;
    u8* adrs;
    s32 ixNum1st;
    s32 unused;
} PPGPalChunkDirArgs;

/* How to byte-swap a block: its size, whether it is long-ended, and its depth. */
typedef struct {
    s32 size;
    s32 dendL;
    s32 col4;
    s32 depth;
} PPGEndianArgs;

/* A run of texture chunks read through a PPG header. */
typedef struct {
    PPGFileHeader* ppg;
    u8* adrs;
    s32 ixNum1st;
    s32 ixNums;
    u32 attribute;
} PPGTexSeqsArgs;

/* One texture handle renewed from a block of dot data. */
typedef struct {
    u32 gix;
    u32* srcRam;
    u32 code;
    u32 size;
} PPGDotDataArgs;

/* The first pass over a texture chunk. */
typedef struct {
    u8* adrs;
    ssize_t size;
    s32 ixNum1st;
    s32 ixNums;
    s32 ar;
    s32 arcnt;
} PPGTexChunk1stArgs;

void ppg_Initialize(void* lcmAdrs, s32 lcmSize);
void ppgSourceDataReleased(PPGDataList* dlist);
void ppgSetupCurrentDataList(PPGDataList* dlist);
void ppgSetupCurrentPaletteNumber(Palette* pal, s32 num);
s32 ppgWriteQuadWithST_B(Vertex* pos, const PPGQuadArgs* a);
s32 ppgWriteQuadWithST_B2(Vertex* pos, const PPGQuadArgs* a);
s32 ppgSetupPalChunk(Palette* pch, const PPGPalChunkArgs* a);
void ppgRenewDotDataSeqs(Texture* tch, const PPGDotDataArgs* a);
void ppgMakeConvTableTexDC();
s32 ppgSetupTexChunk_1st(Texture* tch, const PPGTexChunk1stArgs* a);
s32 ppgSetupTexChunk_1st_Accnum(Texture* tch, u16 accnum);
s32 ppgSetupTexChunk_2nd(Texture* tch, s32 ixNum);
s32 ppgSetupTexChunk_3rd(Texture* tch, s32 ixNum, u32 attribute);
s32 ppgWriteQuadUseTrans(Vertex* pos, const PPGQuadTransArgs* a);
s32 ppgGetUsingTextureHandle(Texture* tch, s32 ixNums);
s32 ppgGetUsingPaletteHandle(Palette* pch, s32 ixNums);
s32 ppgCheckTextureNumber(Texture* tex, s32 num);
s32 ppgReleasePaletteHandle(Palette* pch, s32 ixNum);
s32 ppgReleaseTextureHandle(Texture* tch, s32 ixNum);
s32 ppgSetupTexChunkSeqs(Texture* tch, const PPGTexSeqsArgs* a);
s32 ppgRenewTexChunkSeqs(Texture* tch);
s32 ppgSetupCmpChunk(u8* srcAdrs, s32 num, u8* dstAdrs);
s32 ppgSetupPalChunkDir(Palette* pch, const PPGPalChunkDirArgs* a);
s32 ppgCheckTextureDataBe(Texture* tch);

#endif // PPGFILE_H
