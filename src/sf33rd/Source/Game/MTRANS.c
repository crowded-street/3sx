#include "sf33rd/Source/Game/MTRANS.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/chren3rd.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/PS2/ps2Quad.h"
#include "structs.h"

#define PRIO_BASE_SIZE 128

// sbss
s32 curr_bright;
SpriteChipSet seqs_w;

// bss
f32 PrioBase[PRIO_BASE_SIZE];
f32 PrioBaseOriginal[PRIO_BASE_SIZE];

// rodata
static const u16 flptbl[4] = { 0x0000, 0x8000, 0x4000, 0xC000 };

static const u32 bright_type[4][16] = { { 0x00FFFFFF,
                                          0x00EEEEEE,
                                          0x00DDDDDD,
                                          0x00CCCCCC,
                                          0x00BBBBBB,
                                          0x00AAAAAA,
                                          0x00999999,
                                          0x00888888,
                                          0x00777777,
                                          0x00666666,
                                          0x00555555,
                                          0x00444444,
                                          0x00333333,
                                          0x00222222,
                                          0x00111111,
                                          0x00000000 },
                                        { 0x00FFFFFF,
                                          0x00FFEEEE,
                                          0x00FFDDDD,
                                          0x00FFCCCC,
                                          0x00FFBBBB,
                                          0x00FFAAAA,
                                          0x00FF9999,
                                          0x00FF8888,
                                          0x00FF7777,
                                          0x00FF6666,
                                          0x00FF5555,
                                          0x00FF4444,
                                          0x00FF3333,
                                          0x00FF2222,
                                          0x00FF1111,
                                          0x00FF0000 },
                                        { 0x00FFFFFF,
                                          0x00EEFFEE,
                                          0x00DDFFDD,
                                          0x00CCFFCC,
                                          0x00BBFFBB,
                                          0x00AAFFAA,
                                          0x0099FF99,
                                          0x0088FF88,
                                          0x0077FF77,
                                          0x0066FF66,
                                          0x0055FF55,
                                          0x0044FF44,
                                          0x0033FF33,
                                          0x0022FF22,
                                          0x0011FF11,
                                          0x0000FF00 },
                                        { 0x00FFFFFF,
                                          0x00EEEEFF,
                                          0x00DDDDFF,
                                          0x00CCCCFF,
                                          0x00BBBBFF,
                                          0x00AAAAFF,
                                          0x009999FF,
                                          0x008888FF,
                                          0x007777FF,
                                          0x006666FF,
                                          0x005555FF,
                                          0x004444FF,
                                          0x003333FF,
                                          0x002222FF,
                                          0x001111FF,
                                          0x000000FF } };

// forward decls
static void DebugLine(f32 x, f32 y, f32 w, f32 h);
s32 seqsStoreChip(f32 x, f32 y, s32 w, s32 h, s32 gix, s32 code, s32 attr, s32 alpha, s32 id);
void appRenewTempPriority(s32 z);
static s16 check_patcash_ex_trans(PatternCollection* padr, u32 cg);
static s32 get_free_patcash_index(PatternCollection* padr);
static s32 get_mltbuf16(MultiTexture* mt, u32 code, u32 palt, s32* ret);
static s32 get_mltbuf16_ext(MultiTexture* mt, u32 code, u32 palt);
static s32 get_mltbuf16_ext_2(MultiTexture* mt, u32 code, u32 palt, s32* ret, PatternInstance* cp);
static s32 get_mltbuf32(MultiTexture* mt, u32 code, u32 palt, s32* ret);
static s32 get_mltbuf32_ext(MultiTexture* mt, u32 code, u32 palt);
static s32 get_mltbuf32_ext_2(MultiTexture* mt, u32 code, u32 palt, s32* ret, PatternInstance* cp);
static void lz_ext_p6_fx(u8* srcptr, u8* dstptr, u32 len);
static void lz_ext_p6_cx(u8* srcptr, u16* dstptr, u32 len, u16* palptr);
static u16 x16_mapping_set(PatternMap* map, s32 code);
static u16 x32_mapping_set(PatternMap* map, s32 code);

static void search_trsptr(uintptr_t trstbl, s32 i, s32 n, s32 cods, s32 atrs, s32 codd, s32 atrd) {
    s32 j;
    u16* tmpbas;
    s32 ctemp;
    TileMapEntry* tmpptr;
    TileMapEntry* unused_s4;

    atrd &= 0x3FFF;

    for (j = i; j < n; j++) {
        tmpbas = (u16*)(trstbl + ((u32*)trstbl)[j]);
        ctemp = *tmpbas;
        tmpbas++;
        tmpptr = (TileMapEntry*)tmpbas;

        while (ctemp != 0) {
            if (!(tmpptr->attr & 0x1000) && (tmpptr->code == cods) && ((tmpptr->attr & 0xF) == atrs)) {
                tmpptr->code = codd;
                tmpptr->attr = (tmpptr->attr & 0xC000) | atrd;
            }

            ctemp--;
            unused_s4 = tmpptr;
            tmpptr = unused_s4 + 1;
        }
    }
}

void mlt_obj_disp(MultiTexture* mt, WORK* wk, s32 base_y) {
    u16* trsbas;
    TileMapEntry* trsptr;
    s32 rnum;
    s32 attr;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);
    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd & 0xF;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);

    while (count--) {
        if (attr & 0x8000) {
            x += trsptr->x;
        } else {
            x -= trsptr->x;
        }

        if (attr & 0x4000) {
            y -= trsptr->y;
        } else {
            y += trsptr->y;
        }

        dw = ((trsptr->attr & 0xC00) >> 7) + 8;
        dh = ((trsptr->attr & 0x300) >> 5) + 8;

        if (!(trsptr->attr & 0x2000)) {
            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                 y + (dh * BOOL(attr & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx16,
                                 trsptr->code,
                                 palo + ((trsptr->attr ^ attr) & 0xE00F),
                                 wk->my_clear_level,
                                 mt->id);
        } else {
            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - dw * BOOL(attr & 0x8000),
                                 y + dh * BOOL(attr & 0x4000),
                                 dw,
                                 dh,
                                 mt->mltgidx32,
                                 trsptr->code,
                                 palo + ((trsptr->attr ^ attr) & 0xE00F),
                                 wk->my_clear_level,
                                 mt->id);
        }

        if (rnum == 0) {
            break;
        }

        trsptr += 1;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

void mlt_obj_disp_rgb(MultiTexture* mt, WORK* wk, s32 base_y) {
    u16* trsbas;
    TileMapEntry* trsptr;
    s32 rnum;
    s32 attr;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);
    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);

    while (count--) {
        if (attr & 0x8000) {
            x += trsptr->x;
        } else {
            x -= trsptr->x;
        }

        if (attr & 0x4000) {
            y -= trsptr->y;
        } else {
            y += trsptr->y;
        }

        dw = ((trsptr->attr & 0xC00) >> 7) + 8;
        dh = ((trsptr->attr & 0x300) >> 5) + 8;

        if (!(trsptr->attr & 0x2000)) {
            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                 y + (dh * BOOL(attr & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx16,
                                 trsptr->code,
                                 (trsptr->attr ^ attr) & 0xE000,
                                 wk->my_clear_level,
                                 mt->id);
        } else {
            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                 y + (dh * BOOL(attr & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx32,
                                 trsptr->code,
                                 (trsptr->attr ^ attr) & 0xE000,
                                 wk->my_clear_level,
                                 mt->id);
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

s16 getObjectHeight(u16 cgnum) {
    s32 count;
    TileMapEntry* trsptr;
    s16 maxHeight;
    u16* trsbas;
    s32 i = obj_group_table[cgnum];
    s16 height;

    if (i == 0) {
        return 0;
    }

    if (texgrplds[i].ok == 0) {
        return 0;
    }

    cgnum -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)((s8*)texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[cgnum]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;

    for (maxHeight = height = 0; count--; trsptr++) {
        height = height + trsptr->y;

        if (height > maxHeight) {
            maxHeight = height;
        }
    }

    if (height) {
        // do nothing
    }

    return maxHeight;
}

void mlt_obj_trans_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 attr;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s16 ix;
    PatternCode cc;
    PatternInstance* cp;

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = 0;
    cc.parts.offset = wk->cg_number;
    ix = check_patcash_ex_trans(mt->cpat, cc.code);

    if (ix < 0) {
        {
            s32 size;
            s32 code;
            s32 wh;
            s32 dw;
            s32 dh;

            (void)dw;
            (void)dh;

            ix = get_free_patcash_index(mt->cpat);
            cp = &mt->cpat->patt[ix];
            mt->cpat->adr[mt->cpat->kazu] = cp;
            mt->cpat->kazu += 1;
            cp->curr_disp = 1;
            cp->time = mt->mltcshtime16;
            cp->cg.code = cc.code;
            cp->x16 = 0;
            cp->x32 = 0;
            work_init_zero((s32*)&cp->map, sizeof(PatternMap));
            cc.parts.group = i;

            while (count--) {
                if (attr & 0x8000) {
                    x += trsptr->x;
                } else {
                    x -= trsptr->x;
                }

                if (attr & 0x4000) {
                    y -= trsptr->y;
                } else {
                    y += trsptr->y;
                }

                texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
                dw = (texptr->wh & 0xE0) >> 2;
                dh = (texptr->wh & 0x1C) * 2;
                wh = (texptr->wh & 3) + 1;
                size = (wh * wh) << 6;
                cc.parts.offset = trsptr->code;

                switch (wh) {
                case 1:
                case 2:
                    if (get_mltbuf16_ext_2(mt, cc.code, 0, &code, cp) != 0) {
                        lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                        njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size);
                    }

                    if (Debug_w[0x10]) {
                        DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                         y + (dh * BOOL(attr & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx16,
                                         code,
                                         palo | ((trsptr->attr ^ attr) & 0xC000),
                                         wk->my_clear_level,
                                         mt->id);
                    break;

                case 4:
                    if (get_mltbuf32_ext_2(mt, cc.code, 0, &code, cp) != 0) {
                        lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                        njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size);
                    }

                    if (Debug_w[0x10]) {
                        DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                         y + (dh * BOOL(attr & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx32,
                                         code,
                                         palo | (((trsptr->attr ^ attr) & 0xC000) | 0x2000),
                                         wk->my_clear_level,
                                         mt->id);
                    break;
                }

                if (rnum == 0) {
                    break;
                }

                trsptr++;
            }

            seqs_w.up[mt->id] = 1;
            appRenewTempPriority(wk->position_z);
            return;
        }
    }

    {
        s32 code;
        s32 wh;
        s32 dw;
        s32 dh;

        (void)dw;
        (void)dh;

        cp = mt->cpat->adr[ix];
        cp->curr_disp = 1;
        cp->time = mt->mltcshtime16;

        makeup_tpu_free(mt->mltnum16 / 256, mt->mltnum32 / 64, &cp->map);
        cc.parts.group = i;

        while (count--) {
            if (attr & 0x8000) {
                x += trsptr->x;
            } else {
                x -= trsptr->x;
            }

            if (attr & 0x4000) {
                y -= trsptr->y;
            } else {
                y += trsptr->y;
            }

            texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
            dw = (texptr->wh & 0xE0) >> 2;
            dh = (texptr->wh & 0x1C) * 2;
            wh = (texptr->wh & 3) + 1;
            cc.parts.offset = trsptr->code;

            switch (wh) {
            case 1:
            case 2:
                code = get_mltbuf16_ext(mt, cc.code, 0);

                if (Debug_w[0x10]) {
                    DebugLine(x - (dw & ((s16)attr >> 16)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
                }

                rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                     y + (dh * BOOL(attr & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx16,
                                     code,
                                     palo | ((trsptr->attr ^ attr) & 0xC000),
                                     wk->my_clear_level,
                                     mt->id);
                break;

            case 4:
                code = get_mltbuf32_ext(mt, cc.code, 0);

                if (Debug_w[0x10]) {
                    DebugLine(x - (dw & ((s16)attr >> 16)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
                }

                rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                     y + (dh * BOOL(attr & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx32,
                                     code,
                                     palo | (((trsptr->attr ^ attr) & 0xC000) | 0x2000),
                                     wk->my_clear_level,
                                     mt->id);
                break;
            }

            if (rnum == 0) {
                break;
            }

            trsptr++;
        }

        seqs_w.up[mt->id] = 1;
        appRenewTempPriority(wk->position_z);
    }
}

void mlt_obj_trans(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 attr;
    s32 count;
    s32 palo;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    PatternCode cc;
    s32 size;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);

    if (mt->ext) {
        mlt_obj_trans_ext(mt, wk, base_y);
        return;
    }

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = i;

    while (count--) {
        if (attr & 0x8000) {
            x += trsptr->x;
        } else {
            x -= trsptr->x;
        }

        if (attr & 0x4000) {
            y -= trsptr->y;
        } else {
            y += trsptr->y;
        }

        texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16(mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size);
            }

            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                 y + (dh * BOOL(attr & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx16,
                                 code,
                                 palo | ((trsptr->attr ^ attr) & 0xC000),
                                 wk->my_clear_level,
                                 mt->id);
            break;

        case 4:
            if (get_mltbuf32(mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size);
            }

            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)attr >> 0x10)), y + (dh & ((s16)(attr * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(attr & 0x8000)),
                                 y + (dh * BOOL(attr & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx32,
                                 code,
                                 palo | (((trsptr->attr ^ attr) & 0xC000) | 0x2000),
                                 wk->my_clear_level,
                                 mt->id);
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

void mlt_obj_trans_cp3_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 flip;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s16 ix;
    PatternCode cc;
    PatternInstance* cp;

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = 0;
    cc.parts.offset = wk->cg_number;
    ix = check_patcash_ex_trans(mt->cpat, cc.code);

    if (ix < 0) {
        {
            s32 size;
            s32 code;
            s32 wh;
            s32 dw;
            s32 dh;
            s32 attr;
            s32 palt;

            (void)dw;
            (void)dh;

            ix = get_free_patcash_index(mt->cpat);
            cp = &mt->cpat->patt[ix];
            mt->cpat->adr[mt->cpat->kazu] = cp;
            mt->cpat->kazu += 1;
            cp->curr_disp = 1;
            cp->time = mt->mltcshtime16;
            cp->cg.code = cc.code;
            cp->x16 = 0;
            cp->x32 = 0;
            work_init_zero((s32*)&cp->map, sizeof(PatternMap));
            cc.parts.group = i;

            while (count--) {
                if (flip & 0x8000) {
                    x += trsptr->x;
                } else {
                    x -= trsptr->x;
                }

                if (flip & 0x4000) {
                    y -= trsptr->y;
                } else {
                    y += trsptr->y;
                }

                texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
                dw = (texptr->wh & 0xE0) >> 2;
                dh = (texptr->wh & 0x1C) * 2;
                wh = (texptr->wh & 3) + 1;
                size = (wh * wh) << 6;
                attr = trsptr->attr;
                palt = (attr & 0x1FF) + palo;
                attr = (attr ^ flip) & 0xC000;
                cc.parts.offset = trsptr->code;

                switch (wh) {
                case 1:
                case 2:
                    if (get_mltbuf16_ext_2(mt, cc.code, 0, &code, cp) != 0) {
                        lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                        njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size);
                    }

                    if (Debug_w[0x10]) {
                        DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                         y + (dh * BOOL(flip & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx16,
                                         code,
                                         attr | palt,
                                         wk->my_clear_level,
                                         mt->id);
                    break;

                case 4:
                    if (get_mltbuf32_ext_2(mt, cc.code, 0, &code, cp) != 0) {
                        lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                        njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size);
                    }

                    if (Debug_w[0x10]) {
                        DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                         y + (dh * BOOL(flip & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx32,
                                         code,
                                         (attr | 0x2000) | palt,
                                         wk->my_clear_level,
                                         mt->id);
                    break;
                }

                if (rnum == 0) {
                    break;
                }

                trsptr++;
            }

            seqs_w.up[mt->id] = 1;
            appRenewTempPriority(wk->position_z);
        }

        return;
    }

    {
        s32 code;
        s32 wh;
        s32 dw;
        s32 dh;
        s32 attr;
        s32 palt;

        (void)dw;
        (void)dh;

        cp = mt->cpat->adr[ix];
        cp->curr_disp = 1;
        cp->time = mt->mltcshtime16;
        makeup_tpu_free(mt->mltnum16 / 256, mt->mltnum32 / 64, &cp->map);
        cc.parts.group = i;

        while (count--) {
            if (flip & 0x8000) {
                x += trsptr->x;
            } else {
                x -= trsptr->x;
            }

            if (flip & 0x4000) {
                y -= trsptr->y;
            } else {
                y += trsptr->y;
            }

            texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
            dw = (texptr->wh & 0xE0) >> 2;
            dh = (texptr->wh & 0x1C) * 2;
            wh = (texptr->wh & 3) + 1;
            attr = trsptr->attr;
            palt = (attr & 0x1FF) + palo;
            attr = (attr ^ flip) & 0xC000;
            cc.parts.offset = trsptr->code;

            switch (wh) {
            case 1:
            case 2:
                code = get_mltbuf16_ext(mt, cc.code, 0);

                if (Debug_w[0x10]) {
                    DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
                }

                rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                     y + (dh * BOOL(flip & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx16,
                                     code,
                                     attr | palt,
                                     wk->my_clear_level,
                                     mt->id);
                break;

            case 4:
                code = get_mltbuf32_ext(mt, cc.code, 0);

                if (Debug_w[0x10]) {
                    DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
                }

                rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                     y + (dh * BOOL(flip & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx32,
                                     code,
                                     (attr | 0x2000) | palt,
                                     wk->my_clear_level,
                                     mt->id);
                break;
            }

            if (rnum == 0) {
                break;
            }

            trsptr++;
        }

        seqs_w.up[mt->id] = 1;
        appRenewTempPriority(wk->position_z);
    }
}

void mlt_obj_trans_cp3(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 flip;
    s32 count;
    s32 palo;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    PatternCode cc;
    s32 size;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;
    s32 attr;
    s32 palt;

    ppgSetupCurrentDataList(&mt->texList);

    if (mt->ext) {
        mlt_obj_trans_cp3_ext(mt, wk, base_y);
        return;
    }

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = i;

    while (count--) {
        if (flip & 0x8000) {
            x += trsptr->x;
        } else {
            x -= trsptr->x;
        }

        if (flip & 0x4000) {
            y -= trsptr->y;
        } else {
            y += trsptr->y;
        }

        texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
        dw = (s32)(texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + palo;
        attr = (attr ^ flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16(mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size);
            }

            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                 y + (dh * BOOL(flip & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx16,
                                 code,
                                 attr | palt,
                                 wk->my_clear_level,
                                 mt->id);
            break;

        case 4:
            if (get_mltbuf32(mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size);
            }

            if (Debug_w[0x10]) {
                DebugLine(x - (dw & ((s16)flip >> 0x10)), y + (dh & ((s16)(flip * 2) >> 16)), dw, dh);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                 y + (dh * BOOL(flip & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx32,
                                 code,
                                 attr | 0x2000 | palt,
                                 wk->my_clear_level,
                                 mt->id);
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/sf33rd/Source/Game/MTRANS", mlt_obj_trans_rgb_ext);
#else
void mlt_obj_trans_rgb_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 flip;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s16 ix;
    PatternCode cc;
    PatternInstance* cp;

    (void)textbl;

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = wk->colcd;
    cc.parts.offset = wk->cg_number;
    ix = check_patcash_ex_trans(mt->cpat, cc.code);

    if (ix < 0) {
        {
            s32 size;
            s32 code;
            s32 attr;
            s32 palt;
            s32 wh;
            s32 dw;
            s32 dh;

            ix = get_free_patcash_index(mt->cpat);
            cp = &mt->cpat->patt[ix];
            mt->cpat->adr[mt->cpat->kazu] = cp;
            mt->cpat->kazu += 1;
            cp->curr_disp = 1;
            cp->time = mt->mltcshtime16;
            cp->cg.code = cc.code;
            cp->x16 = 0;
            cp->x32 = 0;
            work_init_zero((s32*)&cp->map, sizeof(PatternMap));
            cc.parts.group = i;

            while (count--) {
                if (flip & 0x8000) {
                    x += trsptr->x;
                } else {
                    x -= trsptr->x;
                }

                if (flip & 0x4000) {
                    y -= trsptr->y;
                } else {
                    y += trsptr->y;
                }

                texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
                dw = (texptr->wh & 0xE0) >> 2;
                dh = (texptr->wh & 0x1C) * 2;
                wh = (texptr->wh & 3) + 1;
                size = (wh * wh) << 6;
                attr = trsptr->attr;
                palt = (attr & 0x1FF) + palo;
                attr = (attr ^ flip) & 0xC000;
                cc.parts.offset = trsptr->code;

                switch (wh) {
                case 1:
                case 2:
                    if (get_mltbuf16_ext_2(mt, cc.code, palt, &code, cp) != 0) {
                        lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                        njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size * 2);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                         y + (dh * BOOL(flip & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx16,
                                         code,
                                         attr,
                                         wk->my_clear_level,
                                         mt->id);
                    break;

                case 4:
                    if (get_mltbuf32_ext_2(mt, cc.code, palt, &code, cp) != 0) {
                        lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                        njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size * 2);
                    }

                    rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                         y + (dh * BOOL(flip & 0x4000)),
                                         dw,
                                         dh,
                                         mt->mltgidx32,
                                         code,
                                         attr | 0x2000,
                                         wk->my_clear_level,
                                         mt->id);
                    break;
                }

                if (rnum == 0) {
                    break;
                }

                trsptr++;
            }

            seqs_w.up[mt->id] = 1;
            appRenewTempPriority(wk->position_z);
        }

        return;
    }

    {
        s32 code;
        s32 attr;
        s32 palt;
        s32 wh;
        s32 dw;
        s32 dh;

        cp = mt->cpat->adr[ix];
        cp->curr_disp = 1;
        cp->time = mt->mltcshtime16;
        makeup_tpu_free(mt->mltnum16 / 256, mt->mltnum32 / 64, &cp->map);
        cc.parts.group = i;

        while (count--) {
            if (flip & 0x8000) {
                x += trsptr->x;
            } else {
                x -= trsptr->x;
            }

            if (flip & 0x4000) {
                y -= trsptr->y;
            } else {
                y += trsptr->y;
            }

            texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
            dw = (texptr->wh & 0xE0) >> 2;
            dh = (texptr->wh & 0x1C) * 2;
            wh = (texptr->wh & 3) + 1;
            attr = trsptr->attr;
            palt = (attr & 0x1FF) + palo;
            attr = (attr ^ flip) & 0xC000;
            cc.parts.offset = trsptr->code;

            switch (wh) {
            case 1:
            case 2:
                code = get_mltbuf16_ext(mt, cc.code, palt);

                rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                     y + (dh * BOOL(flip & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx16,
                                     code,
                                     attr,
                                     wk->my_clear_level,
                                     mt->id);
                break;

            case 4:
                code = get_mltbuf32_ext(mt, cc.code, palt);

                rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                     y + (dh * BOOL(flip & 0x4000)),
                                     dw,
                                     dh,
                                     mt->mltgidx32,
                                     code,
                                     attr | 0x2000,
                                     wk->my_clear_level,
                                     mt->id);
                break;
            }

            if (rnum == 0) {
                break;
            }

            trsptr++;
        }

        seqs_w.up[mt->id] = 1;
        appRenewTempPriority(wk->position_z);
    }
}
#endif

void mlt_obj_trans_rgb(MultiTexture* mt, WORK* wk, s32 base_y) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    s32 rnum;
    s32 flip;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    PatternCode cc;
    s32 size;
    s32 code;
    s32 attr;
    s32 palt;
    s32 wh;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);

    if (mt->ext) {
        mlt_obj_trans_rgb_ext(mt, wk, base_y);
        return;
    }

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
    cc.parts.group = i;

    while (count--) {
        if (flip & 0x8000) {
            x += trsptr->x;
        } else {
            x -= trsptr->x;
        }

        if (flip & 0x4000) {
            y -= trsptr->y;
        } else {
            y += trsptr->y;
        }

        texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + palo;
        attr = (attr ^ flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16(mt, cc.code, palt, &code) != 0) {
                lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, size * 2);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                 y + (dh * BOOL(flip & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx16,
                                 code,
                                 attr,
                                 wk->my_clear_level,
                                 mt->id);
            break;

        case 4:
            if (get_mltbuf32(mt, cc.code, palt, &code) != 0) {
                lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, size * 2);
            }

            rnum = seqsStoreChip(x - (dw * BOOL(flip & 0x8000)),
                                 y + (dh * BOOL(flip & 0x4000)),
                                 dw,
                                 dh,
                                 mt->mltgidx32,
                                 code,
                                 attr | 0x2000,
                                 wk->my_clear_level,
                                 mt->id);
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

void mlt_obj_matrix(WORK* wk, s32 base_y) {
    njSetMatrix(NULL, &BgMATRIX[wk->my_family]);
    njTranslate(NULL, wk->position_x, wk->position_y + base_y, PrioBase[wk->position_z]);

    if (wk->my_mr_flag) {
        njScale(NULL, (1.0f / 64.0f) * (wk->my_mr.size.x + 1), (1.0f / 64.0f) * (wk->my_mr.size.y + 1), 1.0f);
    }
}

void appSetupBasePriority() {
    s32 i;

    for (i = 0; i < PRIO_BASE_SIZE; i++) {
        PrioBaseOriginal[i] = ((i * 512) + 1) / 65535.0f;
    }
}

void appSetupTempPriority() {
    s32 i;

    for (i = 0; i < PRIO_BASE_SIZE; i++) {
        PrioBase[i] = PrioBaseOriginal[i];
    }
}

void appRenewTempPriority_1_Chip() {
    njTranslate(NULL, 0, 0, 1.0f / 65536.0f); // 1 / 2^(-16)
}

void appRenewTempPriority(s32 z) {
    MTX mtx;
    njGetMatrix(&mtx);
    PrioBase[z] = mtx.a[3][2];
}

void seqsInitialize(void* adrs) {
    if (adrs == NULL) {
        while (1) {
            // Do nothing
        }
    }

    seqs_w.chip = (Sprite2*)adrs;
    seqs_w.sprMax = 0;
}

u16 seqsGetSprMax() {
    return seqs_w.sprMax;
}

u32 seqsGetUseMemorySize() {
    return 0xD000;
}

void seqsBeforeProcess() {
    s32 i;

    seqs_w.sprTotal = 0;

    // FIXME: Extract 24 into a define
    for (i = 0; i < 24; i++) {
        seqs_w.up[i] = 0;
    }
}

void seqsAfterProcess() {
    s32 i;
    u32 keep = 0;
    u32 val = 0;

    if ((Debug_w[0x27] != 3) && (seqs_w.sprTotal != 0)) {
        for (i = 0; i < 24; i++) {
            if (seqs_w.up[i]) {
                if (Debug_w[0x22]) {
                    if (ppgCheckTextureDataBe(mts[i].texList.tex) == 0) {
                        seqs_w.up[i] = 0;
                    }
                } else if (ppgRenewTexChunkSeqs(mts[i].texList.tex) == 0) {
                    seqs_w.up[i] = 0;
                }
            }
        }

        if (seqs_w.sprMax < seqs_w.sprTotal) {
            seqs_w.sprMax = seqs_w.sprTotal;
        }

        for (i = 0; i < seqs_w.sprTotal; i++) {
            if (seqs_w.up[seqs_w.chip[i].id]) {
                val = seqs_w.chip[i].texCode;

                if (keep != val) {
                    keep = val;
                    flSetRenderState(FLRENDER_TEXSTAGE0, val);
                }

                ps2SeqsRenderQuad_Ax(&seqs_w.chip[i]);
            }
        }
    }
}

s32 seqsStoreChip(f32 x, f32 y, s32 w, s32 h, s32 gix, s32 code, s32 attr, s32 alpha, s32 id) {
    Sprite2* chip;
    s32 u;
    s32 v;

#if defined(TARGET_PS2)
    f32 dx;
    f32 dy;
#else
    const f32 dx = 0;
    const f32 dy = 0;
#endif

    chip = &seqs_w.chip[seqs_w.sprTotal];
    chip->v[0].x = x;
    chip->v[0].y = y;
    chip->v[1].x = x + w;
    chip->v[1].y = y - h;
    chip->v[0].z = chip->v[1].z = 0.0f;
    njCalcPoint(NULL, &chip->v[0], &chip->v[0]);
    njCalcPoint(NULL, &chip->v[1], &chip->v[1]);

    if ((chip->v[0].x >= 384.0f) || (chip->v[1].x < 0.0f) || (chip->v[0].y >= 224.0f) || (chip->v[1].y < 0.0f)) {
        return 1;
    }

    if (!(attr & 0x2000)) {
        u = (code & 0xF) * 16;
        v = code & 0xF0;
        chip->texCode = ppgGetUsingTextureHandle(NULL, gix + (code >> 8));
    } else {
        u = (code & 7) * 32;
        v = (code & 0x38) * 4;
        chip->texCode = ppgGetUsingTextureHandle(NULL, gix + (code >> 6));
    }

    appRenewTempPriority_1_Chip();

    if (attr & 0x8000) {
#if defined(TARGET_PS2)
        dx = (((chip->v[1].x - chip->v[0].x) / w) > 1.5f) ? 0.34f : 0.68f;
#endif
        chip->t[1].s = (u - dx) / 256.0f;
        chip->t[0].s = (u + w - dx) / 256.0f;
    } else {
#if defined(TARGET_PS2)
        dx = (((chip->v[1].x - chip->v[0].x) / w) > 1.0f) ? 0.0f : 0.5f;
#endif
        chip->t[0].s = (u + dx) / 256.0f;
        chip->t[1].s = (u + w + dx) / 256.0f;
    }

    if (attr & 0x4000) {
#if defined(TARGET_PS2)
        dy = (((chip->v[1].y - chip->v[0].y) / h) > 1.5f) ? 0.34f : 0.68f;
#endif
        chip->t[1].t = (v - dy) / 256.0f;
        chip->t[0].t = (v + h - dy) / 256.0f;
    } else {
#if defined(TARGET_PS2)
        dy = (((chip->v[1].y - chip->v[0].y) / h) > 1.0f) ? 0.0f : 0.5f;
#endif
        chip->t[0].t = (v + dy) / 256.0f;
        chip->t[1].t = (v + h + dy) / 256.0f;
    }

    chip->texCode |= ppgGetUsingPaletteHandle(NULL, attr & 0x1FF) << 16;
    chip->vtxColor = curr_bright | ((0xFF - alpha) << 24);
    chip->id = id;
    seqs_w.sprTotal += 1;

    if (seqs_w.sprTotal > 0x400) {
        // The number of OBJ fragments has exceeded the planned number
        flLogOut("ＯＢＪの破片が予定数を越えてしまいました");
        while (1) {}
    }

    return 1;
}

static s32 get_mltbuf16(MultiTexture* mt, u32 code, u32 palt, s32* ret) {
    s32 i;
    s32 b = -1;
    PatternState* mc = mt->mltcsh16;

    i = mt->mltnum16;

    while (1) {
        if ((mc->cs.code == code) && (mc->state == palt)) {
            mc->time = mt->mltcshtime16;
            *ret = mt->mltnum16 - i;
            return 0;
        }

        if ((mc->cs.code == -1) && (b < 0)) {
            b = i;
        }

        mc++;
        i -= 1;

        if (i <= 0) {
            if (b >= 0) {
                b = mt->mltnum16 - b;
                mt->mltcsh16[b].time = mt->mltcshtime16;
                mt->mltcsh16[b].state = palt;
                mt->mltcsh16[b].cs.code = code;
                *ret = b;
                return 1;
            }

            // CG cache is full. 16x16: %d\n
            flLogOut("ＣＧキャッシュが一杯になりました。１６×１６ : %d\n", mt->id);
            while (1) {}
        }
    }
}

static s32 get_mltbuf32(MultiTexture* mt, u32 code, u32 palt, s32* ret) {
    s32 i;
    s32 b = -1;
    PatternState* mc = mt->mltcsh32;

    i = mt->mltnum32;

    while (1) {
        if ((mc->cs.code == code) && (mc->state == palt)) {
            mc->time = mt->mltcshtime32;
            *ret = mt->mltnum32 - i;
            return 0;
        }

        if ((mc->cs.code == -1) && (b < 0)) {
            b = i;
        }

        mc++;
        i -= 1;

        if (i <= 0) {
            if (b >= 0) {
                b = mt->mltnum32 - b;
                mt->mltcsh32[b].time = mt->mltcshtime32;
                mt->mltcsh32[b].state = palt;
                mt->mltcsh32[b].cs.code = code;
                *ret = b;
                return 1;
            }

            // CG cache is full. 32x32 : %d\n
            flLogOut("ＣＧキャッシュが一杯になりました。３２×３２ : %d\n", mt->id);
            while (1) {}
        }
    }
}

static s32 get_mltbuf16_ext_2(MultiTexture* mt, u32 code, u32 palt, s32* ret, PatternInstance* cp) {
    PatternState* mc = mt->mltcsh16;
    s32 i;

    for (i = 0; i < mt->tpu->x16; i++) {
        if ((code == mc[mt->tpu->x16_used[i]].cs.code) && (palt == mc[mt->tpu->x16_used[i]].state)) {
            *ret = mt->tpu->x16_used[i];

            if (x16_mapping_set(&cp->map, *ret)) {
                cp->x16 += 1;
                mc[mt->tpu->x16_used[i]].time += 1;
            }

            return 0;
        }
    }

    if ((i != mt->mltnum16) && (mt->tpf->x16 != 0)) {
        mt->tpf->x16 -= 1;
        mt->tpu->x16_used[i] = mt->tpf->x16_free[mt->tpf->x16];
        mt->tpu->x16 += 1;
        mc[mt->tpu->x16_used[i]].cs.code = code;
        mc[mt->tpu->x16_used[i]].state = palt;
        *ret = mt->tpu->x16_used[i];
        mc[mt->tpu->x16_used[i]].time = 1;

        if (x16_mapping_set(&cp->map, *ret)) {
            cp->x16 += 1;
        }

        return 1;
    }

    // CG cache is full. x16 EXT2\n
    flLogOut("ＣＧキャッシュが一杯になりました。×１６　ＥＸＴ２\n");
    while (1) {}
}

static s32 get_mltbuf32_ext_2(MultiTexture* mt, u32 code, u32 palt, s32* ret, PatternInstance* cp) {
    PatternState* mc = mt->mltcsh32;
    s32 i;

    for (i = 0; i < mt->tpu->x32; i++) {
        if ((code == mc[mt->tpu->x32_used[i]].cs.code) && (palt == mc[mt->tpu->x32_used[i]].state)) {
            *ret = mt->tpu->x32_used[i];

            if (x32_mapping_set(&cp->map, *ret)) {
                cp->x32 += 1;
                mc[mt->tpu->x32_used[i]].time += 1;
            }

            return 0;
        }
    }

    if ((i != mt->mltnum32) && (mt->tpf->x32 != 0)) {
        mt->tpf->x32 -= 1;
        mt->tpu->x32_used[i] = mt->tpf->x32_free[mt->tpf->x32];
        mt->tpu->x32 += 1;
        mc[mt->tpu->x32_used[i]].cs.code = code;
        mc[mt->tpu->x32_used[i]].state = palt;
        *ret = mt->tpu->x32_used[i];
        mc[mt->tpu->x32_used[i]].time += 1;

        if (x32_mapping_set(&cp->map, *ret)) {
            cp->x32 += 1;
        }

        return 1;
    }

    flLogOut("ＣＧキャッシュが一杯になりました。×３２　ＥＸＴ２\n");
    while (1) {}
}

static s32 get_mltbuf16_ext(MultiTexture* mt, u32 code, u32 palt) {
    PatternState* mc = mt->mltcsh16;
    s32 i;

    for (i = 0; i < tpu_free->x16; i++) {
        if ((code == mc[tpu_free->x16_used[i]].cs.code) && (palt == mc[tpu_free->x16_used[i]].state)) {
            return tpu_free->x16_used[i];
        }
    }

    flLogOut("ＣＧ展開エラー　１６×１６\n");
    while (1) {}
}

static s32 get_mltbuf32_ext(MultiTexture* mt, u32 code, u32 palt) {
    PatternState* mc = mt->mltcsh32;
    s32 i;

    for (i = 0; i < tpu_free->x32; i++) {
        if ((code == mc[tpu_free->x32_used[i]].cs.code) && (palt == mc[tpu_free->x32_used[i]].state)) {
            return tpu_free->x32_used[i];
        }
    }

    flLogOut("ＣＧ展開エラー　３２×３２\n");
    while (1) {}
}

static u16 x16_mapping_set(PatternMap* map, s32 code) {
    u16 num;
    u16 flg;

    flg = 0;
    num = code & 0xF;

    if (!((1 << (num)) & (map->x16_map[code / 256][(code % 256) / 16]))) {
        map->x16_map[code / 256][(code % 256) / 16] |= (1 << num);
        flg = 1;
    }

    return flg;
}

static u16 x32_mapping_set(PatternMap* map, s32 code) {
    u16 flg = 0;
    u8 num = code & 7;

    if (!((map->x32_map[code / 64][(code % 64) / 8]) & (1 << num))) {
        map->x32_map[code / 64][(code % 64) / 8] |= (1 << num);
        flg = 1;
    }

    return flg;
}

void makeup_tpu_free(s32 x16, s32 x32, PatternMap* map) {
    s16 i;
    s16 j;
    s16 k;

    tpu_free->x16 = 0;
    tpu_free->x32 = 0;

    for (i = 0; i < x16; i++) {
        for (j = 0; j < 16; j++) {
            if (map->x16_map[i][j] != 0) {
                for (k = 0; k < 16; k++) {
                    if ((1 << k) & map->x16_map[i][j]) {
                        tpu_free->x16_used[tpu_free->x16] = (i * 256) + (j * 16) + k;
                        tpu_free->x16 += 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < x32; i++) {
        for (j = 0; j < 8; j++) {
            if (map->x32_map[i][j] != 0) {
                for (k = 0; k < 8; k++) {
                    if (map->x32_map[i][j] & (1 << k)) {
                        tpu_free->x32_used[tpu_free->x32] = (i * 64) + (j * 8) + k;
                        tpu_free->x32 += 1;
                    }
                }
            }
        }
    }
}

static s16 check_patcash_ex_trans(PatternCollection* padr, u32 cg) {
    s16 rnum = -1;
    s16 i;

    for (i = 0; i < padr->kazu; i++) {
        if (padr->adr[i]->cg.code == cg) {
            rnum = i;
            break;
        }
    }

    return rnum;
}

static s32 get_free_patcash_index(PatternCollection* padr) {
    s16 i;

    for (i = 0; i < 0x40; i++) {
        if (padr->patt[i].time == 0) {
            return i;
        }
    }

    flLogOut("ＣＧキャッシュバッファが一杯になりました。\n");
    while (1) {}
}

static void lz_ext_p6_fx(u8* srcptr, u8* dstptr, u32 len) {
    u8* endptr = dstptr + len;
    u8* tmpptr;
    u32 tmp;
    u32 flg;

    while (dstptr < endptr) {
        tmp = *srcptr++;

        switch (tmp & 0xC0) {
        case 0x0:
            *dstptr++ = tmp;
            break;

        case 0x40:
            tmp &= 0x3F;
            tmpptr = (dstptr - (tmp >> 2)) - 1;
            tmp = (tmp & 3) + 2;

            while (tmp--) {
                *dstptr++ = *tmpptr++;
            }

            break;

        case 0x80:
            tmp = ((tmp & 0x3F) << 8) | *srcptr++;
            tmpptr = (dstptr - (tmp >> 6)) - 1;
            tmp = (tmp & 0x3F) + 2;

            while (tmp--) {
                *dstptr++ = *tmpptr++;
            }

            break;

        case 0xC0:
            flg = tmp & 0x30;
            tmp = (tmp & 0xF) + 2;

            while (tmp--) {
                *dstptr++ = flg | (*srcptr >> 4);
                *dstptr++ = flg | (*srcptr++ & 0xF);
            }

            break;
        }
    }
}

static void lz_ext_p6_cx(u8* srcptr, u16* dstptr, u32 len, u16* palptr) {
    u16* endptr = dstptr + len;
    u16* tmpptr;
    u32 tmp;
    u32 flg;

    while (dstptr < endptr) {
        tmp = *srcptr++;

        switch (tmp & 0xC0) {
        case 0x0:
            *dstptr++ = palptr[tmp];
            break;

        case 0x40:
            tmp &= 0x3F;
            tmpptr = (dstptr - (tmp >> 2)) - 1;
            tmp = (tmp & 3) + 2;

            while (tmp--) {
                *dstptr++ = *tmpptr++;
            }

            break;

        case 0x80:
            tmp = ((tmp & 0x3F) << 8) | *srcptr++;
            tmpptr = (dstptr - (tmp >> 6)) - 1;
            tmp = (tmp & 0x3F) + 2;

            while (tmp--) {
                *dstptr++ = *tmpptr++;
            }

            break;

        case 0xC0:
            flg = tmp & 0x30;
            tmp = (tmp & 0xF) + 2;

            while (tmp--) {
                *dstptr++ = palptr[flg | (*srcptr >> 4)];
                *dstptr++ = palptr[flg | (*srcptr++ & 0xF)];
            }

            break;
        }
    }
}

void mlt_obj_trans_init(MultiTexture* mt, s32 mode, u8* adrs) {
    PatternState* mc;
    PPGFileHeader ppg;
    s32 i;

    ppg.width = ppg.height = 16;
    ppg.compress = 0;
    ppg.formARGB = 0x1555;
    ppg.transNums = 0;
    mt->texList.tex = &mt->tex;

    switch (mode & 7) {
    case 4:
        ppg.pixel = 0x82;
        mt->texList.pal = NULL;
        break;

    case 2:
        ppg.pixel = 0x81;
        mt->texList.pal = palGetChunkGhostCP3();
        break;

    default:
        ppg.pixel = 0x81;
        mt->texList.pal = palGetChunkGhostDC();
        break;
    }

    mt->texList.tex->be = 0;
    ppgSetupTexChunkSeqs(&mt->tex, &ppg, adrs, mt->mltgidx16, mt->mltnum, mt->attribute);

    if (!(mode & 0x20)) {
        mc = mt->mltcsh16;

        for (i = 0; i < mt->mltnum16; i++) {
            mc->time = 0;
            mc->cs.code = -1;
            mc++;
        }

        mc = mt->mltcsh32;

        for (i = 0; i < mt->mltnum32; i++) {
            mc->time = 0;
            mc->cs.code = -1;
            mc++;
        }
    }
}

void mlt_obj_trans_update(MultiTexture* mt) {
    s32 i;
    PatternState* mc;

    PatternState* assign1;
    PatternState* assign2;

    for (mc = mt->mltcsh16, i = 0; i < mt->mltnum16; i++, mc += 1, assign1 = mc) {
        if (mc->time) {
            if (--mc->time == 0) {
                mc->cs.code = -1;
            }
        }
    }

    for (mc = mt->mltcsh32, i = 0; i < mt->mltnum32; i++, mc += 1, assign2 = mc) {
        if (mc->time) {
            if (--mc->time == 0) {
                mc->cs.code = -1U;
            }
        }
    }
}

void draw_box(f64 arg0, f64 arg1, f64 arg2, f64 arg3, u32 col, u32 attr, s16 prio) {
    f32 px;
    f32 py;
    f32 sx;
    f32 sy;
    Vec3 point[2];
    PAL_CURSOR line;
    PAL_CURSOR_P xy[4];
    PAL_CURSOR_COL cc[4];

    px = arg0;
    py = arg1;
    sx = arg2;
    sy = arg3;
    point[0].x = px;
    point[0].y = py;
    point[0].z = 0.0f;
    point[1].x = px + sx;
    point[1].y = py + sy;
    point[1].z = 0.0f;
    njCalcPoints(NULL, point, point, 2);
    line.p = xy;
    line.col = cc;
    line.tex = NULL;
    line.num = 4;
    line.p[0].x = line.p[2].x = point[0].x;
    line.p[1].x = line.p[3].x = point[1].x;
    line.p[0].y = line.p[1].y = point[0].y;
    line.p[2].y = line.p[3].y = point[1].y;
    line.col[0].color = line.col[1].color = line.col[2].color = line.col[3].color = col;
    njDrawPolygon2D(&line, 4, PrioBase[prio], attr);
    appRenewTempPriority(prio);
}

static void DebugLine(f32 x, f32 y, f32 w, f32 h) {
    Vec3 point[2];
    PAL_CURSOR line;
    PAL_CURSOR_P xy[4];
    PAL_CURSOR_COL cc[4];

    line.p = &xy[0];
    line.col = &cc[0];
    line.tex = NULL;
    line.num = 4;
    point[0].x = x;
    point[0].y = y;
    point[0].z = 1.0f;
    point[1].x = x + w;
    point[1].y = y - h;
    point[1].z = 1.0f;
    njCalcPoints(NULL, point, point, 2);
    line.p[0].x = line.p[2].x = point[0].x;
    line.p[1].x = line.p[3].x = point[1].x;
    line.p[0].y = line.p[1].y = point[0].y;
    line.p[2].y = line.p[3].y = point[1].y;
    line.col[0].color = line.col[1].color = line.col[2].color = line.col[3].color = 0x80FFFFFF;
    njDrawPolygon2D(&line, 4, PrioBase[1], 0x20);
}

void mlt_obj_melt2(MultiTexture* mt, u16 cg_number) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    TEX_GRP_LD* grplds;
    s32 count;
    s32 n;
    s32 i;
    s32 cd16;
    s32 cd32;
    s32 size;
    s32 attr;
    s32 palt;
    s32 wh;
    s32 dd;

    ppgSetupCurrentDataList(&mt->texList);
    grplds = &texgrplds[obj_group_table[cg_number]];

    if (grplds->ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", obj_group_table[cg_number]);
        while (1) {}
    }

    n = *(u32*)grplds->trans_table / 4;
    textbl = (u32*)grplds->texture_table;
    cd16 = 0;
    cd32 = 0;

    for (i = 0; i < n; i++) {
        trsbas = (u16*)(grplds->trans_table + ((u32*)grplds->trans_table)[i]);
        count = *trsbas;
        trsbas++;
        trsptr = (TileMapEntry*)trsbas;

        while (count != 0) {
            attr = trsptr->attr;

            if (!(attr & 0x1000)) {
                texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
                dd = (((texptr->wh & 0xE0) << 5) - 0x400) | (((texptr->wh & 0x1C) << 6) - 0x100);
                wh = (texptr->wh & 3) + 1;
                size = (wh * wh) << 6;
                palt = attr & 3;

                switch (wh) {
                case 1:
                case 2:
                    lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                    njReLoadTexturePartNumG(mt->mltgidx16 + (cd16 >> 8), (s8*)mt->mltbuf, cd16 & 0xFF, size);
                    attr = (attr & 0xC000) | 0x1000 | dd;
                    trsptr->attr |= 0x1000;
                    attr |= palt;
                    search_trsptr(grplds->trans_table, i, n, trsptr->code, palt, cd16, attr);
                    trsptr->code = cd16;
                    trsptr->attr = attr;
                    cd16 += 1;
                    break;

                case 4:
                    lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                    njReLoadTexturePartNumG(mt->mltgidx32 + (cd32 >> 6), (s8*)mt->mltbuf, cd32 & 0x3F, size);
                    attr = (attr & 0xC000) | 0x3000 | dd;
                    trsptr->attr |= 0x1000;
                    attr |= palt;
                    search_trsptr(grplds->trans_table, i, n, trsptr->code, palt, cd32, attr);
                    trsptr->code = cd32;
                    trsptr->attr = attr;
                    cd32 += 1;
                    break;
                }
            }

            count -= 1;
            trsptr++;
        }
    }

    ppgRenewTexChunkSeqs(NULL);
}
