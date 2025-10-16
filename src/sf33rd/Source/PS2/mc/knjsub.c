#include "sf33rd/Source/PS2/mc/knjsub.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <libgraph.h>

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "port/sdl/sdl_message_renderer.h"

typedef struct _rgba {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} _rgba;

typedef struct _kanji_w {
    u32 type;
    u32 sort;
    u32 fontw;
    u32 fonth;
    u32 grada;
    u32 bound;
    u32 bsize;
    u32 fmax;
    u32 fone;
    u32 pmax;
    uintptr_t font_adrs;
    uintptr_t img_adrs[2];
    _rgba* rgba_adrs;
    u32* pack_top[2];
    u32* pack_fnt[2];
    u32 pack_idx;
    u32* pack_cur;
    u32 pack_size;
    u32 fdbp;
    u32 pdbp;
    u32 mem_size;
    u32 dmax;
    u32 dcur;
    u32 dlast;
    u32 dispw;
    u32 disph;
    s32 x;
    s32 y;
    u32 z;
    u32 palet;
    u32 color;
    u32 bg_mode;
    u32 bg_color;
    u32 uni_half;
    u32 uni_table;
    u32 uni_ascii;
} _kanji_w;

typedef struct _kanji_tbl {
    u32 sort;
    u32 fontw;
    u32 fonth;
    u32 grada;
    u32 bound;
    u32 bsize;
    u8* pal_tbl;
    u32 pal_max;
    u32 font_max;
    u32 one_size;
    u32 file_size;
    u32 uni_half;
    u32 uni_table;
    u32 uni_ascii;
} _kanji_tbl;

// forward decls
static u32 ascii2sjis(u8 data, u32 sort);
static u32 ascii2sjis_sce(u8 data);
static u32 ascii2sjis_nec(u8 data);
static u32 sjis2index(u32 code, u32 sort);
static void unicode_puts(_kanji_w* kw, const s8* str);
static u32* make_fbg_pkt(_kanji_w* kw, u32* p, u32* /* unused */, u32 han_f);
static u32* make_fnt_pkt(_kanji_w* kw, u32* p, u32* img, u32 han_f);
static u32* make_env_pkt(u32* p, u32 /* unused */, u32 /* unused */);
static u32* make_pal_pkt(_kanji_w* kw, u32* p);
static u32* get_img_adrs(_kanji_w* kw, u32 index);
static u32* get_uni_adrs(_kanji_w* kw, u32 index);

// sdata

static u8 rgba_tbl16[64] = { 0x0,  0x0,  0x19, 0x0,  0x33, 0x0,  0x4C, 0x0,  0x66, 0x0,  0x33, 0x33, 0x4C,
                             0x33, 0x66, 0x33, 0x80, 0x33, 0x4C, 0x66, 0x66, 0x66, 0x80, 0x66, 0x66, 0x99,
                             0x80, 0x99, 0x80, 0xCC, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80,
                             0xFF, 0x80, 0xFF, 0x33, 0xCC, 0x4C, 0xCC, 0x66, 0xCC, 0x80, 0xCC, 0x4C, 0x99,
                             0x66, 0x99, 0x80, 0x99, 0x66, 0x66, 0x80, 0x66, 0x80, 0x33, 0x80, 0x0 };

static u8 rgba_tbl4[64] = { 0x0,  0xFF, 0x2A, 0xFF, 0x55, 0xFF, 0x80, 0xFF, 0x0,  0x0,  0x0,  0x33, 0x0,
                            0x33, 0x0,  0x33, 0x0,  0x33, 0x0,  0x66, 0x0,  0x66, 0x0,  0x66, 0x0,  0x99,
                            0x0,  0x99, 0x0,  0xCC, 0x0,  0xFF, 0x80, 0xFF, 0x80, 0xAA, 0x80, 0x55, 0x80,
                            0x0,  0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF,
                            0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF, 0x80, 0xFF };

static u8 kata1[9] = { 0xA1, 0xD4, 0xD5, 0xA0, 0xA4, 0x0, 0x0, 0x0, 0x0 };

static u8 kata2[56] = { 0xF0, 0x9F, 0xA1, 0xA3, 0xA5, 0xA7, 0xE1, 0xE3, 0xE5, 0xC1, 0x0,  0xA0, 0xA2, 0xA4,
                        0xA6, 0xA8, 0xA9, 0xAB, 0xAD, 0xAF, 0xB1, 0xB3, 0xB5, 0xB7, 0xB9, 0xBB, 0xBD, 0xBF,
                        0xC2, 0xC4, 0xC6, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xD0, 0xD3, 0xD6, 0xD9, 0xDC,
                        0xDD, 0xDE, 0xDF, 0xE0, 0xE2, 0xE4, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xED, 0xF1 };

// data

static _kanji_tbl kanji_tbl[14] = { { .sort = 0x0,
                                      .fontw = 0x16,
                                      .fonth = 0x16,
                                      .grada = 0x0,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl16,
                                      .pal_max = 0x2,
                                      .font_max = 0x2284,
                                      .one_size = 0xF2,
                                      .file_size = 0x20A0C8,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x0,
                                      .fontw = 0x16,
                                      .fonth = 0x16,
                                      .grada = 0x0,
                                      .bound = 0x1,
                                      .bsize = 0x0,
                                      .pal_tbl = rgba_tbl16,
                                      .pal_max = 0x2,
                                      .font_max = 0x2284,
                                      .one_size = 0x100,
                                      .file_size = 0x228400,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x1,
                                      .fontw = 0x16,
                                      .fonth = 0x16,
                                      .grada = 0x0,
                                      .bound = 0x1,
                                      .bsize = 0x0,
                                      .pal_tbl = rgba_tbl16,
                                      .pal_max = 0x2,
                                      .font_max = 0x1E80,
                                      .one_size = 0x100,
                                      .file_size = 0x1E8000,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x1,
                                      .fontw = 0x16,
                                      .fonth = 0x16,
                                      .grada = 0x0,
                                      .bound = 0x1,
                                      .bsize = 0x0,
                                      .pal_tbl = rgba_tbl16,
                                      .pal_max = 0x2,
                                      .font_max = 0x1117,
                                      .one_size = 0x100,
                                      .file_size = 0x111700,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x1,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x1,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x1E80,
                                      .one_size = 0x64,
                                      .file_size = 0xBEA00,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x1,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x0,
                                      .bound = 0x1,
                                      .bsize = 0x0,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x1E80,
                                      .one_size = 0x100,
                                      .file_size = 0x1E8000,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x92E,
                                      .one_size = 0x3C,
                                      .file_size = 0x27FC8,
                                      .uni_half = 0x0,
                                      .uni_table = 0x2C,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0xC5,
                                      .one_size = 0x3C,
                                      .file_size = 0x352C,
                                      .uni_half = 0xC0,
                                      .uni_table = 0x3,
                                      .uni_ascii = 0x1 },
                                    { .sort = 0x2,
                                      .fontw = 0x1C,
                                      .fonth = 0x1C,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x1C0,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0xC5,
                                      .one_size = 0x70,
                                      .file_size = 0x5D30,
                                      .uni_half = 0xC0,
                                      .uni_table = 0x3,
                                      .uni_ascii = 0x1 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x9F9,
                                      .one_size = 0x3C,
                                      .file_size = 0x2BB5C,
                                      .uni_half = 0x61,
                                      .uni_table = 0x32,
                                      .uni_ascii = 0x1 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x1B34,
                                      .one_size = 0x3C,
                                      .file_size = 0x70D30,
                                      .uni_half = 0x5F,
                                      .uni_table = 0x56,
                                      .uni_ascii = 0x1 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0x1B34,
                                      .one_size = 0x3C,
                                      .file_size = 0x70D30,
                                      .uni_half = 0x5F,
                                      .uni_table = 0x56,
                                      .uni_ascii = 0x1 },
                                    { .sort = 0x2,
                                      .fontw = 0x14,
                                      .fonth = 0x14,
                                      .grada = 0x2,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl4,
                                      .pal_max = 0x2,
                                      .font_max = 0xA57,
                                      .one_size = 0x3C,
                                      .file_size = 0x2D364,
                                      .uni_half = 0x61,
                                      .uni_table = 0x33,
                                      .uni_ascii = 0x0 },
                                    { .sort = 0x0,
                                      .fontw = 0x14,
                                      .fonth = 0x16,
                                      .grada = 0x0,
                                      .bound = 0x0,
                                      .bsize = 0x100,
                                      .pal_tbl = rgba_tbl16,
                                      .pal_max = 0x2,
                                      .font_max = 0x2284,
                                      .one_size = 0xDC,
                                      .file_size = 0x1DA970,
                                      .uni_half = 0x0,
                                      .uni_table = 0x0,
                                      .uni_ascii = 0x0 } };

// sbss
static s32 knj_use_flag;
static u8 ascii_chr_tbl[1];

// bss
_kanji_w kanji_w;

void KnjInit(u32 type, uintptr_t adrs, u32 disp_max, u32 top_dbp) {
    s32 i;
    s32 j;
    _rgba* rp;
    u8* pt;
    _kanji_tbl* tbl;
    u32 psize;
    u32* pp;
    _kanji_w* kw = &kanji_w;

    _rgba* unused_s8;

    memset(kw, 0, sizeof(_kanji_w));
    kw->type = type;
    tbl = &kanji_tbl[kw->type];
    kw->sort = tbl->sort;
    kw->fontw = tbl->fontw;
    kw->fonth = tbl->fonth;
    kw->grada = tbl->grada;
    kw->bound = tbl->bound;
    kw->bsize = tbl->bsize;
    kw->fmax = tbl->font_max;
    kw->fone = tbl->one_size;
    kw->pmax = tbl->pal_max;
    kw->dmax = disp_max;
    kw->uni_half = tbl->uni_half;
    kw->uni_table = tbl->uni_table;
    kw->uni_ascii = tbl->uni_ascii;
    kw->font_adrs = adrs;
    adrs = (adrs + tbl->file_size + 0x3F) / 64 * 64;

    for (i = 0; i < 2; i++) {
        kw->img_adrs[i] = adrs;
        adrs = (adrs + (kw->bsize * kw->dmax) + 0x3F) / 64 * 64;
    }

    kw->rgba_adrs = (_rgba*)adrs;
    rp = kw->rgba_adrs;
    pt = tbl->pal_tbl;

    for (i = 0; i < kw->pmax; i++) {
        for (j = 0; j < 16; j++, unused_s8 = rp, rp = unused_s8 + 1) {
            rp->a = *pt++;
            rp->r = rp->g = rp->b = *pt++;
        }
    }

    adrs += kw->pmax * 64;
    psize = ((kw->pmax * 0x90) + 0x8F + (kw->dmax * 0x120)) / 64 * 64;
    kw->pack_size = psize;
    kw->fdbp = top_dbp;
    kw->pdbp = top_dbp + 2;

    for (i = 0; i < 2; i++) {
        pp = (u32*)adrs;
        kw->pack_top[i] = pp;
        adrs += psize;
        pp = make_env_pkt(pp, kw->fontw, kw->fonth);
        pp = make_pal_pkt(kw, pp);
        kw->pack_fnt[i] = pp;
    }

    kw->pack_idx = 0;
    kw->pack_cur = kw->pack_fnt[0];
    kw->mem_size = adrs - kw->font_adrs;
    printf("KnjInit: adrs=0x%" PRIXPTR " size=0x%X\n", kw->font_adrs, kw->mem_size);
    kw->dispw = kw->fontw;
    kw->disph = kw->fonth;
    kw->x = 0;
    kw->y = 0;
    kw->z = 0xFFFFFF;
    kw->palet = 0;
    kw->color = 0x80808080;
    kw->bg_mode = 1;
    kw->bg_color = 0x80000000;
    knj_use_flag = 1;
}

void KnjFinish() {
    knj_use_flag = 0;
}

s32 KnjUseCheck() {
    return knj_use_flag;
}

void KnjSetSize(s32 dispw, s32 disph) {
    if (KnjUseCheck() != 0) {
        kanji_w.dispw = dispw;
        kanji_w.disph = disph;
    }
}

void KnjLocate(s32 x, s32 y) {
    if (KnjUseCheck() != 0) {
        kanji_w.x = x;
        kanji_w.y = y;
    }
}

void KnjSetColor(u32 color) {
    _kanji_w* kw = &kanji_w;

    if (KnjUseCheck() != 0) {
        kw->color = color;
        kw->bg_color = (kw->bg_color & 0xFFFFFF) | (color & 0xFF000000);
    }
}

void KnjSetAlpha(u32 alpha) {
    _kanji_w* kw = &kanji_w;

    if (KnjUseCheck() == 0) {
        return;
    }

    kw->color = (kw->color & 0xFFFFFF) | (alpha << 0x18);
    kw->bg_color = (kw->bg_color & 0xFFFFFF) | (alpha << 0x18);
}

void KnjSetRgb(u32 color) {
    _kanji_w* kw = &kanji_w;

    if (KnjUseCheck() != 0) {
        kw->color = (kw->color & 0xFF000000) | (color & 0xFFFFFF);
    }
}

void KnjPuts(const s8* str) {
    s32 x;
    u32 c;
    u32 code;
    u32 index;
    u32* img;
    u32* pp;
    u32 han_f;
    _kanji_w* kw = &kanji_w;

    if (KnjUseCheck() == 0) {
        return;
    }

    if (kw->sort == 2) {
        unicode_puts(kw, str);
        return;
    }

    x = kw->x;
    pp = kw->pack_cur;

    while (1) {
#if defined(TARGET_PS2)
        code = *((u8*)str)++;
#else
        code = *(u8*)str;
        str++;
#endif

        if (code == 0) {
            break;
        }

        // Handle \n
        if (code == 0xA) {
            kw->x = x;
            kw->y += kw->disph;
            continue;
        }

        if (((code >= 0x80) && (code <= 0x9F)) || ((code >= 0xE0) && (code < 0x100))) {
#if defined(TARGET_PS2)
            c = *((u8*)str)++;
#else
            c = *(u8*)str;
            str++;
#endif

            if (c == 0) {
                break;
            }

            code = (code << 8) | c;
            han_f = 0;
        } else {
            code = ascii2sjis(code, kw->sort);
            han_f = 1;
        }

        index = sjis2index(code, kw->sort);

        if (index < kw->fmax) {
            if (kw->dcur < kw->dmax) {
                img = get_img_adrs(kw, index);
                pp = make_fnt_pkt(kw, pp, img, han_f);
                kw->dcur += 1;
            } else {
                break;
            }
        }

        kw->x += kw->dispw >> han_f;
    }

    kw->pack_cur = pp;
}

static u32* get_img_adrs(_kanji_w* kw, u32 index) {
    s32 i;
    s32 n;
    u8* src;
    u8* p;
    u16* dst;
    u16 d;

    if (kw->sort == 2) {
        return get_uni_adrs(kw, index);
    }

    src = (u8*)(kw->font_adrs + (index * kw->fone));

    if ((kw->grada == 0) && (kw->bound == 1)) {
        return (u32*)src;
    }

    dst = (u16*)(kw->img_adrs[kw->pack_idx] + (kw->bsize * kw->dcur));

    if (kw->grada == 0) {
        memcpy(dst, src, kw->fone);
        return (u32*)dst;
    }

    p = (u8*)dst;
    n = (kw->fontw + 3) / 4 * 4 * kw->fonth / 4;

    for (i = 0; i < n; i++) {
        d = *src++;
        *dst++ = ((d & 0xC0) >> 6) | (d & 0x30) | ((d & 0xC) << 6) | ((d & 3) << 0xC);
    }

    return (u32*)p;
}

void KnjPrintf(const s8* fmt, ...) {
    s8* p;
    s8 s[128];
    va_list args;

    if (!KnjUseCheck()) {
        return;
    }

    va_start(args, fmt);
    p = s;
    p += vsprintf(s, fmt, args);
    *p = 0;
    KnjPuts(s);
}

void KnjFlush() {
    u32* pp;
    u32 psize;
    _kanji_w* kw = &kanji_w;

    if (KnjUseCheck() != 0) {
        pp = kw->pack_cur;
        *pp++ = 0xF0000000;
        *pp++ = 0;
        *pp++ = 0;
        *pp++ = 0;
        psize = (uintptr_t)pp - (uintptr_t)kw->pack_top[kw->pack_idx];

        if (kw->pack_size < psize) {
            printf("KnjFlush: packet over, 0x%X > 0x%X.\n", psize, kw->pack_size);

            while (1) {
                // Do Nothing
            }
        }

        kw->pack_idx ^= 1;
        kw->pack_cur = kw->pack_fnt[kw->pack_idx];
        kw->dlast = kw->dcur;
        kw->dcur = 0;
    }
}

static u32 sjis2jis_sce(u32 code) {
    u32 ih = (code >> 8) & 0xFF;
    u32 il = code & 0xFF;

    if (((s64)ih >= 0x81) && ((s64)ih < 0xA0)) {
        ih -= 0x81;
    } else if (((s64)ih >= 0xE0) && ((s64)ih < 0xF0)) {
        ih = (s64)ih - 0xC1;
    }

    ih *= 2;

    if (((s64)il >= 0x40) && ((s64)il < 0x7F)) {
        il -= 0x40;
    } else if (((s64)il >= 0x80) && ((s64)il < 0x9F)) {
        il = (s64)il - 0x41;
    } else if (((s64)il >= 0x9F) && ((s64)il < 0xFD)) {
        il -= 0x9F;
        ih += 1;
    }

    return (((s64)ih + 1) << 8) + (s64)il + 0x2021;
}

static u32 sjis2jis_nec(u32 code) {
    u16 hib;
    u16 lob;

    u32 var_s2;

    hib = (code >> 8) & 0xFF;
    lob = code & 0xFF;

    if (hib < 0xA0) {
        var_s2 = 0x71;
    } else {
        var_s2 = 0xB1;
    }

    hib -= var_s2;
    hib = hib * 2 + 1;

    if (lob > 0x7F) {
        lob -= 1;
    }

    if (lob >= 0x9E) {
        lob -= 0x7D;
        hib += 1;
    } else {
        lob -= 0x1F;
    }

    return (hib << 8) | lob;
}

static u32 sjis2index(u32 code, u32 sort) {
    s32 hib;
    s32 lob;

    if (sort == 0) {
        code = sjis2jis_sce(code);
        hib = ((code >> 8) - 0x21) * 0x5E;
        lob = (code & 0xFF) - 0x21;
        code = hib + lob;
    } else {
        code = sjis2jis_nec(code);
        hib = (code >> 8) & 0xFF;
        lob = code & 0xFF;
        code = (lob - 0x21) + (hib - 0x21) * 0x5E;
    }

    return code;
}

static u32 ascii2sjis(u8 data, u32 sort) {
    if (sort == 0) {
        return ascii2sjis_sce(data);
    } else {
        return ascii2sjis_nec(data);
    }
}

static u32 ascii2sjis_sce(u8 data) {
    // Maps control characters and space to full-width space
    if (data < 0x21) {
        return 0x8140;
    }

    // Maps printable ASCII chars
    if (data < 0x7F) {
        return data + 0x7FFF + 0x6D7F;
    }

    if (data < 0xA1) {
        return 0x8140;
    }

    if (data < 0xA6) {
        return kata1[data - 0xA1] + 0x7FFF + 0x6E01;
    }

    if (data == 0xB0) {
        return 0xEEBA;
    }

    if (data == 0xDE) {
        return 0xEEAA;
    }

    if (data == 0xDF) {
        return 0xEEA9;
    }

    return kata2[data - 0xA6] + 0x7FFF + 0x6F01;
}

static u32 ascii2sjis_nec(u8 data) {
    if (data < 0x60) {
        return data + 0x7FFF + 0x520;
    }

    if (data < 0x80) {
        return data + 0x7FFF + 0x521;
    }

    if (data == 0xA5) {
        return 0x85A3;
    }

    return data + 0x7FFF + 0x4FF;
}

static s32 utf82unicode(const u8** str) {
    s32 code;
    s32 tmpa;
    s32 tmpb;

    code = **str;
    *str += 1;

    if (code > 0xBF) {
        if ((code >= 0xC0) && (code < 0xE0)) {
            tmpa = **str;

            if (tmpa == 0) {
                return -1;
            }

            *str += 1;

            if (tmpa < 0x80 || tmpa > 0xBF) {
                return -1;
            }

            code = ((code & 0x1F) << 6) | (tmpa & 0x3F);
        } else if ((code >= 0xE0) && (code < 0xF0)) {
            tmpa = **str;

            if (tmpa == 0) {
                return -1;
            }

            *str += 1;

            if (tmpa < 0x80 || tmpa > 0xBF) {
                return -1;
            }

            tmpb = **str;

            if (tmpb == 0) {
                return -1;
            }

            *str += 1;

            if (tmpb < 0x80 || tmpb > 0xBF) {
                return -1;
            }

            code = (tmpb & 0x3F) | (((code & 0xF) << 12) | ((tmpa & 0x3F) << 6));
        } else {
            return -1;
        }
    }

    return code;
}

static u32 unicode2index(_kanji_w* kw, u32 code) {
    u8* tbl1;
    u8 idx1;
    u16* tbl2;
    u16 idx2;

    tbl1 = (u8*)kw->font_adrs;
    idx1 = *(tbl1 + ((code >> 8) & 0xFF));

    if (idx1 != 0) {
        tbl2 = (u16*)(tbl1 + 0x100);
        idx2 = *(tbl2 + ((idx1 - 1) << 8) + (code & 0xFF));

        if (idx2 != 0) {
            return idx2 - 1;
        }
    }

    if (kw->uni_ascii == 0) {
        return 0;
    }

    return unicode2index(kw, 0x3F);
}

static u32* get_uni_adrs(_kanji_w* kw, u32 index) {
    s32 i;
    s32 j;
    s32 n1;
    s32 n2;
    u8* src;
    u8* dst;
    u8* p;
    u32 d0;
    u32 d1;
    u32 d2;

    src = (u8*)(kw->font_adrs + 0x100 + (kw->uni_table << 9) + (index * kw->fone));
    dst = (u8*)(kw->img_adrs[kw->pack_idx] + (kw->bsize * kw->dcur));
    p = dst;
    n1 = kw->fontw / 8;
    n2 = kw->fontw % 8;

    for (i = 0; i < kw->fonth; i++) {
        for (j = 0; j < n1; j++) {
            d0 = *src++;

            d1 = (d0 & 0x80) >> 7;
            d1 |= d1 << 1;
            d2 = (d0 & 0x40) >> 2;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;

            d1 = (d0 & 0x20) >> 5;
            d1 |= d1 << 1;
            d2 = d0 & 0x10;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;

            d1 = (d0 & 8) >> 3;
            d1 |= d1 << 1;
            d2 = (d0 & 4) << 2;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;

            d1 = (d0 & 2) >> 1;
            d1 |= d1 << 1;
            d2 = (d0 & 1) << 4;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;
        }

        if (n2) {
            d0 = *src++;

            d1 = (d0 & 0x80) >> 7;
            d1 |= d1 << 1;
            d2 = (d0 & 0x40) >> 2;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;

            d1 = (d0 & 0x20) >> 5;
            d1 |= d1 << 1;
            d2 = d0 & 0x10;
            d2 |= d2 << 1;
            *dst++ = d1 | d2;
        }
    }

    return (u32*)p;
}

static u32* get_uni_adrs2(_kanji_w* kw, u32 code) {
    s32 i;
    s32 n;
    u8* src;
    u8* p;
    u16* dst;
    u16 d;

    n = kw->fonth * (((kw->fontw + 3) / 4) * 4) / 4;
    src = &ascii_chr_tbl[code * n];
    dst = (u16*)(kw->img_adrs[kw->pack_idx] + (kw->bsize * kw->dcur));
    p = (u8*)dst;

    for (i = 0; i < n; i++) {
        d = *src++;
        *dst++ = ((d & 0xC0) >> 6) | (d & 0x30) | ((d & 0xC) << 6) | ((d & 3) << 12);
    }

    return (u32*)p;
}

static s32 is_unicode_han(_kanji_w* kw, u32 index) {
    return index < kw->uni_half ? 1 : 0;
}

static void unicode_puts(_kanji_w* kw, const s8* str) {
    const u8* str_buf;
    u32 code;
    u32 index;
    u32* img;
    u32* pp;
    u32 han_f;

    str_buf = (const u8*)str;
    pp = kw->pack_cur;

    while (1) {
        code = utf82unicode(&str_buf);

        if (code == 0) {
            break;
        }

        if (code == 0x20) {
            han_f = 1;
        } else {
            if (kw->uni_ascii == 0) {
                if (code < 0x10) {
                    han_f = 0;
                    img = get_uni_adrs2(kw, code);
                    goto block_14;
                }

                if (code < 0x80) {
                    han_f = 1;
                    img = get_uni_adrs2(kw, code);
                    goto block_14;
                }
            }

            index = unicode2index(kw, code);
            han_f = is_unicode_han(kw, index);

            if (index < kw->fmax) {
                if (kw->dcur < kw->dmax) {
                    img = get_uni_adrs(kw, index);
                block_14:
                    pp = make_fnt_pkt(kw, pp, img, han_f);
                    kw->dcur += 1;
                } else {
                    break;
                }
            }
        }

        kw->x += kw->dispw >> han_f;
    }

    kw->pack_cur = pp;
}

s32 KnjStrLen(const s8* str) {
    u32 index;
    s32 ret;
    s32 code;
    const u8* str_buf;
    _kanji_w* kw = &kanji_w;

    if (kw->sort != 2) {
        return strlen(str);
    }

    ret = 0;
    str_buf = (const u8*)str;

    while (1) {
        code = utf82unicode(&str_buf);

        if (code == 0) {
            break;
        }

        if (kw->uni_ascii == 0) {
            if (code < 0x10) {
                ret += 2;
                continue;
            } else if (code < 0x80) {
                ret += 1;
                continue;
            }
        }

        index = unicode2index(kw, code);
        ret += (is_unicode_han(kw, index) == 0) ? 2 : 1;
    }

    return ret;
}

s32 KnjCheckCode(const s8* str) {
    s32 ret;
    u32 code;
    u32 index;
    const u8* str_buf;
    _kanji_w* kw = &kanji_w;

    str_buf = (const u8*)str;
    code = *str_buf;

    if (code == 0) {
        return 0;
    }

    if (kw->sort != 2) {
        if (((code >= 0x80) && (code <= 0xBF)) || ((code >= 0xE0) && (code < 0x100))) {
            return 0x12;
        }

        return 1;
    }

    if (code < 0xC0) {
        ret = 1;
    } else if ((code >= 0xC0) && (code < 0xE0)) {
        ret = 2;
    } else if ((code >= 0xE0) && (code < 0xF0)) {
        ret = 3;
    } else {
        return 1;
    }

    code = utf82unicode(&str_buf);

    if (kw->uni_ascii == 0) {
        if (code < 0x10) {
            ret |= 0x10;
            return ret;
        }

        if (code < 0x80) {
            return ret;
        }
    }

    index = unicode2index(kw, code);

    if (is_unicode_han(kw, index) == 0) {
        ret |= 0x10;
    }

    return ret;
}

static u32* make_env_pkt(u32* p, u32 /* unused */, u32 /* unused */) {

    return p;
}

static u32* make_img_pkt(u32* p, u32* img, u32 dbp, u32 dbw, u32 dbsm, u32 dsax, u32 dsay, u32 rrw, u32 rrh) {
    SDLMessageRenderer_CreateTexture(rrw, rrh, img, dbsm);

    return p;
}

static u32* make_pal_pkt(_kanji_w* kw, u32* p) {
    s32 i;
    u32* img;

    for (i = 0; i < kw->pmax; i++) {
        img = (u32*)(kw->rgba_adrs + (i * 16));
        p = make_img_pkt(p, img, kw->pdbp + i, 1, SCE_GS_PSMCT32, 0, 0, 8, 2);
    }

    return p;
}

static u32* make_fnt_pkt(_kanji_w* kw, u32* p, u32* img, u32 han_f) {
    s32 x;
    s32 y;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    u32 ox;
    u32 oy;
    u32 u1;
    u32 v1;
    u32 xs;
    u32 ys;

    p = make_img_pkt(p, img, kw->fdbp, 1, SCE_GS_PSMT4, 0, 0, kw->fontw, kw->fonth);
    p = make_fbg_pkt(kw, p, img, han_f);
    x = kw->x * 16;
    y = kw->y * 16;
    ox = ((4096 - flPs2State.DispWidth) / 2) * 0x10;
    oy = ((4096 - flPs2State.DispHeight) / 2) * 0x10;
    xs = (kw->dispw * 16) >> han_f;
    ys = kw->disph * 16;

    if (kw->type == 8) {
        ys = (ys * 12) / 10;
    }

    x0 = ox + x;
    y0 = oy + y;
    x1 = x0 + xs;
    y1 = y0 + ys;
    u1 = ((kw->fontw * 16) >> han_f);
    v1 = kw->fonth * 16;

    SDLMessageRenderer_DrawTexture(x0, y0, x1, y1, 0, 0, u1, v1, kw->color);

    return p;
}

static u32* make_fbg_pkt(_kanji_w* kw, u32* p, u32* /* unused */, u32 han_f) {
    s32 x;
    s32 y;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    u32 ox;
    u32 oy;
    u32 u1;
    u32 v1;
    u32 xs;
    u32 ys;

    if (kw->bg_mode == 0) {
        return p;
    }

    x = (kw->x + 2) << 4;
    y = (kw->y + 2) << 4;
    ox = ((4096 - flPs2State.DispWidth) / 2) << 4;
    oy = ((4096 - flPs2State.DispHeight) / 2) << 4;
    xs = (kw->dispw * 16) >> han_f;
    ys = kw->disph * 16;

    if (kw->type == 8) {
        ys = (ys * 12) / 10;
    }

    x0 = ox + x;
    y0 = oy + y;
    x1 = x0 + xs;
    y1 = y0 + ys;
    u1 = ((kw->fontw * 16) >> han_f);
    v1 = kw->fonth * 16;

    SDLMessageRenderer_DrawTexture(x0, y0, x1, y1, 8, 8, u1 + 8, v1 + 8, kw->bg_color);

    return p;
}
