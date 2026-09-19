#include "sf33rd/AcrSDK/common/pltim2.h"
#include "common.h"

static s32 CheckTIM2FileHeader(u8* lpTim2FileHead);
static u32 InputTim2AlignRegulation(u8* lpTim2FileHead, u32 size);
static u8* GetTim2PictureHead(u8* lpFile, s32 no);
static u8* GetTim2PictureData(u8* lpFile, s32 /* unused */, s32 Mipmap);
static u8* GetTim2ClutData(u8* lpFile, s32 /* unused */);

s32 plTIM2GetMipmapTextureNum(void* lpbas) {
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;
    u8 mip_num;

    lpTim2FileHead = (u8*)lpbas;

    if (CheckTIM2FileHeader(lpTim2FileHead) == 0) {
        return 0;
    }

    lpTim2PictureHead = GetTim2PictureHead((u8*)lpbas, 0);
    mip_num = lpTim2PictureHead[0x11] - 1;

    return mip_num;
}

/* The pixel-format layouts the two context setters share, each exactly the
 * run of assignments that stood at both of its call sites. */

/* the 16-bit RGBA5551 layout */
static void set_tim2_pixelformat_16bit(plContext* dst) {
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
}

/* the 24-bit RGB888 layout */
static void set_tim2_pixelformat_24bit(plContext* dst) {
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
}

/* the 32-bit RGBA8888 layout */
static void set_tim2_pixelformat_32bit(plContext* dst) {
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
}

/* every channel zeroed, for the two indexed formats */
static void clear_tim2_pixelformat(plContext* dst) {
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
}
/* The three direct-colour layouts, reached from the indexed switch's default.
 * The case labels are the original ones and the switch is on the same
 * expression, so a format byte that used to match here still matches here, and
 * one that matched nothing still does nothing. */
static void set_tim2_direct_format(plContext* dst, u8* lpTim2PictureHead) {
    switch (lpTim2PictureHead[0x13]) {
    case 1:
        dst->bitdepth = 2;
        dst->pitch = dst->bitdepth * dst->width;
        set_tim2_pixelformat_16bit(dst);
        break;

    case 2:
        dst->bitdepth = 3;
        dst->pitch = dst->bitdepth * dst->width;
        set_tim2_pixelformat_24bit(dst);
        break;

    case 3:
        dst->bitdepth = 4;
        dst->pitch = dst->bitdepth * dst->width;
        set_tim2_pixelformat_32bit(dst);
        break;
    }
}

s32 plTIM2SetContextFromImage(plContext* dst, void* lpbas) {
    u8* lpData;
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;
    s32 lp0;
    s32 dw;
    s32 dh;

    lpTim2FileHead = (u8*)lpbas;

    if (CheckTIM2FileHeader(lpTim2FileHead) == 0) {
        return 0;
    }

    lpData = GetTim2PictureHead((u8*)lpbas, 0);
    lpTim2PictureHead = lpData;

    if (lpTim2PictureHead[0x11] > 7) {
        return 0;
    }

    dw = ((u16*)lpTim2PictureHead)[0xA];
    dh = ((u16*)lpTim2PictureHead)[0xB];

    for (lp0 = 0; lp0 < lpTim2PictureHead[0x11]; lp0++) {
        dst->desc = 0;
        dst->width = dw;
        dst->height = dh;

        switch (lpTim2PictureHead[0x13]) {
        case 4:
            dst->desc = dst->desc | 0x14;
            dst->bitdepth = 0;
            dst->pitch = dst->width >> 1;
            clear_tim2_pixelformat(dst);
            break;

        case 5:
            dst->desc = dst->desc | 4;
            dst->bitdepth = 1;
            dst->pitch = dst->bitdepth * dst->width;
            clear_tim2_pixelformat(dst);
            break;

        default:
            set_tim2_direct_format(dst, lpTim2PictureHead);
            break;
        }

        dst += 1;
        dw >>= 1;
        dh >>= 1;
    }

    return 1;
}

/* The palette's own dimensions, from the entry count in the picture header.
 * Returns 0 where the switch returned 0 from its caller, and 1 where it fell
 * through to the line below. */
static s32 set_tim2_palette_dimensions(plContext* dst, u8* lpTim2PictureHead) {
    switch (lpTim2PictureHead[0x13]) {
    case 4:
        if (((u16*)lpTim2PictureHead)[0x7] != 0x10) {
            return 0;
        }

        dst->width = 0x10;
        dst->height = 1;
        break;

    case 5:
        if (((u16*)lpTim2PictureHead)[0x7] != 0x100) {
            return 0;
        }

        dst->width = 0x100;
        dst->height = 1;
        break;

    default:
        return 0;
    }

    return 1;
}

s32 plTIM2SetPaletteContextFromImage(plContext* dst, void* lpbas) {
    u8* lpData;
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;

    lpTim2FileHead = (u8*)lpbas;

    if (CheckTIM2FileHeader(lpTim2FileHead) == 0) {
        return 0;
    }

    lpData = GetTim2PictureHead((u8*)lpbas, 0);
    lpTim2PictureHead = lpData;

    if (lpTim2PictureHead[0x11] > 7) {
        return 0;
    }

    dst->desc = 0;

    switch (lpTim2PictureHead[0x12]) {
    case 1:
        dst->bitdepth = 2;
        set_tim2_pixelformat_16bit(dst);
        break;

    case 2:
        dst->bitdepth = 3;
        set_tim2_pixelformat_24bit(dst);
        break;

    case 3:
        dst->bitdepth = 4;
        set_tim2_pixelformat_32bit(dst);
        break;

    default:
        return 0;
    }

    if (set_tim2_palette_dimensions(dst, lpTim2PictureHead) == 0) {
        return 0;
    }

    dst->pitch = dst->bitdepth * dst->width;
    return 1;
}

u8* plTIM2GetPixelAddressFromImage(void* lpbas, s32 Mipmap) {
    u8* lpData = GetTim2PictureData((u8*)lpbas, 0, Mipmap);
    return lpData;
}

u8* plTIM2GetPaletteAddressFromImage(void* lpbas) {
    u8* lpData = GetTim2ClutData((u8*)lpbas, 0);
    return lpData;
}

/* The three tests CheckTIM2FileHeader makes, each copied character for
 * character from the condition it stood in. */
static s32 is_not_tim2_magic(u8* lpTim2FileHead) {
    return (lpTim2FileHead[0] != 'T') || (lpTim2FileHead[1] != 'I') || (lpTim2FileHead[2] != 'M') ||
           (lpTim2FileHead[3] != '2');
}

static s32 is_clt2_magic(u8* lpTim2FileHead) {
    return (lpTim2FileHead[0] == 'C') && (lpTim2FileHead[1] == 'L') && (lpTim2FileHead[2] == 'T') &&
           (lpTim2FileHead[3] == '2');
}

static s32 is_unsupported_tim2_version(u8 FormatVersion, u8 FormatId) {
    return (FormatVersion != 3) && ((FormatVersion != 4) || ((FormatId != 0) && (FormatId != 1)));
}

s32 CheckTIM2FileHeader(u8* lpTim2FileHead) {
    u8 FormatVersion;
    u8 FormatId;

    if (is_not_tim2_magic(lpTim2FileHead)) {
        if (is_clt2_magic(lpTim2FileHead)) {
            return 0;
        }
        return 0;
    }

    FormatVersion = lpTim2FileHead[4];
    FormatId = lpTim2FileHead[5];

    if (is_unsupported_tim2_version(FormatVersion, FormatId)) {
        return 0;
    }

    if (((u16*)lpTim2FileHead)[3] != 1) {
        return 0;
    }

    return 1;
}

u32 InputTim2AlignRegulation(u8* lpTim2FileHead, u32 size) {
    u32 work;
    u32 al;
    u32 tmp;

    if (lpTim2FileHead[5] == 0) {
        al = 0x10;
    } else {
        al = 0x80;
    }

    if ((size % al) != 0) {
        work = size / al;
        tmp = size - al * work;
        size += al - tmp % al;
    }

    return size;
}

u8* GetTim2PictureHead(u8* lpFile, s32 no) {
    s32 i;
    u8* lpTim2FileHead;
    u8* lpPict;

    lpTim2FileHead = lpFile;

    lpFile += InputTim2AlignRegulation(lpFile, 0x10);
    lpPict = lpFile;

    for (i = 0; i < no; i++) {
        if (i >= ((u16*)lpTim2FileHead)[3]) {
            break;
        }

        lpFile += InputTim2AlignRegulation(lpTim2FileHead, 0x30);
        lpFile += ((u32*)lpPict)[0];
        lpPict = lpFile;
    }

    return lpPict;
}

/* The sub-header of each mipmap level after the first, and the size of the
 * levels before the one being asked for. */
static void read_tim2_mipmap_subheads(u8** lpTim2MipmapSubHead, u8* lpTim2PictureHead, u8* lpData) {
    s32 lp0;

    for (lp0 = 0; lp0 < lpTim2PictureHead[0x11] - 1; lp0++) {
        lpTim2MipmapSubHead[lp0] = (u8*)((uintptr_t)lpData + (lp0 << 2) + 0x10);
    }
}

static s32 tim2_mipmap_offset(u8* lpTim2FileHead, u8** lpTim2MipmapSubHead, s32 Mipmap) {
    s32 image_size;
    s32 lp0;
    u32 pixel_size;

    image_size = 0;

    for (lp0 = 0; lp0 < Mipmap; lp0++) {
        pixel_size = ((u32*)(lpTim2MipmapSubHead[lp0]))[0];
        pixel_size = InputTim2AlignRegulation(lpTim2FileHead, pixel_size);
        image_size += pixel_size;
    }

    return image_size;
}

u8* GetTim2PictureData(u8* lpFile, s32 /* unused */, s32 Mipmap) {
    s32 header_size;
    s32 mipmap_header_size;
    s32 image_size;
    u8* lpData;
    u8* lpImage;
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;
    u8* lpTim2MipmapSubHead[7];

    lpTim2FileHead = lpFile;
    lpData = GetTim2PictureHead(lpFile, 0);
    lpTim2PictureHead = lpData;
    header_size = 0x30;
    lpData += header_size;

    if (lpTim2PictureHead[0x11] > 1) {
        read_tim2_mipmap_subheads(lpTim2MipmapSubHead, lpTim2PictureHead, lpData);

        if (lpTim2PictureHead[0x11] < 5) {
            mipmap_header_size = 0x20;
        } else {
            mipmap_header_size = 0x30;
        }

        lpData += mipmap_header_size;
        header_size += mipmap_header_size;
    }

    lpData = lpTim2PictureHead + InputTim2AlignRegulation(lpTim2FileHead, header_size);

    if (lpTim2PictureHead[0x11] > 1) {
        image_size = tim2_mipmap_offset(lpTim2FileHead, lpTim2MipmapSubHead, Mipmap);

        lpImage = lpData + image_size;
    } else {
        lpImage = lpData;
    }

    return lpImage;
}

u8* GetTim2ClutData(u8* lpFile, s32 /* unused */) {
    s32 header_size;
    s32 lp0;
    s32 mipmap_header_size;
    u8* lpData;
    u8* lpClut;
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;
    u8* lpTim2MipmapSubHead[7];

    lpTim2FileHead = lpFile;
    lpData = GetTim2PictureHead(lpFile, 0);
    lpTim2PictureHead = lpData;
    header_size = 0x30;
    lpData += header_size;

    if (lpTim2PictureHead[0x11] > 1) {
        for (lp0 = 0; lp0 < lpTim2PictureHead[0x11]; lp0++) {
            lpTim2MipmapSubHead[lp0] = (u8*)((uintptr_t)lpData + (lp0 << 2) + 0x10);
        }

        if (lpTim2PictureHead[0x11] < 5) {
            mipmap_header_size = 0x20;
        } else {
            mipmap_header_size = 0x30;
        }

        lpData += mipmap_header_size;
        header_size += mipmap_header_size;
    }

    lpData = lpTim2PictureHead + InputTim2AlignRegulation(lpTim2FileHead, header_size);

    if (((u32*)lpTim2PictureHead)[1]) {
        lpClut = lpData + ((u32*)lpTim2PictureHead)[2];
    } else {
        return NULL;
    }

    return lpClut;
}
