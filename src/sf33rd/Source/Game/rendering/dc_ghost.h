#ifndef DC_GHOST_H
#define DC_GHOST_H

#include "structs.h"
#include "types.h"

void njUnitMatrix(MTX* mtx);
void njGetMatrix(MTX* m);
void njSetMatrix(MTX* md, MTX* ms);
void njScale(MTX* mtx, f32 x, f32 y, f32 z);
void njTranslate(MTX* mtx, f32 x, f32 y, f32 z);
void njSetBackColor(u32 c0, u32 c1, u32 c2);
void njColorBlendingMode(s32 target, s32 mode);
void njCalcPoint(MTX* mtx, Vec3* ps, Vec3* pd);
void njCalcPoints(MTX* mtx, Vec3* ps, Vec3* pd, s32 num);
void njDrawTexture(Polygon* polygon, s32 /* unused */, s32 tex, s32 /* unused */);
void njDrawSprite(Polygon* polygon, s32 /* unused */, s32 tex, s32 /* unused */);
void njdp2d_init();
void njdp2d_draw();
void njdp2d_sort(f32* pos, f32 pri, uintptr_t col, s32 flag);
void njDrawPolygon2D(PAL_CURSOR* p, s32 /* unused */, f32 pri, u32 attr);
void njSetPaletteBankNumG(u32 globalIndex, s32 bank);
void njSetPaletteData(s32 offset, s32 count, void* data);
s32 njReLoadTexturePartNumG(u32 gix, s8* srcAdrs, u32 ofs, u32 size);

#endif // DC_GHOST_H
