#include "sf33rd/AcrSDK/common/plapx.h"
#include "common.h"

typedef struct {
    u32 FileSize;
    u32 PixelSize;
    u32 ClutSize;
    u16 PixelBit;
    u16 PixelWidth;
    u16 PixelHeight;
    u16 MipmapNum;
    u16 ClutBit;
    u16 ClutNum;
    u32 Reserved[2];
} APXFileHeader;

static APXFileHeader* GetAPXFileHeader(void* lpbas);
static u8* GetAPXPixelMipmapAdrs(void* lpbas, s32 index);
static u8* GetAPXPaletteAdrs(void* lpbas, s32 index);

s32 plAPXGetMipmapTextureNum(void* lpbas) {
    APXFileHeader* lpApxFileHeader = GetAPXFileHeader(lpbas);
    return lpApxFileHeader->MipmapNum;
}

s32 plAPXGetPaletteNum(void* lpbas) {
    APXFileHeader* lpApxFileHeader = GetAPXFileHeader(lpbas);
    return lpApxFileHeader->ClutNum;
}

s32 plAPXSetContextFromImage(plContext* dst, void* lpbas) {
    s32 mipmap_num = plAPXGetMipmapTextureNum(lpbas);
    APXFileHeader* lpApxFileHeader = GetAPXFileHeader(lpbas);
    s32 dw = lpApxFileHeader->PixelWidth;
    s32 dh = lpApxFileHeader->PixelHeight;
    s32 lp0;

    for (lp0 = 0; lp0 < mipmap_num; lp0++) {
        dst->desc = 0;
        dst->width = dw;
        dst->height = dh;

        switch (lpApxFileHeader->PixelBit) {
        case 4:
            dst->desc |= 0x14;
            dst->bitdepth = 0;
            dst->pitch = dst->width >> 1;
            dst->pixelformat.rs = 0;
            dst->pixelformat.rl = 0;
            dst->pixelformat.rm = 0;
            dst->pixelformat.gs = 0;
            dst->pixelformat.gl = 0;
            dst->pixelformat.gm = 0;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bl = 0;
            dst->pixelformat.bm = 0;
            dst->pixelformat.as = 0;
            dst->pixelformat.al = 0;
            dst->pixelformat.am = 0;
            break;

        case 8:
            dst->desc |= 4;
            dst->bitdepth = 1;
            dst->pitch = dst->bitdepth * dst->width;
            dst->pixelformat.rs = 0;
            dst->pixelformat.rl = 0;
            dst->pixelformat.rm = 0;
            dst->pixelformat.gs = 0;
            dst->pixelformat.gl = 0;
            dst->pixelformat.gm = 0;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bl = 0;
            dst->pixelformat.bm = 0;
            dst->pixelformat.as = 0;
            dst->pixelformat.al = 0;
            dst->pixelformat.am = 0;
            break;

        case 16:
            dst->bitdepth = 2;
            dst->pitch = dst->bitdepth * dst->width;
            dst->pixelformat.rl = 5;
            dst->pixelformat.rs = 0xA;
            dst->pixelformat.rm = 0x1F;
            dst->pixelformat.gl = 5;
            dst->pixelformat.gs = 5;
            dst->pixelformat.gm = 0x1F;
            dst->pixelformat.bl = 5;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bm = 0x1F;
            dst->pixelformat.al = 1;
            dst->pixelformat.as = 0xF;
            dst->pixelformat.am = 1;
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0xA;
            break;

        case 24:
            dst->bitdepth = 3;
            dst->pitch = dst->bitdepth * dst->width;
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
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0x10;
            break;

        case 32:
            dst->bitdepth = 4;
            dst->pitch = dst->bitdepth * dst->width;
            dst->pixelformat.rl = 8;
            dst->pixelformat.rs = 0x10;
            dst->pixelformat.rm = 0xFF;
            dst->pixelformat.gl = 8;
            dst->pixelformat.gs = 8;
            dst->pixelformat.gm = 0xFF;
            dst->pixelformat.bl = 8;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bm = 0xFF;
            dst->pixelformat.al = 8;
            dst->pixelformat.as = 0x18;
            dst->pixelformat.am = 0xFF;
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0x10;
            break;
        }

        dst += 1;
        dw >>= 1;
        dh >>= 1;
    }

    return 1;
}

s32 plAPXSetPaletteContextFromImage(plContext* dst, void* lpbas) {
    s32 pal_num;
    APXFileHeader* lpApxFileHeader;
    s32 lp0;

    pal_num = plAPXGetPaletteNum(lpbas);
    lpApxFileHeader = GetAPXFileHeader(lpbas);

    (void)lp0;

    for (lp0 = 0; lp0 < pal_num; lp0++) {
        dst->desc = 0;

        switch (lpApxFileHeader->ClutBit) {
        case 16:
            dst->bitdepth = 2;
            dst->pixelformat.rl = 5;
            dst->pixelformat.rs = 0xA;
            dst->pixelformat.rm = 0x1F;
            dst->pixelformat.gl = 5;
            dst->pixelformat.gs = 5;
            dst->pixelformat.gm = 0x1F;
            dst->pixelformat.bl = 5;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bm = 0x1F;
            dst->pixelformat.al = 1;
            dst->pixelformat.as = 0xF;
            dst->pixelformat.am = 1;
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0xA;
            break;

        case 24:
            dst->bitdepth = 3;
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
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0x10;
            break;

        case 32:
            dst->bitdepth = 4;
            dst->pixelformat.rl = 8;
            dst->pixelformat.rs = 0x10;
            dst->pixelformat.rm = 0xFF;
            dst->pixelformat.gl = 8;
            dst->pixelformat.gs = 8;
            dst->pixelformat.gm = 0xFF;
            dst->pixelformat.bl = 8;
            dst->pixelformat.bs = 0;
            dst->pixelformat.bm = 0xFF;
            dst->pixelformat.al = 8;
            dst->pixelformat.as = 0x18;
            dst->pixelformat.am = 0xFF;
            dst->pixelformat.rs = 0;
            dst->pixelformat.bs = 0x10;
            break;

        default:
            return 0;
        }

        switch (lpApxFileHeader->PixelBit) {
        case 4:
            dst->width = 0x10;
            dst->height = 1;
            break;

        case 8:
            dst->width = 0x100;
            dst->height = 1;
            break;

        default:
            return 0;
        }

        dst->pitch = dst->bitdepth * dst->width;
        dst += 1;
    }

    return 1;
}

u8* plAPXGetPixelAddressFromImage(void* lpbas, s32 Mipmap) {
    return GetAPXPixelMipmapAdrs(lpbas, Mipmap);
}

u8* plAPXGetPaletteAddressFromImage(void* lpbas, s32 index) {
    return GetAPXPaletteAdrs(lpbas, index);
}

static APXFileHeader* GetAPXFileHeader(void* lpbas) {
    return (APXFileHeader*)lpbas;
}

static u8* GetAPXPixelMipmapAdrs(void* lpbas, s32 index) {
    APXFileHeader* lpApxFileHeader;
    s32 mipmap_num;
    s32 lp0;
    s32 dw;
    s32 dh;
    u8* pixel_ptr;

    mipmap_num = plAPXGetMipmapTextureNum(lpbas);

    if (!(mipmap_num > index)) {
        return NULL;
    }

    lpApxFileHeader = GetAPXFileHeader(lpbas);
    pixel_ptr = (u8*)(lpApxFileHeader + 1);
    dw = lpApxFileHeader->PixelWidth;
    dh = lpApxFileHeader->PixelHeight;

    for (lp0 = 0; lp0 < index; lp0++) {
        switch (lpApxFileHeader->PixelBit) {
        case 4:
            pixel_ptr += dw * dh / 2;
            break;

        case 8:
            pixel_ptr += dw * dh;
            break;

        case 16:
            pixel_ptr += dw * dh * 2;
            break;

        case 24:
            pixel_ptr += dw * dh * 3;
            break;

        case 32:
            pixel_ptr += dw * dh * 4;
            break;
        }

        dw >>= 1;
        dh >>= 1;
    }

    return pixel_ptr;
}

static u8* GetAPXPaletteAdrs(void* lpbas, s32 index) {
    APXFileHeader* lpApxFileHeader;
    u8* pixel_ptr;
    s32 lp0;
    s32 pal_form;

    (void)pal_form;

    lpApxFileHeader = GetAPXFileHeader(lpbas);
    pixel_ptr = GetAPXPixelMipmapAdrs(lpbas, 0);
    pixel_ptr += lpApxFileHeader->PixelSize;

    switch (lpApxFileHeader->PixelBit) {
    case 4:
        pal_form = 0x10;
        break;

    case 8:
        pal_form = 0x100;
        break;
    }

    for (lp0 = 0; lp0 < index; lp0++) {
        switch (lpApxFileHeader->ClutBit) {
        case 16:
            pixel_ptr += pal_form * 2;
            break;

        case 24:
            pixel_ptr += pal_form * 3;
            break;

        case 32:
            pixel_ptr += pal_form * 4;
            break;
        }
    }

    return pixel_ptr;
}
