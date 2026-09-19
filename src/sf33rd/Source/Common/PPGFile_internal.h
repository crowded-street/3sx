/*
 * Shared between the PPGFile files.
 *
 * The macros, the two work types and the file-scope objects were all written at
 * the top of PPGFile.c, where nothing outside it could see them. The split
 * needs them in four places, so they move here; `ppg_w`, `dctex_linear` and
 * `pplColorModeWidth` are still *defined* in PPGFile.c and had external linkage
 * already. Nothing is widened.
 */

#ifndef PPGFILE_INTERNAL_H
#define PPGFILE_INTERNAL_H

#include "sf33rd/Source/Common/PPGFile.h"
#include "structs.h"
#include "types.h"

#define MAGIC_TO_INT(str) ((str[0] << 0x18) | (str[1] << 0x10) | (str[2] << 0x8) | (str[3]))

#define CODE_0(val) ((val & 0xF0) << 8) + ((val & 0xF) << 4)
#define CODE_1(val) ((val & 0x38) << 0xA) + ((val & 7) << 5)

typedef struct {
    PPGDataList* cur;
    u16 hanPal;
    u16 hanTex;
    _MEMMAN_OBJ mm;
} PPG_W;

typedef struct {
    Vec3 v;
    TexCoord t;
} _Vertex;

extern const u8 pplColorModeWidth[4];

extern PPG_W ppg_w;
extern s16* dctex_linear;

s32 ppgCheckPaletteDataBe(Palette* pch);
void ppgWriteQuadOnly(Vertex* pos, u32 col, u32 texCode);
void ppgWriteQuadOnly2(Vertex* pos, u32 col, u32 texCode);
void ppgChangeDataEndian(u8* adrs, const PPGEndianArgs* a);
void ppgSetupContextFromPPL(PPLFileHeader* ppl, plContext* bits);
void ppgSetupContextFromPPG(PPGFileHeader* ppg, plContext* bits);

void ppgFree(void* adrs);
void* ppgMallocF(s32 size);
void* ppgPullDecBuff(s32 size);
void ppgPushDecBuff(void* adrs);

/* What the transparent-run path needs from the quad it is drawing for: the PPG
 * header, how many runs it lists, and the handles resolved from the data list. */
typedef struct {
    PPGFileHeader* ppg;
    u16 transTotal;
    u16 ix_ofs;
    u16* phan;
    u16 texhan;
} PPGTransRun;

#endif
