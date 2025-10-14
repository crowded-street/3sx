#include "sf33rd/AcrSDK/common/plbmp.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/prilay.h"

#define BMP_MAGIC 0x4D42
#define U16_AT(_ptr, _offset) (((u16*)_ptr)[_offset])

s32 plBMPSetContextFromImage(plContext* dst, void* bmp) {
    u8* lp;
    s32 i;

    (void)i;

    lp = (u8*)bmp;
    i = U16_AT(lp, 0);

    if (i != BMP_MAGIC) {
        plReport("This is not BMP image. @plBMPSetContextFromImage");
        return 0;
    }

    lp += 0xA;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 0x10;

    lp += 8;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 0x10;

    dst->width = i;

    lp += 4;
    i = U16_AT(lp, 0);
    i |= U16_AT(lp, 1) << 0x10;

    dst->height = i;

    lp += 6;
    i = U16_AT(lp, 0);

    if (i != 24) {
        plReport("Not support except 24bit. @plCreateContextFromBMPImage");
        return 0;
    }

    dst->bitdepth = 3;
    dst->pitch = dst->width * 3;
    dst->pixelformat.rl = 8;
    dst->pixelformat.rs = 0x10;
    dst->pixelformat.rm = 0xFF;
    dst->pixelformat.gl = 8;
    dst->pixelformat.gs = 8;
    dst->pixelformat.gm = 0xFF;
    dst->pixelformat.bl = 8;
    dst->pixelformat.bs = 0;
    dst->pixelformat.bm = 0xFF;
    dst->pixelformat.al = 0;
    dst->pixelformat.as = 0;
    dst->pixelformat.am = 0;
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
