#ifndef FLPS2RENDER_H
#define FLPS2RENDER_H

#include "structs.h"
#include "types.h"

#define MIP_TBP_SIZE 6
#define MIP_DBW_SIZE 6

s32 flSetRenderState(enum _FLSETRENDERSTATE func, u32 value);
f32 flPS2ConvScreenFZ(f32 z);
/* The six GS register words flPS2SetTextureRegister took one at a time, in the
 * order and with the types its parameter list had them. */
typedef struct {
    u64* texA;
    u64* tex1;
    u64* tex0;
    u64* clamp;
    u64* miptbp1;
    u64* miptbp2;
} FLPS2TextureRegs;

s32 flPS2SetTextureRegister(u32 th, const FLPS2TextureRegs* regs, u32 render_ope);

#endif
