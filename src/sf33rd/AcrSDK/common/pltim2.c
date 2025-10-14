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

        case 5:
            dst->desc = dst->desc | 4;
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

        case 1:
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

        case 2:
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

        case 3:
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

    case 2:
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

    case 3:
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

s32 CheckTIM2FileHeader(u8* lpTim2FileHead) {
    u8 FormatVersion;
    u8 FormatId;

    if ((lpTim2FileHead[0] != 'T') || (lpTim2FileHead[1] != 'I') || (lpTim2FileHead[2] != 'M') ||
        (lpTim2FileHead[3] != '2')) {
        if ((lpTim2FileHead[0] == 'C') && (lpTim2FileHead[1] == 'L') && (lpTim2FileHead[2] == 'T') &&
            (lpTim2FileHead[3] == '2')) {
            return 0;
        }
        return 0;
    }

    FormatVersion = lpTim2FileHead[4];
    FormatId = lpTim2FileHead[5];

    if ((FormatVersion != 3) && ((FormatVersion != 4) || ((FormatId != 0) && (FormatId != 1)))) {
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

u8* GetTim2PictureData(u8* lpFile, s32 /* unused */, s32 Mipmap) {
    s32 header_size;
    s32 mipmap_header_size;
    s32 lp0;
    s32 image_size;
    u8* lpData;
    u8* lpImage;
    u8* lpTim2FileHead;
    u8* lpTim2PictureHead;
    u8* lpTim2MipmapSubHead[7];
    u32 pixel_size;

    lpTim2FileHead = lpFile;
    lpData = GetTim2PictureHead(lpFile, 0);
    lpTim2PictureHead = lpData;
    header_size = 0x30;
    lpData += header_size;

    if (lpTim2PictureHead[0x11] > 1) {
        for (lp0 = 0; lp0 < lpTim2PictureHead[0x11] - 1; lp0++) {
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

    if (lpTim2PictureHead[0x11] > 1) {
        image_size = 0;

        for (lp0 = 0; lp0 < Mipmap; lp0++) {
            pixel_size = ((u32*)(lpTim2MipmapSubHead[lp0]))[0];
            pixel_size = InputTim2AlignRegulation(lpTim2FileHead, pixel_size);
            image_size += pixel_size;
        }

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
