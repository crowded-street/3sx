#include "sf33rd/AcrSDK/common/plbmp.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/prilay.h"

#define BMP_MAGIC 0x4D42
#define U16_AT(_ptr, _offset) (((u16*)_ptr)[_offset])

s32 plBMPSetContextFromImage(plContext* dst, void* bmp) {
    const u8* lp = bmp;
    s32 i = U16_AT(lp, 0);

    if (i != BMP_MAGIC) {
        plReport("This is not BMP image. @plBMPSetContextFromImage");
        return 0;
    }

    lp += 10;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 16;

    lp += 8;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 16;

    dst->width = i;

    lp += 4;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 16;

    dst->height = i;

    lp += 6;
    i = U16_AT(lp, 0);

    if (i != 24 && i != 32) {
        plReport("Only 24-bit and 32-bit BMPs are supported. @plBMPSetContextFromImage");
        return 0;
    }

    dst->bitdepth = i / 8;
    dst->pitch = dst->width * dst->bitdepth;
    dst->pixelformat.rl = 8;
    dst->pixelformat.rs = 0x10;
    dst->pixelformat.rm = 0xFF;
    dst->pixelformat.gl = 8;
    dst->pixelformat.gs = 8;
    dst->pixelformat.gm = 0xFF;
    dst->pixelformat.bl = 8;
    dst->pixelformat.bs = 0;
    dst->pixelformat.bm = 0xFF;
    dst->pixelformat.al = i == 32 ? 8 : 0;
    dst->pixelformat.as = i == 32 ? 0x18 : 0;
    dst->pixelformat.am = i == 32 ? 0xFF : 0;
    dst->desc = 0;

    return 1;
}

u8* plBMPGetPixelAddressFromImage(void* bmp) {
    u8* lpsrc;
    u8* lp;
    s32 i;

    lp = (u8*)bmp;
    i = U16_AT(lp, 0);

    if (i != 0x4D42) {
        plReport("This is not BMP image. @plCreateContextFromBMPImage");
        return NULL;
    }

    lp += 0xA;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 0x10;

    lpsrc = (u8*)bmp + i;

    return lpsrc;
}
