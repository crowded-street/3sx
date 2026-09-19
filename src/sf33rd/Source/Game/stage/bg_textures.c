/**
 * @file bg_textures.c
 * Background/stage texture loading, split from bg.c.
 */

#include "sf33rd/Source/Game/stage/bg.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_state.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "structs.h"

typedef struct {
    void* adrs;
    u32 size;
} TextureSource;

void Bg_TexInit() {
    s32 i;

    for (i = 0; i < 3; i++) {
        ppgBgList[i].tex = &ppgBgTex[i];
        ppgBgList[i].pal = palGetChunkGhostCP3();
    }

    ppgRwBgList.tex = &ppgRwBgTex;
    ppgRwBgList.pal = palGetChunkGhostCP3();
    ppgAkeList.tex = &ppgAkeTex;
    ppgAkeList.pal = &ppgAkePal;
    ppgAkaneList.tex = &ppgAkaneTex;
    ppgAkaneList.pal = &ppgAkanePal;
}

static void set_default_kakikae() {
    u8 i;
    const bgrw_data_tbl_elem* rwtbl_ptr;
    s8 rw;

    if (bg_w.stage == 7) {
        tokusyu_stage = 4;
    } else {
        tokusyu_stage = 0;
    }

    rw_num = 0;

    for (i = 0; i < 4; i++) {
        rw_bg_flag[i] = 0;
    }

    for (i = 0; i < 8; i++) {
        rw = bgrw_on[bg_w.stage][i];

        if (rw == -1) {
            break;
        }

        rw_num += 1;

        rwtbl_ptr = &bgrw_data_tbl[rw];
        rw_dat[i].bg_num = rwtbl_ptr->bg_num;
        rw_bg_flag[rw_dat[i].bg_num] = 1;
        rw_dat[i].rwgbix = rwtbl_ptr->rwgbix;
        rw_dat[i].rwd_ptr = rw_dat[i].brw_ptr = rwtbl_ptr->rw_ptr;
        rw_dat[i].rw_cnt = *rw_dat[i].rwd_ptr++;
        rw_dat[i].gbix = *rw_dat[i].rwd_ptr++;
    }
}

static void load_rw_slot_from_table(s32 slot, s8 rw) {
    const bgrw_data_tbl_elem* rwtbl_ptr;

    rwtbl_ptr = &bgrw_data_tbl[rw];
    rw_dat[slot].bg_num = rwtbl_ptr->bg_num;
    rw_dat[slot].rwgbix = rwtbl_ptr->rwgbix;
    rw_dat[slot].rwd_ptr = rw_dat[slot].brw_ptr = rwtbl_ptr->rw_ptr;
    rw_dat[slot].rw_cnt = *rw_dat[slot].rwd_ptr++;
    rw_dat[slot].gbix = *rw_dat[slot].rwd_ptr++;
}

static void set_stage03_kakikae() {
    u8 i;
    s8 rw;

    tokusyu_stage = 1;
    stage_flash = 0;
    stage_ftimer = 0;
    rw_dat->rwd_ptr = rw_dat->brw_ptr = (s16*)rw30;
    rw_dat->rw_cnt = 2;

    for (i = 0; i < 13; i++) {
        rw_gbix[i] = stage03rw_data_tbl[i];
    }

    rw3col_ptr = (u32*)rw30col;

    for (i = 0; i < 4; i++) {
        rw = bgrw_on[bg_w.stage][i];

        load_rw_slot_from_table(i + 1, rw);
    }
}

static void set_stage19_kakikae() {
    u8 i;
    s8 rw;

    tokusyu_stage = 3;
    stage_flash = 0;
    stage_ftimer = 2;
    rw_dat->rwd_ptr = rw_dat->brw_ptr = (s16*)rw190;
    rw_dat->rw_cnt = 2;

    for (i = 0; i < 4; i++) {
        rw_gbix[i] = stage19rw_data_tbl[i];
    }

    rw = bgrw_on[bg_w.stage][0];

    load_rw_slot_from_table(1, rw);
}

void Bg_Kakikae_Set() {
    switch (bg_w.stage) {
    case 3:
        set_stage03_kakikae();
        break;

    case 10:
        tokusyu_stage = 2;
        yang_ix = 0;
        yang_ix_plus = 0;
        yang_timer = 4;
        break;

    case 19:
        set_stage19_kakikae();
        break;

    default:
        set_default_kakikae();
        break;
    }
}

static void set_default_ending_kakikae(s16 type) {
    u8 i;
    s8 rw;

    if (edrw_num[type][0] != -1) {
        rw = edrw_num[type][0];

        for (i = 0; i < edrw_num[type][1]; i++) {
            const edrw_data* edrw_data_ptr = &edrw_data_tbl[rw + i];
            rw_num += 1;
            rw_dat[i].bg_num = edrw_data_ptr->bg_num;
            rw_bg_flag[rw_dat[i].bg_num] = 1;
            rw_dat[i].rwgbix = edrw_data_ptr->rwgbix;
            rw_dat[i].rwd_ptr = rw_dat[i].brw_ptr = edrw_data_ptr->rw_ptr;
            rw_dat[i].rw_cnt = *rw_dat[i].rwd_ptr++;
            rw_dat[i].gbix = *rw_dat[i].rwd_ptr++;
        }
    }
}

void Ed_Kakikae_Set(s16 type) {
    u8 i;

    rw_num = 0;

    for (i = 0; i < 4; i++) {
        rw_bg_flag[i] = 0;
    }

    switch (type) {
    case 14:
        for (i = 0; i < 20; i++) {
            const gedrw_data* gedrw_data_ptr = &gedrw_data_tbl[i];
            rw_dat[i].rwgbix = gedrw_data_ptr->rwgbix;
            rw_dat[i].rwd_ptr = rw_dat[i].brw_ptr = gedrw_data_ptr->rw_ptr;
        }

        break;

    case 15:
        for (i = 0; i < 16; i++) {
            const cedrw_data* cedrw_data_ptr = &cedrw_data_tbl[i];
            rw_dat[i].rwgbix = cedrw_data_ptr->rwgbix;
            rw_dat[i].rwd_ptr = rw_dat[i].brw_ptr = cedrw_data_ptr->rw_ptr;
        }

        break;

    default:
        set_default_ending_kakikae(type);
        break;
    }
}

void Bg_Close() {
    u32 i;

    tokusyu_stage = 0;
    rw_num = 0;

    for (i = 0; i < 3; i++) {
        ppgReleaseTextureHandle(&ppgBgTex[i], -1);
    }

    ppgReleaseTextureHandle(&ppgRwBgTex, -1);
    ppgReleaseTextureHandle(&ppgAkeTex, -1);
    ppgReleasePaletteHandle(&ppgAkePal, -1);
    ppgReleaseTextureHandle(&ppgAkaneTex, -1);
    ppgReleasePaletteHandle(&ppgAkanePal, -1);
    Screen_Switch = 0;
    Screen_Switch_Buffer = 0;
    bg_disp_off = 0;
}

static void load_ake_stage_textures() {
    u8* akeAdrs;
    s32 akeSize;
    s16 akeKey;
    u8 i;

    if (bg_w.stage != 20 && bg_w.stage != 21) {
        akeKey = Search_ramcnt_type(0x1F);
        akeSize = Get_size_data_ramcnt_key(akeKey);
        akeAdrs = Get_ramcnt_pointer(akeKey);
        ppgSetupCurrentDataList(&ppgAkeList);
        ppgSetupPalChunk(NULL, &(PPGPalChunkArgs){akeAdrs, akeSize, 0, 0, 1});
        ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){akeAdrs, akeSize, 0, 3, 0, 0});

        for (i = 0; i < 3; i++) {
            ppgSetupTexChunk_2nd(NULL, i);
            ppgSetupTexChunk_3rd(NULL, i, 1);
        }

        ppgSourceDataReleased(&ppgAkeList);
    }
}

static void load_stage07_textures(void* loadAdrs, u32 loadSize, u16 accnum) {
    u8 i;

    if (bg_w.stage == 7) {
        ppgSetupCurrentDataList(&ppgAkaneList);
        ppgSetupPalChunk(NULL, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 0, 1});
        ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 0, 3, 0, 0});
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < 3; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i);
            ppgSetupTexChunk_3rd(NULL, i, 1);
        }

        ppgSourceDataReleased(&ppgAkaneList);
    }
}

static u16 load_rewrite_textures(const TextureSource* source, s32 ixNum1st, u8 x, u16 accnum) {
    u8 i;

    if (x) {
        ppgSetupCurrentDataList(&ppgRwBgList);
        ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){source->adrs, source->size, ixNum1st, x, 0, 0});
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < x; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i + ixNum1st);
            ppgSetupTexChunk_3rd(NULL, i + ixNum1st, 1);
        }
    }

    return accnum;
}

static u8 find_first_stage_background(void) {
    u8 stg;

    for (stg = 0; stg < 3; stg++) {
        if (stage_bgw_number[bg_w.stage][stg] != 0) {
            break;
        }
    }

    return stg;
}

static u16 load_stage_screen_textures(const TextureSource* source, u8 stg, u32 tgbix, u16 accnum) {
    u32 mask;
    u32 assign2;
    u8 i;

    mask = 0x80000000;
    ppgSetupCurrentDataList(&ppgBgList[stg]);
    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){source->adrs, source->size, (stg * 64) + 0x84, 32, 0, 0});
    ppgSetupTexChunk_1st_Accnum(0, accnum);

    for (i = 0; i < 32; i++, assign2 = mask >>= 1) {
        if (tgbix & mask) {
            accnum = ppgSetupTexChunk_2nd(NULL, i + ((stg * 64) + 0x84));
            ppgSetupTexChunk_3rd(NULL, i + ((stg * 64) + 0x84), 1);
        }
    }

    return accnum;
}

void Bg_Texture_Load_EX() {
    void* loadAdrs;
    u32 loadSize;
    u32 tgbix;
    u32 prio;
    u32 pmask;
    s16 key1;
    u16 accnum;
    u8 i;
    u8 j;
    u8 x;
    u8 shift;
    u8 stg;

    u32 assign1;
    u8 assign3;

    Bg_TexInit();

    for (i = 0; i < 8; i++) {
        bgPalCodeOffset[i] = 0x12C;
    }

    ending_flag = 0;

    stg = find_first_stage_background();

    for (i = 0; i < use_real_scr[bg_w.stage]; i++) {
        scr_bcm[stg + i] = bg_map_tbl[bg_w.stage][i];
    }

    for (i = 0; i < 3; i++) {
        if (stage_bgw_number[bg_w.stage][i] > 0) {
            Bg_On_R(1 << i);
        }
    }

    if (bg_w.stage == 7) {
        Bg_On_R(4);
    }

    key1 = Search_ramcnt_type(0x12);
    loadAdrs = Get_ramcnt_pointer(key1);
    loadSize = Get_size_data_ramcnt_key(key1);
    pmask = 0xFF000000;
    shift = 0x18;

    for (j = 0; j < 3; j++, shift -= 8, assign1 = pmask >>= 8) {
        prio = stage_priority[bg_w.stage];
        prio &= pmask;
        prio >>= shift;
        bg_priority[j] = prio;
    }

    bg_priority[3] = 70;
    accnum = 0;

    for (j = 0; j < bg_w.scrno; j++, assign3 = stg++) {
        tgbix = bgtex_stage_gbix[bg_w.stage][j];
        accnum = load_stage_screen_textures(&(TextureSource){ loadAdrs, loadSize }, stg, tgbix, accnum);
    }

    x = rewrite_scr[bg_w.stage];
    accnum = load_rewrite_textures(&(TextureSource){ loadAdrs, loadSize }, (stg * 64) + 0x64, x, accnum);

    load_stage07_textures(loadAdrs, loadSize, accnum);
    load_ake_stage_textures();
}

static void load_texture2_chunks(u8 type, u32 tgbix) {
    u32 mask;
    u32 assign;
    u8 i;
    u8 j;

    mask = 0x80000000;

    for (j = 0, i = 0; i < 32; i++, assign = mask >>= 1) {
        if (tgbix & mask) {
            ppgBgList->tex->accnum = etcBgGixCnvTable[type][j];
            ppgSetupTexChunk_2nd(NULL, i + 0x84);
            ppgSetupTexChunk_3rd(NULL, i + 0x84, 1);
            j++;
        }
    }
}

void Bg_Texture_Load2(u8 type) {
    void* loadAdrs;
    u32 loadSize;
    s16 key;
    u32 tgbix;
    u32 prio;
    u32 pmask;
    u8 i;
    u8 shift;

    Bg_TexInit();
    ending_flag = 0;
    tokusyu_stage = 0;
    rw_num = 0;

    for (i = 0; i < 4; i++) {
        rw_bg_flag[i] = 0;
    }

    for (i = 0; i < bg_w.scno; i++) {
        scr_bcm[i] = bg_map_tbl2[type];
        Bg_On_R(1 << i);
    }

    ppgSetupCurrentDataList(ppgBgList);
    ppgReleaseTextureHandle(NULL, -1);
    key = Search_ramcnt_type(0x18);

    if (key == 0) {
        flLogOut("背景用テクスチャが読み込まれていませんでした。\n");
        while (!NULL) {};
    }

    loadSize = Get_size_data_ramcnt_key(key);
    loadAdrs = Get_ramcnt_pointer(key);
    ppgSetupTexChunk_1st(0, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 0x84, 0x20, 0, 0});
    pmask = 0xFF000000;
    shift = 24;
    tgbix = bgtex_etc_gbix[type];
    prio = etc_bg_priority[type];
    prio &= pmask;
    prio >>= shift;
    bg_priority[0] = prio;
    load_texture2_chunks(type, tgbix);

    bgPalCodeOffset[0] = etcBgPalCnvTable[type] + 144;
}

static void setup_ending_type14(void* loadAdrs, u32 loadSize, u16 accnum) {
    u8 i;
    u8 j;

    tokusyu_stage = 5;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gouki_end_gbix[j + (i * 4)] = (j + ((i * 8) + 100));
        }
    }

    ppgSetupCurrentDataList(&ppgAkeList);
    ppgSetupPalChunk(NULL, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 0, 1});
    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 0x1A0, 0x18, 0, 0});
    ppgSetupTexChunk_1st_Accnum(0, accnum);

    for (i = 0; i < 0x18; i++) {
        accnum = ppgSetupTexChunk_2nd(NULL, i + 0x1A0);
        ppgSetupTexChunk_3rd(NULL, i + 0x1A0, 1);
    }
}

static u16 load_ending_screen_textures(s16 type, const TextureSource* source, u8 j, u16 accnum) {
    u32 tgbix[2];
    u32 mask;
    u8 i;
    u8 k;
    u32 assign2;

    tgbix[0] = bgtex_ending_gbix[type][j * 2];
    tgbix[1] = bgtex_ending_gbix[type][(j * 2) + 1];
    mask = 0x80000000;
    ppgSetupCurrentDataList(&ppgBgList[j]);
    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){source->adrs, source->size, (j * 64) + 100, 64, 0, 0});
    ppgSetupTexChunk_1st_Accnum(0, accnum);

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 32; i++, assign2 = mask >>= 1) {
            if (mask & tgbix[k]) {
                accnum = ppgSetupTexChunk_2nd(NULL, i + ((j * 64) + 100 + (k * 32)));
                ppgSetupTexChunk_3rd(NULL, i + ((j * 64) + 100 + (k * 32)), 1);
            }
        }

        mask = 0x80000000;
    }

    return accnum;
}

void Bg_Texture_Load_Ending(s16 type) {
    void* loadAdrs;
    u32 loadSize;
    u16 accnum;
    u32 prio;
    u32 pmask;
    s16 key1;
    u8 i;
    u8 j;
    u8 x;
    u8 shift;

    u32 assign;

    rw_num = 0;
    Bg_TexInit();
    ending_flag = 1;

    for (i = 0; i < end_use_real_scr[type]; i++) {
        scr_bcm[i] = ending_map_tbl[type][i];
    }

    loadSize = load_it_use_any_key2(bgtex_ending_file[type], &loadAdrs, &key1, 2, 0);
    pmask = 0xFF000000;
    shift = 0x18;

    for (j = 0; j < 4; j++, shift -= 8, assign = pmask >>= 8) {
        prio = ending_priority[0];
        prio &= pmask;
        prio >>= shift;
        bg_priority[j] = prio;
    }

    for (accnum = 0, j = 0; j < bg_w.scrno; j++) {
        accnum = load_ending_screen_textures(type, &(TextureSource){ loadAdrs, loadSize }, j, accnum);
    }

    x = ending_rewrite_scr[type];
    accnum = load_rewrite_textures(&(TextureSource){ loadAdrs, loadSize }, (j * 64) + 100, x, accnum);

    switch (type) {
    case 14:
        setup_ending_type14(loadAdrs, loadSize, accnum);
        break;

    case 15:
        tokusyu_stage = 6;
        break;

    case 19:
        tokusyu_stage = 7;
        ppgSetupCurrentDataList(&ppgAkeList);
        ppgSetupPalChunk(NULL, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 0, 1});
        ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 0xE4, 1, 0, 0});
        ppgSetupTexChunk_1st_Accnum(0, accnum);
        accnum = ppgSetupTexChunk_2nd(NULL, 0xE4);
        ppgSetupTexChunk_3rd(NULL, 0xE4, 1);
        break;

    default:
        tokusyu_stage = 7;
        break;
    }

    Push_ramcnt_key(key1);
    Ed_Kakikae_Set(type);
    ppgSourceDataReleased(&ppgBgList[0]);
    ppgSourceDataReleased(&ppgBgList[1]);
    ppgSourceDataReleased(&ppgBgList[2]);
    ppgSourceDataReleased(&ppgRwBgList);
    ppgSourceDataReleased(&ppgAkeList);
}
