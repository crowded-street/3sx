#include "sf33rd/AcrSDK/common/plpic.h"
#include "common.h"

typedef struct {
    // total size: 0x68
    u32 magic;   // offset 0x0, size 0x4
    f32 ver;     // offset 0x4, size 0x4
    u8 comm[80]; // offset 0x8, size 0x50
    u32 id;      // offset 0x58, size 0x4
    u16 w;       // offset 0x5C, size 0x2
    u16 h;       // offset 0x5E, size 0x2
    f32 ratio;   // offset 0x60, size 0x4
    u16 fields;  // offset 0x64, size 0x2
    u16 pad;     // offset 0x66, size 0x2
} plPICHeader;

#define U32_AT(_ptr, _offset)                                                                                          \
    (((u8*)_ptr)[_offset] << 0x18 | ((u8*)_ptr)[_offset + 1] << 0x10 | ((u8*)_ptr)[_offset + 2] << 0x8 |               \
     (u32)((u8*)_ptr)[_offset + 3])
#define U16_AT(_ptr, _offset) (((u8*)_ptr)[_offset] << 0x8 | ((u8*)_ptr)[_offset + 1])
#define AS_U32(_var) (*((u32*)(&_var)))

s32 plPICSetContextFromImage(plContext* dst, void* lpbas) {
    plPICHeader hed;
    plPICHeader* lphed;
    u8* lpsrc;
    u32 chn[2];

    lphed = (plPICHeader*)lpbas;

    hed.magic = U32_AT(lphed, 0);
    hed.id = U32_AT(lphed, 0x58);
    hed.w = U16_AT(lphed, 0x5C);
    hed.h = U16_AT(lphed, 0x5E);
    hed.fields = U16_AT(lphed, 0x64);
    AS_U32(hed.ver) = U32_AT(lphed, 0x4);
    AS_U32(hed.ratio) = U32_AT(lphed, 0x60);

    if (hed.magic != 0x5380F634) {
        return 0;
    }

    if (hed.id != 0x50494354) {
        return 0;
    }

    if (hed.ratio != 1.0f) {
        return 0;
    }

    if (hed.fields != 3) {
        return 0;
    }

    lpsrc = (u8*)lphed + 0x68;
    chn[0] = U32_AT(lpsrc, 0);
    lpsrc += 4;

    if (chn[0] != 0x802E0) {
        if (chn[0] != 0x10802E0) {
            return 0;
        }

        chn[1] = U32_AT(lpsrc, 0);
        lpsrc += 4;

        if (chn[1] != 0x80210) {
            return 0;
        }
    }

    dst->bitdepth = (chn[0] == 0x802E0) ? 3 : 4;
    dst->width = hed.w;
    dst->height = hed.h;
    dst->pitch = dst->bitdepth * dst->width;
    dst->desc = 0;
    dst->pixelformat.rs = 0x10;
    dst->pixelformat.rl = 8;
    dst->pixelformat.rm = 0xFF;
    dst->pixelformat.gs = 8;
    dst->pixelformat.gl = 8;
    dst->pixelformat.gm = 0xFF;
    dst->pixelformat.bs = 0;
    dst->pixelformat.bl = 8;
    dst->pixelformat.bm = 0xFF;

    if (chn[0] == 0x802E0) {
        dst->pixelformat.as = 0;
        dst->pixelformat.al = 0;
        dst->pixelformat.am = 0;
    } else {
        dst->pixelformat.as = 0x18;
        dst->pixelformat.al = 8;
        dst->pixelformat.am = 0xFF;
    }

    return 1;
}

u8* plPICGetPixelAddressFromImage(void* lpbas) {
    plPICHeader hed;
    plPICHeader* lphed;
    u8* lpsrc;
    u32 chn[2];

    lphed = (plPICHeader*)lpbas;

    hed.magic = U32_AT(lphed, 0);
    hed.id = U32_AT(lphed, 0x58);
    hed.w = U16_AT(lphed, 0x5C);
    hed.h = U16_AT(lphed, 0x5E);
    hed.fields = U16_AT(lphed, 0x64);
    AS_U32(hed.ver) = U32_AT(lphed, 0x4);
    AS_U32(hed.ratio) = U32_AT(lphed, 0x60);

    if (hed.magic != 0x5380F634) {
        return 0;
    }

    if (hed.id != 0x50494354) {
        return 0;
    }

    if (hed.ratio != 1.0f) {
        return 0;
    }

    if (hed.fields != 3) {
        return 0;
    }

    lpsrc = (u8*)lphed + 0x68;
    chn[0] = U32_AT(lpsrc, 0);
    lpsrc += 4;

    if (chn[0] != 0x802E0) {
        if (chn[0] != 0x10802E0) {
            return NULL;
        }

        chn[1] = U32_AT(lpsrc, 0);
        lpsrc += 4;

        if (chn[1] != 0x80210) {
            return NULL;
        }
    }

    return lpsrc;
}
