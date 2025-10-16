/**
 * @file texcash.c
 * Texture Cache Manager
 */

#include "sf33rd/Source/Game/rendering/texcash.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "structs.h"

typedef struct {
    s32 p16;
    s32 p32;
    s32 gix;
    s16 life16;
    s16 life32;
    s16 type;
    u16 mode;
    u32 attribute;
} MTSBase;

// sdata
s8* texcash_name[29] = { "    16x16 (tm)  32x32 (tm) GIX      (mn.nw)",
                         "QA",
                         "HT",
                         "1P",
                         "2P",
                         "CA",
                         "SW",
                         "OB",
                         "ED",
                         "DM",
                         "OT",
                         "MJ",
                         "MS",
                         "SL",
                         "EU",
                         "RR",
                         "GI",
                         "xx",
                         "xx",
                         "xx",
                         "xx",
                         "xx",
                         "xx",
                         "xx",
                         "   /   (  )    /   (  )     +    (  .  )",
                         "--- --- --  --- --- --  ---   --  -- --",
                         "--- --- --",
                         "--",
                         "--.--" };

// sbss
u8* texcash_melt_buffer;
TexturePoolUsed* tpu_free;
s16 mts_ob_curr_stage;

// forward decls
extern const s16 mts_OB_page[22][2];
extern const MTSBase mts_base[24];
void clear_texcash_work(s16 ix);

void disp_texcash_free_area() {
    s16 i;

    if (Debug_w[11]) {
        flPrintColor(0xFF8F8F8F);

        for (i = 0; i < 24; i++) {
            flPrintL(13, i + 8, texcash_name[i]);
            if (i) {
                flPrintL(16, i + 8, texcash_name[24]);
            }
        }

        flPrintColor(0xFFCFCFCF);
        for (i = 1; i < 24; i++) {
            if (mts_ok[i].be) {
                if (mts[i].mltnum16 != 0) {
                    flPrintL(16, i + 8, "%3X", mts_ok[i].min16);
                    flPrintL(20, i + 8, "%3X", mts[i].mltnum16);

                    if (mts[i].mltcshtime16 != 0) {
                        flPrintL(24, i + 8, "%2d", mts[i].mltcshtime16);
                    } else {
                        flPrintL(24, i + 8, texcash_name[27]);
                    }
                } else {
                    flPrintL(16, i + 8, texcash_name[26]);
                }

                if (mts[i].mltnum32 != 0) {
                    flPrintL(28, i + 8, "%3X", mts_ok[i].min32);
                    flPrintL(32, i + 8, "%3X", mts[i].mltnum32);

                    if (mts[i].mltcshtime32 != 0) {
                        flPrintL(36, i + 8, "%2d", mts[i].mltcshtime32);
                    } else {
                        flPrintL(36, i + 8, texcash_name[27]);
                    }
                } else {
                    flPrintL(28, i + 8, texcash_name[26]);
                }

                flPrintL(40, i + 8, "%3X", mts[i].mltgidx16);
                flPrintL(46, i + 8, "%2X", mts[i].mltnum);

                if ((i == 7) && (mts_ob_curr_stage != bg_w.stage)) {
                    flPrintColor(0xFFFF8F8F);
                    flPrintL(11, i + 8, "!?");
                    flPrintColor(0xFFCFCFCF);
                }

                if (mts[i].ext) {
                    flPrintL(50, i + 8, "%2d", 64 - mts_ok[i].mincg);
                    flPrintL(53, i + 8, "%2d", 64 - mts[i].cpat->kazu);
                } else {
                    flPrintL(50, i + 8, texcash_name[28]);
                }
            } else {
                flPrintL(16, i + 8, texcash_name[25]);
            }
        }
    }
}

void search_texcash_free_area(s16 ix) {
    PatternState* mc;
    s16 i;
    s16 num = 0;

    for (mc = mts[ix].mltcsh16, i = 0; i < mts[ix].mltnum16; i++) {
        if (mc[i].cs.code == -1) {
            num++;
        }
    }

    if (num < mts_ok[ix].min16) {
        mts_ok[ix].min16 = num;
    }

    num = 0;
    for (mc = mts[ix].mltcsh32, i = 0; i < mts[ix].mltnum32; i++) {
        if (mc[i].cs.code == -1) {
            num++;
        }
    }

    if (num < mts_ok[ix].min32) {
        mts_ok[ix].min32 = num;
    }

    if ((mts[ix].ext) && (mts[ix].cpat->kazu > mts_ok[ix].mincg)) {
        mts_ok[ix].mincg = mts[ix].cpat->kazu;
    }
}

void init_texcash_1st() {
    s16 i;

    for (i = 0; i < MULTITEXTURE_MAX; i++) {
        mts_ok[i].be = 0;
        mts_ok[i].mincg = 0;
        mts_ok[i].min16 = 0x7FFF;
        mts_ok[i].min32 = 0x7FFF;
        mts_ok[i].key0 = 0;
        mts_ok[i].key1 = 0;
        mts[i].mode = -1;
    }
}

void init_texcash_before_process() {
    s16 i;

    for (i = 1; i < 24; i++) {
        if ((mts_ok[i].be) && (mts[i].ext)) {
            init_texcash_2nd(i);
        }
    }
}

void init_texcash_2nd(s16 ix) {
    PatternState* mc;
    PatternCollection* cp;
    TexturePoolFree* tf;
    TexturePoolUsed* tu;
    s16 i;

    tf = mts[ix].tpf;
    tu = mts[ix].tpu;
    cp = mts[ix].cpat;
    tf->x32 = 0;
    tf->x16 = 0;
    tu->x32 = 0;
    tu->x16 = 0;
    mc = mts[ix].mltcsh16;

    for (i = mts[ix].mltnum16 - 1; i >= 0; i--) {
        if (mc[i].cs.code == -1) {
            tf->x16_free[tf->x16] = i;
            tf->x16 += 1;
        } else {
            tu->x16_used[tu->x16] = i;
            tu->x16 += 1;
        }
    }

    mc = mts[ix].mltcsh32;

    for (i = mts[ix].mltnum32 - 1; i >= 0; i--) {
        if (mc[i].cs.code == -1) {
            tf->x32_free[tf->x32] = i;
            tf->x32 += 1;
        } else {
            tu->x32_used[tu->x32] = i;
            tu->x32 += 1;
        }
    }

    cp->kazu = 0;

    for (i = 0; i < 0x40; i++) {
        if (cp->patt[i].time) {
            cp->adr[cp->kazu] = &cp->patt[i];
            cp->kazu += 1;
        }
    }
}

void texture_cash_update() {
    s16 i;
    s16 num;

    for (num = 0; num < 24; num++) {
        if (mts_ok[num].be != 0) {
            if (mts[num].ext) {
                for (i = 0; i < mts[num].cpat->kazu; i++) {
                    if ((--mts[num].cpat->adr[i]->time) == 0) {
                        makeup_tpu_free(mts[num].mltnum16 / 256, mts[num].mltnum32 / 64, &mts[num].cpat->adr[i]->map);

                        if ((tpu_free->x16 != mts[num].cpat->adr[i]->x16) ||
                            (tpu_free->x32 != mts[num].cpat->adr[i]->x32)) {
                            Debug_w[11] = 1;
                            do {
                                disp_texcash_free_area();
                                flPrintL(2, 3, "MAPPING MISS : %2d : &2d", num, i);
                                njWaitVSync_with_N();
                            } while (1);
                        }

                        update_with_tpu_free(mts[num].mltcsh16, mts[num].mltcsh32);
                    }
                }
            } else {
                if ((mts[num].mltcshtime16 + mts[num].mltcshtime32) != 0) {
                    mlt_obj_trans_update(&mts[num]);
                }
            }

            search_texcash_free_area(num);
        }
    }
    disp_texcash_free_area();
}

void update_with_tpu_free(PatternState* mc16, PatternState* mc32) {
    s16 i;

    for (i = 0; i < tpu_free->x16; i++) {
        mc16[tpu_free->x16_used[i]].time -= 1;
        if (mc16[tpu_free->x16_used[i]].time < 0) {
            Debug_w[11] = 1;
            do {
                disp_texcash_free_area();
                flPrintL(2, 3, "CACHE MISS x16 : %3d", tpu_free->x16_used[i]);
                njWaitVSync_with_N();
            } while (1);
        }

        if (mc16[tpu_free->x16_used[i]].time <= 0) {
            mc16[tpu_free->x16_used[i]].cs.code = -1;
        }
    }

    for (i = 0; i < tpu_free->x32; i++) {
        mc32[tpu_free->x32_used[i]].time -= 1;
        if (mc32[tpu_free->x32_used[i]].time < 0) {
            Debug_w[11] = 1;
            do {
                disp_texcash_free_area();
                flPrintL(2, 3, "CACHE MISS x32 : %3d", tpu_free->x32_used[i]);
                njWaitVSync_with_N();
            } while (1);
        }

        if (mc32[tpu_free->x32_used[i]].time <= 0) {
            mc32[tpu_free->x32_used[i]].cs.code = -1;
        }
    }
}

s16 get_my_trans_mode(s16 curr) {
    if (mts_ok[curr].be == 0) {
        return -1;
    }

    return mts[curr].mode;
}

void make_texcash_work(s16 ix) {
    size_t memreq;
    u8* adrs;
    // For some reason page16 is reused later as a pointer.
    // That's why it's uintptr_t and not u32 like page32.
    // I guess the devs were too lazy to make another var or something.
    uintptr_t page16;
    u32 page32;

    if (mts_ok[ix].be) {
        if ((Test_ramcnt_key(mts_ok[ix].key0) != 0) && (Test_ramcnt_key(mts_ok[ix].key1) != 0)) {
            return;
        }

        Debug_w[10] = 2;

        while (1) {
            disp_ramcnt_free_area();
            flPrintL(5, 30, "TEXCASH KEY ERROR");
            njWaitVSync_with_N();
        }
    } else {
        if (ix == 7) {
            page16 = mts_OB_page[bg_w.stage][0];
            page32 = mts_OB_page[bg_w.stage][1];
            mts_ob_curr_stage = bg_w.stage;
        } else {
            page16 = mts_base[ix].p16;
            page32 = mts_base[ix].p32;
        }

        mts[ix].mltnum16 = (u32)page16 << 8;
        mts[ix].mltnum32 = page32 << 6;
        mts[ix].mltnum = (u32)page16 + page32;
        mts[ix].mltgidx16 = mts_base[ix].gix;
        mts[ix].mltgidx32 = (u32)page16 + mts_base[ix].gix;
        mts[ix].mltcshtime16 = mts_base[ix].life16;
        mts[ix].mltcshtime32 = mts_base[ix].life32;

        if ((mts[ix].ext = ((mts_base[ix].mode & 0x2000) != 0))) {
            memreq = (mts[ix].mltnum16 * 8) + (mts[ix].mltnum32 * 8) + sizeof(PatternCollection) +
                     sizeof(TexturePoolFree) + sizeof(TexturePoolUsed);
            mts_ok[ix].key0 = Pull_ramcnt_key(memreq, mts_base[ix].type, 0, 0);
            adrs = (u8*)Get_ramcnt_address(mts_ok[ix].key0);
            mts[ix].mltcsh16 = (PatternState*)adrs;
            adrs += mts[ix].mltnum16 * 8;
            mts[ix].mltcsh32 = (PatternState*)adrs;
            adrs += mts[ix].mltnum32 * 8;
            mts[ix].cpat = (PatternCollection*)adrs;
            adrs += sizeof(PatternCollection);
            mts[ix].tpf = (TexturePoolFree*)adrs;
            adrs += sizeof(TexturePoolFree);
            mts[ix].tpu = (TexturePoolUsed*)adrs;
            work_init_zero((s32*)mts[ix].cpat, sizeof(PatternCollection));
            work_init_zero((s32*)mts[ix].tpf, sizeof(TexturePoolFree));
            work_init_zero((s32*)mts[ix].tpu, sizeof(TexturePoolUsed));
            init_texcash_2nd(ix);
        } else {
            memreq = mts[ix].mltnum16 * 8 + mts[ix].mltnum32 * 8;
            mts_ok[ix].key0 = Pull_ramcnt_key(memreq, mts_base[ix].type, 0, 0);
            adrs = (u8*)Get_ramcnt_address(mts_ok[ix].key0);
            mts[ix].mltcsh16 = (PatternState*)adrs;
            adrs += mts[ix].mltnum16 * 8;
            mts[ix].mltcsh32 = (PatternState*)adrs;
        }

        mts[ix].mltbuf = texcash_melt_buffer;
        memreq = ((mts_base[ix].mode & 4) != 0) + 1;
        memreq *= (mts[ix].mltnum << 0x10);
        mts_ok[ix].key1 = Pull_ramcnt_key(memreq, mts_base[ix].type, 0, 0);
        mts[ix].attribute = mts_base[ix].attribute;
        page16 = Get_ramcnt_address(mts_ok[ix].key1);
        mlt_obj_trans_init(&mts[ix], mts_base[ix].mode, (u8*)page16);

        if (mts[ix].ext) {
            init_texcash_2nd(ix);
        }

        mts[ix].id = ix;
        mts[ix].mode = mts_base[ix].mode & 0xFF;
        mts_ok[ix].be = 1;
        mts_ok[ix].mincg = 0;
        mts_ok[ix].min16 = 0x7FFF;
        mts_ok[ix].min32 = 0x7FFF;
    }
}

void Clear_texcash_work() {
    s16 i;

    for (i = 1; i < 24; i++) {
        clear_texcash_work(i);
    }
}

void clear_texcash_work(s16 ix) {
    s16 i;

    if (((mts_ok[ix].be) != 0) && ((mts_base[ix].mode & 0x20) == 0)) {
        for (i = 0; i < mts[ix].mltnum16; i++) {
            mts[ix].mltcsh16[i].time = 0;
            mts[ix].mltcsh16[i].cs.code = -1;
        }

        for (i = 0; i < mts[ix].mltnum32; i++) {
            mts[ix].mltcsh32[i].time = 0;
            mts[ix].mltcsh32[i].cs.code = -1;
        }

        if (mts[ix].ext) {
            work_init_zero((s32*)mts[ix].cpat, sizeof(PatternCollection));
            work_init_zero((s32*)mts[ix].tpf, sizeof(TexturePoolFree));
            work_init_zero((s32*)mts[ix].tpu, sizeof(TexturePoolUsed));
            init_texcash_2nd(ix);
        }

        mts_ok[ix].mincg = 0;
        mts_ok[ix].min16 = 0x7FFF;
        mts_ok[ix].min32 = 0x7FFF;
    }
}

void purge_texcash_work(s16 ix) {
    if (mts_ok[ix].be == 0) {
        return;
    }

    if ((Test_ramcnt_key(mts_ok[ix].key0) != 0) && (Test_ramcnt_key(mts_ok[ix].key1) != 0)) {
        Push_ramcnt_key_original(mts_ok[ix].key0);
        Push_ramcnt_key_original(mts_ok[ix].key1);
    } else {
        Debug_w[10] = 2;

        while (1) {
            disp_ramcnt_free_area();
            flPrintL(5, 30, "TEXCASH KEY ERROR");
            njWaitVSync_with_N();
        }
    }

    ppgReleaseTextureHandle(&mts[ix].tex, -1);
    mts_ok[ix].be = 0;
    mts_ok[ix].key0 = 0;
    mts_ok[ix].key1 = 0;
}

const MTSBase mts_base[24] = {
    { .p16 = 0, .p32 = 0, .gix = 0, .life16 = 0, .life32 = 0, .type = 0, .mode = 0, .attribute = 0 },
    { .p16 = 1, .p32 = 1, .gix = 20, .life16 = 0, .life32 = 0, .type = 8, .mode = 4114, .attribute = 1 },
    { .p16 = 2, .p32 = 4, .gix = 30, .life16 = 8, .life32 = 8, .type = 8, .mode = 4113, .attribute = 1 },
    { .p16 = 3, .p32 = 6, .gix = 40, .life16 = 20, .life32 = 20, .type = 9, .mode = 8209, .attribute = 1 },
    { .p16 = 3, .p32 = 6, .gix = 50, .life16 = 20, .life32 = 20, .type = 9, .mode = 8209, .attribute = 1 },
    { .p16 = 1, .p32 = 4, .gix = 60, .life16 = 2, .life32 = 2, .type = 9, .mode = 8210, .attribute = 1 },
    { .p16 = 1, .p32 = 5, .gix = 70, .life16 = 0, .life32 = 0, .type = 8, .mode = 4113, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 80, .life16 = 12, .life32 = 12, .type = 9, .mode = 8210, .attribute = 1 },
    { .p16 = 2, .p32 = 8, .gix = 1200, .life16 = 16, .life32 = 16, .type = 9, .mode = 4114, .attribute = 1 },
    { .p16 = 4, .p32 = 34, .gix = 500, .life16 = 20, .life32 = 20, .type = 9, .mode = 4129, .attribute = 1 },
    { .p16 = 4, .p32 = 8, .gix = 80, .life16 = 4, .life32 = 4, .type = 9, .mode = 4113, .attribute = 1 },
    { .p16 = 1, .p32 = 2, .gix = 1000, .life16 = 0, .life32 = 0, .type = 8, .mode = 4113, .attribute = 1 },
    { .p16 = 1, .p32 = 0, .gix = 1020, .life16 = 10, .life32 = 10, .type = 9, .mode = 4114, .attribute = 1 },
    { .p16 = 1, .p32 = 6, .gix = 1030, .life16 = 2, .life32 = 2, .type = 8, .mode = 8210, .attribute = 1 },
    { .p16 = 2, .p32 = 6, .gix = 1100, .life16 = 4, .life32 = 4, .type = 8, .mode = 8210, .attribute = 1 },
    { .p16 = 1, .p32 = 2, .gix = 1120, .life16 = 2, .life32 = 2, .type = 8, .mode = 4113, .attribute = 1 },
    { .p16 = 1, .p32 = 2, .gix = 960, .life16 = 2, .life32 = 2, .type = 8, .mode = 4113, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1140, .life16 = 2, .life32 = 2, .type = 8, .mode = 4114, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 },
    { .p16 = 1, .p32 = 1, .gix = 1100, .life16 = 0, .life32 = 0, .type = 8, .mode = 4116, .attribute = 1 }
};

const s16 mts_OB_page[22][2] = { { 1, 1 }, { 1, 3 }, { 1, 2 }, { 1, 1 }, { 1, 2 }, { 1, 1 }, { 1, 2 }, { 1, 2 },
                                 { 1, 2 }, { 1, 1 }, { 1, 1 }, { 1, 2 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 2 },
                                 { 1, 2 }, { 1, 4 }, { 1, 2 }, { 1, 4 }, { 1, 1 }, { 1, 2 } };
