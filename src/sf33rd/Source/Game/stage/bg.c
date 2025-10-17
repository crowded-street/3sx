/**
 * @file bg.c
 * Background/Stage logic
 */

#include "sf33rd/Source/Game/stage/bg.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "structs.h"

// sbss
Vertex scrDrawPos[4];
Polygon bgpoly[4];
u8 bg_priority[4];
u16 Screen_Switch;
u16 Screen_Switch_Buffer;
u8 rw_num;
u8 rw_bg_flag[4];
u8 tokusyu_stage;
s32 rw_gbix[13];
s8 stage_flash;
s8 stage_ftimer;
s32 yang_ix_plus;
s8 yang_ix;
s8 yang_timer;
u8 ending_flag;
BackgroundParameters end_prm[8];
u8 gouki_end_gbix[16];
const u32* rw3col_ptr;
u8 bg_disp_off;
s32 bgPalCodeOffset[8];

// bss
BG bg_w;
RW_DATA rw_dat[20];

static void bgRWWorkUpdate();
static void bgDrawOneScreen(s32 bgnum, s32 gixbase, s32* xx, s32* yy, s32 /* unused */, s32 ofsPal,
                            PPGDataList* curDataList);
static void bgDrawOneChip(s32 x, s32 y, s32 xs, s32 ys, s32 gbix, u32 vtxCol, s32 ofsPal);
static void bgAkebonoDraw();
static void ppgCalScrPosition(s32 x, s32 y, s32 xs, s32 ys);

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

void Bg_Kakikae_Set() {
    u8 i;
    const bgrw_data_tbl_elem* rwtbl_ptr;
    s8 rw;

    switch (bg_w.stage) {
    case 3:
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

            rwtbl_ptr = &bgrw_data_tbl[rw];
            rw_dat[i + 1].bg_num = rwtbl_ptr->bg_num;
            rw_dat[i + 1].rwgbix = rwtbl_ptr->rwgbix;
            rw_dat[i + 1].rwd_ptr = rw_dat[i + 1].brw_ptr = rwtbl_ptr->rw_ptr;
            rw_dat[i + 1].rw_cnt = *rw_dat[i + 1].rwd_ptr++;
            rw_dat[i + 1].gbix = *rw_dat[i + 1].rwd_ptr++;
        }
        break;

    case 10:
        tokusyu_stage = 2;
        yang_ix = 0;
        yang_ix_plus = 0;
        yang_timer = 4;
        break;

    case 19:
        tokusyu_stage = 3;
        stage_flash = 0;
        stage_ftimer = 2;
        rw_dat->rwd_ptr = rw_dat->brw_ptr = (s16*)rw190;
        rw_dat->rw_cnt = 2;

        for (i = 0; i < 4; i++) {
            rw_gbix[i] = stage19rw_data_tbl[i];
        }

        rw = bgrw_on[bg_w.stage][0];

        rwtbl_ptr = &bgrw_data_tbl[rw];
        rw_dat[1].bg_num = rwtbl_ptr->bg_num;
        rw_dat[1].rwgbix = rwtbl_ptr->rwgbix;
        rw_dat[1].rwd_ptr = rw_dat[1].brw_ptr = rwtbl_ptr->rw_ptr;
        rw_dat[1].rw_cnt = *rw_dat[1].rwd_ptr++;
        rw_dat[1].gbix = *rw_dat[1].rwd_ptr++;
        break;

    default:
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

        break;
    }
}

void Ed_Kakikae_Set(s16 type) {
    u8 i;
    s8 rw;

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

void Bg_Texture_Load_EX() {
    void* loadAdrs;
    u32 loadSize;
    u32 tgbix;
    u32 prio;
    u32 mask;
    u32 pmask;
    s16 key1;
    u16 accnum;
    u8 i;
    u8 j;
    u8 x;
    u8 shift;
    u8 stg;
    u8* akeAdrs;
    s32 akeSize;
    s16 akeKey;

    u32 assign1;
    u32 assign2;
    u8 assign3;

    mmDebWriteTag("\nSTAGE\n\n");
    Bg_TexInit();

    for (i = 0; i < 8; i++) {
        bgPalCodeOffset[i] = 0x12C;
    }

    ending_flag = 0;

    for (stg = 0; stg < 3; stg++) {
        if (stage_bgw_number[bg_w.stage][stg] != 0) {
            break;
        }
    }

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
    loadAdrs = (void*)Get_ramcnt_address(key1);
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
        mask = 0x80000000;
        ppgSetupCurrentDataList(&ppgBgList[stg]);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, (stg * 64) + 0x84, 32, 0, 0);
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < 32; i++, assign2 = mask >>= 1) {
            if (tgbix & mask) {
                accnum = ppgSetupTexChunk_2nd(NULL, i + ((stg * 64) + 0x84));
                ppgSetupTexChunk_3rd(NULL, i + ((stg * 64) + 0x84), 1);
            }
        }
    }

    x = rewrite_scr[bg_w.stage];

    if (x) {
        ppgSetupCurrentDataList(&ppgRwBgList);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, (stg * 64) + 0x64, x, 0, 0);
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < x; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i + ((stg * 64) + 0x64));
            ppgSetupTexChunk_3rd(NULL, i + ((stg * 64) + 0x64), 1);
        }
    }

    if (bg_w.stage == 7) {
        ppgSetupCurrentDataList(&ppgAkaneList);
        ppgSetupPalChunk(NULL, loadAdrs, loadSize, 0, 0, 1);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 0, 3, 0, 0);
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < 3; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i);
            ppgSetupTexChunk_3rd(NULL, i, 1);
        }

        ppgSourceDataReleased(&ppgAkaneList);
    }

    if (bg_w.stage != 20 && bg_w.stage != 21) {
        akeKey = Search_ramcnt_type(0x1F);
        akeSize = Get_size_data_ramcnt_key(akeKey);
        akeAdrs = (u8*)Get_ramcnt_address(akeKey);
        ppgSetupCurrentDataList(&ppgAkeList);
        ppgSetupPalChunk(NULL, akeAdrs, akeSize, 0, 0, 1);
        ppgSetupTexChunk_1st(NULL, akeAdrs, akeSize, 0, 3, 0, 0);

        for (i = 0; i < 3; i++) {
            ppgSetupTexChunk_2nd(NULL, i);
            ppgSetupTexChunk_3rd(NULL, i, 1);
        }

        ppgSourceDataReleased(&ppgAkeList);
    }
}

void Bg_Texture_Load2(u8 type) {
    void* loadAdrs;
    u32 loadSize;
    s16 key;
    u32 tgbix;
    u32 prio;
    u32 mask;
    u32 pmask;
    u8 i;
    u8 j;
    u8 shift;

    u32 assign;

    mmDebWriteTag("\nBG ETC.\n\n");
    Bg_TexInit();
    (void)assign;
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
    loadAdrs = (void*)Get_ramcnt_address(key);
    ppgSetupTexChunk_1st(0, loadAdrs, loadSize, 0x84, 0x20, 0, 0);
    pmask = 0xFF000000;
    shift = 24;
    tgbix = bgtex_etc_gbix[type];
    mask = 0x80000000;
    prio = etc_bg_priority[type];
    prio &= pmask;
    prio >>= shift;
    bg_priority[0] = prio;

    for (j = 0, i = 0; i < 32; i++, assign = mask >>= 1) {
        if (tgbix & mask) {
            ppgBgList->tex->accnum = etcBgGixCnvTable[type][j];
            ppgSetupTexChunk_2nd(NULL, i + 0x84);
            ppgSetupTexChunk_3rd(NULL, i + 0x84, 1);
            j++;
        }
    }

    bgPalCodeOffset[0] = etcBgPalCnvTable[type] + 144;
}

void Bg_Texture_Load_Ending(s16 type) {
    void* loadAdrs;
    u32 loadSize;
    u16 accnum;
    u32 tgbix[2];
    u32 prio;
    u32 mask;
    u32 pmask;
    s16 key1;
    u8 i;
    u8 j;
    u8 k;
    u8 x;
    u8 shift;

    u32 assign;
    u32 assign2;

    mmDebWriteTag("\nENDING\n\n");
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
        tgbix[0] = bgtex_ending_gbix[type][j * 2];
        tgbix[1] = bgtex_ending_gbix[type][(j * 2) + 1];
        mask = 0x80000000;
        ppgSetupCurrentDataList(&ppgBgList[j]);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, (j * 64) + 100, 64, 0, 0);
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
    }

    x = ending_rewrite_scr[type];

    if (x) {
        ppgSetupCurrentDataList(&ppgRwBgList);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, (j * 64) + 100, x, 0, 0);
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < x; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i + ((j * 64) + 100));
            ppgSetupTexChunk_3rd(NULL, i + ((j * 64) + 100), 1);
        }
    }

    switch (type) {
    case 14:
        tokusyu_stage = 5;

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                gouki_end_gbix[j + (i * 4)] = (j + ((i * 8) + 100));
            }
        }

        ppgSetupCurrentDataList(&ppgAkeList);
        ppgSetupPalChunk(NULL, loadAdrs, loadSize, 0, 0, 1);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 0x1A0, 0x18, 0, 0);
        ppgSetupTexChunk_1st_Accnum(0, accnum);

        for (i = 0; i < 0x18; i++) {
            accnum = ppgSetupTexChunk_2nd(NULL, i + 0x1A0);
            ppgSetupTexChunk_3rd(NULL, i + 0x1A0, 1);
        }

        break;

    case 15:
        tokusyu_stage = 6;
        break;

    case 19:
        tokusyu_stage = 7;
        ppgSetupCurrentDataList(&ppgAkeList);
        ppgSetupPalChunk(NULL, loadAdrs, loadSize, 0, 0, 1);
        ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 0xE4, 1, 0, 0);
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

void scr_trans(u8 bgnm) {
    PPGDataList* curDataList;
    Vec3 point[2];
    s32 xx[2];
    s32 yy[2];
    s32 i;
    s32 x;
    s32 y;
    s32 global_index;
    s32 global_index_real;
    s32 palOffset;
    u32 vtxColor;
    s32 suzi_pos;

    njUnitMatrix(0);
    njScale(0, 1.0f, -1.0f, 1.0f);
    njTranslate(0, 0.0f, -1024.0f, 0.0f);
    njTranslate(0, (s16)bg_prm[bgnm].bg_h_shift, (s16)bg_prm[bgnm].bg_v_shift, 0.0f);
    njScale(0, 1.0f, -1.0f, 1.0f);
    njTranslate(0, 0.0f, -224.0f, 0.0f);
    njScale(0, 1.0f / scr_sc, 1.0f / scr_sc, 1.0f);
    njScale(0, 1.0f / Frame_Zoom_X, 1.0f / Frame_Zoom_Y, 1.0f);
    point[0].x = 0.0f;
    point[0].y = 0.0f;
    point[0].z = 00.f;
    point[1].x = 648.0f;
    point[1].y = 488.0f;
    point[1].z = 0.0f;
    njCalcPoints(0, &point[0], &point[0], 2);
    xx[0] = ((s32)point[0].x) & ~0x7F;
    yy[0] = ((s32)point[0].y) & ~0x7F;
    xx[1] = ((s32)point[1].x + 0x7F) & ~0x7F;
    yy[1] = ((s32)point[1].y + 0x7F) & ~0x7F;

    for (x = 0; x < 2; x++) {
        if (xx[x] < 0) {
            xx[x] = 0;
        }

        if (0x3FF < xx[x]) {
            xx[x] = 0x3FF;
        }

        if (yy[x] < 0) {
            yy[x] = 0;
        }

        if (0x3FF < yy[x]) {
            yy[x] = 0x3FF;
        }
    }

    njUnitMatrix(0);
    njScale(0, Frame_Zoom_X, Frame_Zoom_Y, 1.0);
    njScale(0, scr_sc, scr_sc, 1.0);
    njTranslate(0, 0, 224.0, 0);
    njScale(0, 1.0, -1.0, 1.0);
    njTranslate(0, (s16)-bg_prm[bgnm].bg_h_shift, (s16)-bg_prm[bgnm].bg_v_shift, 0);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
    njTranslate(0, 0, 1024.0, PrioBase[bg_priority[bgnm]]);
    njScale(0, 1.0, -1.0, 1.0);

    if (Debug_w[42]) {
        return;
    }

    palOffset = bgPalCodeOffset[bgnm];

    if (ending_flag == 0) {
        if (bgnm == 3) {
            ppgSetupCurrentDataList(&ppgAkeList);
            bgAkebonoDraw();
            return;
        }

        global_index = (bgnm * 64) + 100;
        ppgSetupCurrentDataList(&ppgBgList[bgnm]);
        curDataList = &ppgBgList[bgnm];
    } else {
        global_index = (bgnm * 64) + 100;
        ppgSetupCurrentDataList(&ppgBgList[bgnm]);
        curDataList = &ppgBgList[bgnm];
    }

    switch (tokusyu_stage) {
    case 1:
        for (y = yy[0]; y < yy[1]; y += 128) {
            for (x = xx[0]; x < xx[1]; x += 128) {
                global_index_real = global_index + (((y >> 7) << 3) + (x >> 7));
                vtxColor = 0xFFFFFFFF;

                if (bgnm == 0) {
                    for (i = 0; i < 4; i++) {
                        if (global_index_real == rw_dat[i + 1].rwgbix) {
                            global_index_real = rw_dat[i + 1].gbix;
                            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                                ppgSetupCurrentDataList(&ppgRwBgList);
                            }
                            break;
                        }
                    }
                } else {
                    for (i = 0; i < 13; i++) {
                        if (global_index_real == rw_gbix[i]) {
                            global_index_real = *(rw_dat[0].rwd_ptr + i + 1);
                            vtxColor = *rw3col_ptr;

                            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                                ppgSetupCurrentDataList(&ppgRwBgList);
                            }
                            break;
                        }
                    }
                }

                bgDrawOneChip(x, y, 128, 128, global_index_real, vtxColor, palOffset);
                ppgSetupCurrentDataList(curDataList);
            }
        }

        if (EXE_flag != 0 || Game_pause != 0) {
            return;
        }

        if (bgnm == 0) {
            for (i = 0; i < 4; i = i + 1) {
                rw_dat[i + 1].rw_cnt--;

                if (rw_dat[i + 1].rw_cnt == 0) {
                    if (rw_dat[i + 1].rwd_ptr[0] == -1) {
                        rw_dat[i + 1].rwd_ptr = rw_dat[i + 1].brw_ptr;
                        rw_dat[i + 1].rw_cnt = *rw_dat[i + 1].rwd_ptr++;
                        rw_dat[i + 1].gbix = *rw_dat[i + 1].rwd_ptr++;
                    } else {
                        rw_dat[i + 1].rw_cnt = *rw_dat[i + 1].rwd_ptr++;
                        rw_dat[i + 1].gbix = *rw_dat[i + 1].rwd_ptr++;
                    }
                }
            }

            break;
        }

        rw_dat[0].rw_cnt--;

        if (rw_dat[0].rw_cnt != 0) {
            break;
        }

        if (stage_flash == 0) {
            rw_dat[0].rwd_ptr += 14;
            rw3col_ptr++;
            rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

            if (rw_dat[0].rw_cnt != -1) {
                break;
            }

            stage_flash = random_16_bg();
            stage_flash = stage03_flash_tbl[stage_flash];

            if (stage_flash == 0) {
                rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr;
                rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];
                rw3col_ptr = &rw30col[0];
            } else {
                rw_dat[0].rwd_ptr = &rw31[0];
                rw_dat[0].rw_cnt = 2;
                stage_ftimer = stage_flash;
                rw3col_ptr = rw31col;
            }

            break;
        }

        rw_dat[0].rwd_ptr += 14;
        rw3col_ptr++;
        rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

        if (rw_dat[0].rw_cnt != -1) {
            break;
        }

        stage_ftimer--;

        if (stage_ftimer < 1) {
            stage_flash = 0;
            rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr;
            rw_dat[0].rw_cnt = 2;
            rw3col_ptr = rw30col;
        } else {
            rw_dat[0].rwd_ptr = rw31;
            rw_dat[0].rw_cnt = 2;
            rw3col_ptr = rw31col;
        }

        break;

    case 2:
        if (judge_flag == 1 && bgnm == 1) {
            vtxColor = 0xFFA0A0A0;
        } else {
            vtxColor = 0xFFFFFFFF;
        }

        for (y = yy[0]; y < yy[1]; y += 128) {
            for (x = xx[0]; x < xx[1]; x += 128) {
                global_index_real = global_index + (((y >> 7) << 3) + (x >> 7));

                if (bgnm == 1) {
                    global_index_real += yang_ix_plus;
                }

                if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                    ppgSetupCurrentDataList(&ppgRwBgList);
                }
                bgDrawOneChip(x, y, 128, 128, global_index_real, vtxColor, palOffset);
                ppgSetupCurrentDataList(curDataList);
            }
        }

        if (EXE_flag != 0 || Game_pause != 0) {
            return;
        }

        if (bgnm != 1) {
            break;
        }

        yang_timer--;

        if (yang_timer != 0) {
            break;
        }

        yang_timer = 4;
        yang_ix++;

        if (yang_ix == 4) {
            yang_ix = 0;
        }

        yang_ix_plus = yang_ix << 5;
        break;

    case 3:
        for (y = yy[0]; y < yy[1]; y += 128) {
            for (x = xx[0]; x < xx[1]; x += 128) {
                global_index_real = global_index + (((y >> 7) << 3) + (x >> 7));

                if (bgnm == 1) {
                    if (rw_dat[1].rwgbix == global_index_real) {
                        global_index_real = rw_dat[1].gbix;

                        if (!ppgCheckTextureNumber(0, global_index_real)) {
                            ppgSetupCurrentDataList(&ppgRwBgList);
                        }
                    } else {
                        for (i = 0; i < 4; i++) {
                            if (global_index_real == rw_gbix[i]) {
                                global_index_real = *(rw_dat[0].rwd_ptr + i + 1);

                                if (!ppgCheckTextureNumber(0, global_index_real)) {
                                    ppgSetupCurrentDataList(&ppgRwBgList);
                                }

                                break;
                            }
                        }
                    }
                }

                bgDrawOneChip(x, y, 128, 128, global_index_real, -1, palOffset);
                ppgSetupCurrentDataList(curDataList);
            }
        }

        if (EXE_flag != 0 || Game_pause != 0) {
            return;
        }

        if (bgnm != 1) {
            break;
        }

        rw_dat[0].rw_cnt--;

        if (rw_dat[0].rw_cnt == 0) {
            rw_dat[0].rwd_ptr += 5;
            rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

            if (rw_dat[0].rw_cnt == -1) {
                stage_ftimer--;

                if (stage_ftimer == 0) {
                    stage_flash = random_16_bg();
                    stage_ftimer = random_16_bg();

                    switch (stage_flash) {
                    case 0:
                    case 1:
                        rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr = rw191;
                        rw_dat[0].rw_cnt = 1;
                        stage_ftimer = stage19_loop_tbl2[stage_ftimer];
                        break;

                    case 2:
                    case 3:
                        rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr = rw192;
                        rw_dat[0].rw_cnt = 1;
                        stage_ftimer = stage19_loop_tbl2[stage_ftimer];
                        break;

                    default:
                        rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr = rw190;
                        rw_dat[0].rw_cnt = 2;
                        stage_ftimer = stage19_loop_tbl1[stage_ftimer];
                        break;
                    }
                } else {
                    rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr;
                    rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];
                }
            }
        }

        rw_dat[1].rw_cnt--;

        if (rw_dat[1].rw_cnt != 0) {
            break;
        }

        if (rw_dat[1].rwd_ptr[0] == -1) {
            rw_dat[1].rwd_ptr = rw_dat[1].brw_ptr;
            rw_dat[1].rw_cnt = *rw_dat[1].rwd_ptr++;
            rw_dat[1].gbix = *rw_dat[1].rwd_ptr++;
        } else {
            rw_dat[1].rw_cnt = *rw_dat[1].rwd_ptr++;
            rw_dat[1].gbix = *rw_dat[1].rwd_ptr++;
        }

        break;

    case 5:
        for (y = yy[0]; y < yy[1]; y += 128) {
            for (x = xx[0]; x < xx[1]; x += 128) {
                global_index_real = global_index + (((y >> 7) << 3) + (x >> 7));

                if (nosekae != 0) {
                    for (i = 0; i < 16; i++) {
                        if (gouki_end_gbix[i] == global_index_real) {
                            global_index_real = gouki_end_nosekae[nosekae - 1][i];

                            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                                if (ppgCheckTextureNumber(&ppgRwBgTex, global_index_real)) {
                                    ppgSetupCurrentDataList(&ppgRwBgList);
                                } else {
                                    ppgSetupCurrentDataList(&ppgAkeList);
                                }
                            }

                            break;
                        }
                    }
                }

                if (bgnm == 0) {
                    if (g_kakikae[0]) {
                        for (i = 0; i < 12; i++) {
                            if (global_index_real == rw_dat[i].rwgbix) {
                                global_index_real = rw_dat[i].rwd_ptr[g_number[0]];

                                if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                                    if (ppgCheckTextureNumber(&ppgRwBgTex, global_index_real)) {
                                        ppgSetupCurrentDataList(&ppgRwBgList);
                                    } else {
                                        ppgSetupCurrentDataList(&ppgAkeList);
                                    }
                                }

                                break;
                            }
                        }
                    }

                    if (g_kakikae[1]) {
                        for (i = 12; i < 20; i++) {
                            if (global_index_real == rw_dat[i].rwgbix) {
                                global_index_real = rw_dat[i].rwd_ptr[g_number[1]];

                                if (!ppgCheckTextureNumber(0, global_index_real)) {
                                    if (ppgCheckTextureNumber(&ppgRwBgTex, global_index_real)) {
                                        ppgSetupCurrentDataList(&ppgRwBgList);
                                    } else {
                                        ppgSetupCurrentDataList(&ppgAkeList);
                                    }
                                }

                                break;
                            }
                        }
                    }
                }

                bgDrawOneChip(x, y, 128, 128, global_index_real, -1, palOffset);
                ppgSetupCurrentDataList(curDataList);
            }
        }

        scr_calc2(bgnm);
        break;

    case 6:
        for (y = yy[0]; y < yy[1]; y += 128) {
            for (x = xx[0]; x < xx[1]; x += 128) {
                global_index_real = global_index + (((y >> 7) << 3) + (x >> 7));

                if (bgnm == 0) {
                    switch (c_kakikae) {
                    case 1:
                        for (i = 0; i < 8; i++) {
                            if (global_index_real == rw_dat[i].rwgbix) {
                                global_index_real = rw_dat[i].rwd_ptr[c_number];

                                if (!ppgCheckTextureNumber(0, global_index_real)) {
                                    ppgSetupCurrentDataList(&ppgRwBgList);
                                }

                                break;
                            }
                        }

                        break;

                    case 2:
                        for (i = 8; i < 16; i++) {
                            if (global_index_real == rw_dat[i].rwgbix) {
                                global_index_real = rw_dat[i].rwd_ptr[c_number];

                                if (!ppgCheckTextureNumber(0, global_index_real)) {
                                    ppgSetupCurrentDataList(&ppgRwBgList);
                                }

                                break;
                            }
                        }
                    }
                }

                bgDrawOneChip(x, y, 128, 128, global_index_real, -1, palOffset);
                ppgSetupCurrentDataList(curDataList);
            }
        }

        scr_calc2(bgnm);
        break;

    case 7:
        bgDrawOneScreen(bgnm, global_index, &xx[0], &yy[0], -1, palOffset, curDataList);

        if (EXE_flag != 0) {
            break;
        }

        if (Game_pause != 0) {
            break;
        }

        if (rw_bg_flag[bgnm] && rw_num) {
            bgRWWorkUpdate();
        }

        scr_calc2(bgnm);
        break;

    case 4:
        if (bgnm == 2) {
            suzi_pos = bg_pos[2].scr_x_buff.word_pos.h - 320;
            suzi_pos = suzi_pos * -0.5f;
            ppgSetupCurrentDataList(&ppgAkaneList);

            for (x = 0; x < 3; x = x + 1) {
                scr_trans_sub2(x * 256 + 128, 128, suzi_pos);

                if (No_Trans == 0) {
                    ppgSetupCurrentPaletteNumber(0, x);
                    njDrawTexture(bgpoly, 4, x, 0);
                }
            }
        }

        /* fallthrough */

    default:
        bgDrawOneScreen(bgnm, global_index, &xx[0], &yy[0], -1, palOffset, curDataList);

        if (EXE_flag == 0 && Game_pause == 0 && rw_bg_flag[bgnm] && rw_num) {
            bgRWWorkUpdate();
        }

        break;
    }
}

void bgRWWorkUpdate() {
    s32 i;

    for (i = 0; i < rw_num; i++) {
        rw_dat[i].rw_cnt--;

        if (rw_dat[i].rw_cnt == 0) {
            if (*rw_dat[i].rwd_ptr == -1) {
                rw_dat[i].rwd_ptr = rw_dat[i].brw_ptr;
                rw_dat[i].rw_cnt = *rw_dat[i].rwd_ptr++;
                rw_dat[i].gbix = *rw_dat[i].rwd_ptr++;
            } else {
                rw_dat[i].rw_cnt = *rw_dat[i].rwd_ptr++;
                rw_dat[i].gbix = *rw_dat[i].rwd_ptr++;
            }
        }
    }
}

void bgDrawOneScreen(s32 bgnum, s32 gixbase, s32* xx, s32* yy, s32 /* unused */, s32 ofsPal, PPGDataList* curDataList) {
    s32 i, x, y, gbix;

    for (y = yy[0]; y < yy[1]; y += 128) {
        for (x = xx[0]; x < xx[1]; x += 128) {
            gbix = ((y >> 7) << 3) + (x >> 7) + gixbase;

            if (rw_bg_flag[bgnum] && rw_num) {
                for (i = 0; i < rw_num; i++) {
                    if (bgnum == rw_dat[i].bg_num && gbix == rw_dat[i].rwgbix) {
                        gbix = rw_dat[i].gbix;
                        if (!(ppgCheckTextureNumber(0, gbix))) {
                            ppgSetupCurrentDataList(&ppgRwBgList);
                        }
                        break;
                    }
                }
            }

            bgDrawOneChip(x, y, 128, 128, gbix, -1, ofsPal);
            ppgSetupCurrentDataList(curDataList);
        }
    }
}

void bgDrawOneChip(s32 x, s32 y, s32 xs, s32 ys, s32 gbix, u32 vtxCol, s32 ofsPal) {
    if ((No_Trans == 0) && ppgCheckTextureNumber(0, gbix)) {
        ppgCalScrPosition(x, y, xs, ys);

        if ((scrDrawPos->x >= 384.0f) || (scrDrawPos[3].x < 0.0f) || (scrDrawPos->y >= 224.0f) ||
            (scrDrawPos[3].y < 0.0f)) {
            return;
        }

        ppgWriteQuadUseTrans(scrDrawPos, vtxCol, 0, gbix, 0, 0, ofsPal);
    }
}

void bgAkebonoDraw() {
    s32 i;

    scrDrawPos->x = 0.0f;
    scrDrawPos->y = 0.0f;
    scrDrawPos[3].x = 128.0f;
    scrDrawPos[3].y = 224.0f;
    scrDrawPos->z = scrDrawPos[3].z = PrioBase[bg_priority[3]];
    scrDrawPos->s = scrDrawPos->t = 0.0f;
    scrDrawPos[3].s = 1.0f;
    scrDrawPos[3].t = 0.875f;

    for (i = 0; i < 3; i++) {
        ppgWriteQuadUseTrans(scrDrawPos, 0xFFFFFFFF, NULL, i, i, 0, 0);
        scrDrawPos->x += 128.0f;
        scrDrawPos[3].x += 128.0f;
    }
}

void ppgCalScrPosition(s32 x, s32 y, s32 xs, s32 ys) {
    Vec3 point[2];

    point[0].x = (f32)x;
    point[0].y = (f32)y;
    point[1].x = (f32)(x + xs);
    point[1].y = (f32)(y + ys);
    point[0].z = point[1].z = 0;
    njCalcPoints(0, point, point, 2);
    scrDrawPos[0].x = scrDrawPos[2].x = point[0].x;
    scrDrawPos[0].y = scrDrawPos[1].y = point[0].y;
    scrDrawPos[1].x = scrDrawPos[3].x = point[1].x;
    scrDrawPos[2].y = scrDrawPos[3].y = point[1].y;
    scrDrawPos[0].z = scrDrawPos[1].z = scrDrawPos[2].z = scrDrawPos[3].z = point[0].z;

    scrDrawPos[0].s = (f32)(x & 0x7F) / 128.0f;
    scrDrawPos[0].t = (f32)(y & 0x7F) / 128.0f;
    scrDrawPos[3].s = (f32)((x & 0x7F) + xs) / 128.0f;
    scrDrawPos[3].t = (f32)((y & 0x7F) + ys) / 128.0f;
    scrDrawPos[1].s = scrDrawPos[3].s;
    scrDrawPos[2].s = scrDrawPos[0].s;
    scrDrawPos[1].t = scrDrawPos[0].t;
    scrDrawPos[2].t = scrDrawPos[3].t;
}

void scr_trans_sub2(s32 x, s32 y, s32 suzi) {
    Vec3 point[2];
    Vec3 spoint[2];

    point[0].x = (f32)x;
    spoint[0].x = (f32)(x + suzi);
    point[0].y = spoint[0].y = (f32)(y + 0x200);
    point[1].x = (f32)(x + 0x100);
    spoint[1].x = (f32)(x + suzi + 0x100);
    point[1].y = spoint[1].y = (f32)(y + 0x300);
    point[0].z = point[1].z = spoint[0].z = spoint[1].z = 0;
    njCalcPoints(NULL, &point[0], &point[0], 2);
    njCalcPoints(NULL, &spoint[0], &spoint[0], 2);
    bgpoly[0].x = spoint[0].x;
    bgpoly[0].y = point[0].y;
    bgpoly[0].z = point[0].z;
    bgpoly[0].u = 0.0f;
    bgpoly[0].v = 0.0f;
    bgpoly[0].col = 0xFFFFFFFF;
    bgpoly[1].x = spoint[1].x;
    bgpoly[1].y = point[0].y;
    bgpoly[1].z = point[0].z;
    bgpoly[1].u = 1.0f;
    bgpoly[1].v = 0.0f;
    bgpoly[1].col = 0xFFFFFFFF;
    bgpoly[2].x = point[0].x;
    bgpoly[2].y = point[1].y;
    bgpoly[2].z = point[1].z;
    bgpoly[2].u = 0.0f;
    bgpoly[2].v = 1.0f;
    bgpoly[2].col = 0xFFFFFFFF;
    bgpoly[3].x = point[1].x;
    bgpoly[3].y = point[1].y;
    bgpoly[3].z = point[1].z;
    bgpoly[3].u = 1.0f;
    bgpoly[3].v = 1.0f;
    bgpoly[3].col = 0xFFFFFFFF;
}

void scr_calc(u8 bgnm) {
    njUnitMatrix(NULL);
    njScale(NULL, Frame_Zoom_X, Frame_Zoom_Y, 1.0f);
    njScale(NULL, scr_sc, scr_sc, 1.0f);
    njTranslate(NULL, 0.0f, 224.0f, 0.0f);
    njScale(NULL, 1.0f, -1.0f, 1.0f);
    njTranslate(NULL, (s16)-bg_prm[bgnm].bg_h_shift, (s16)-bg_prm[bgnm].bg_v_shift, 0.0f);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
}

void scr_calc2(u8 bgnm) {
    njUnitMatrix(NULL);
    njScale(NULL, Frame_Zoom_X, Frame_Zoom_Y, 1.0f);
    njScale(NULL, scr_sc, scr_sc, 1.0f);
    njTranslate(NULL, 0.0f, 224.0f, 0.0f);
    njScale(NULL, 1.0f, -1.0f, 1.0f);
    njTranslate(NULL, (s16)-end_prm[bgnm + 1].bg_h_shift, (s16)-end_prm[bgnm + 1].bg_v_shift, 0.0f);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
}

void Pause_Family_On() {
    njUnitMatrix(0);
    njScale(0, Frame_Zoom_X, Frame_Zoom_Y, 1);
    njTranslate(0, 0, 224, 0);
    njScale(0, 1, -1, 1);
    njGetMatrix(&BgMATRIX[8]);
}

void Zoomf_Init() {
    zoom_add = 64;
    scr_sc = 1.0f;
    sca_x = 0;
    sca_y = 1023;
    scrn_adgjust_x = 0;
    scrn_adgjust_y = 0;
}

void Zoom_Value_Set(u16 zadd) {
    f32 work;
    u16 add;

    if (zadd < 0x40) {
        scr_sc = 64.0f / zadd;
        return;
    }

    if (zadd == 0x40) {
        scr_sc = 1.0f;
        return;
    }

    add = zadd & 0x3F;
    work = 1.0f / (64.0f / add);
    add = zadd & 0xFFC0;
    add >>= 6;
    scr_sc = 1.0f / (add + work);
}

void Frame_Up(u16 x, u16 y, u16 add) {
    if (zoom_add < 2) {
        scr_sc = 64.0f;
        return;
    }

    zoom_add -= add;
    Zoom_Value_Set(zoom_add);
    Frame_Adgjust(x, y);
}

void Frame_Down(u16 x, u16 y, u16 add) {
    if (zoom_add >= 0xFFC0) {
        scr_sc = 0.0009775171f;
        return;
    }

    zoom_add += add;
    Zoom_Value_Set(zoom_add);
    Frame_Adgjust(x, y);
}

void Frame_Adgjust(u16 pos_x, u16 pos_y) {
    u16 buff;

    if (zoom_add >= 0x40) {
        buff = zoom_add;
        buff -= 0x40;
        buff *= pos_x;
        buff >>= 6;
        buff &= 0x1FF;
        scrn_adgjust_x = -buff;
    } else {
        buff = 0x40;
        buff -= zoom_add;
        buff *= pos_x;
        buff >>= 6;
        buff &= 0x1FF;
        scrn_adgjust_x = buff;
    }

    if (zoom_add >= 0x40) {
        buff = zoom_add;
        buff -= 0x40;
        buff *= pos_y + 0x15;
        buff >>= 6;
        buff &= 0x1FF;
        scrn_adgjust_y = -buff;

        if (scrn_adgjust_y == -0x14) {
            scrn_adgjust_y += 1;
        }
    } else {
        buff = 0x40;
        buff -= zoom_add;
        buff *= pos_y + 0x15;
        buff >>= 6;
        buff &= 0x1FF;
        scrn_adgjust_y = buff;

        if (scrn_adgjust_y == -0x14) {
            scrn_adgjust_y += 1;
        }
    }
}

void Scrn_Pos_Init() {
    u8 i;

    for (i = 0; i < 8; i++) {
        bg_pos[i].scr_x.long_pos = 0;
        bg_pos[i].scr_x_buff.long_pos = 0;
        bg_pos[i].scr_y.long_pos = 0;
        bg_pos[i].scr_y_buff.long_pos = 0;
        bg_prm[i].bg_h_shift = 0;
        bg_prm[i].bg_v_shift = 0;
        end_prm[i].bg_h_shift = 0;
        end_prm[i].bg_v_shift = 0;
    }
}

void Scrn_Move_Set(s8 bgnm, s16 x, s16 y) {
    bg_pos[bgnm].scr_x.word_pos.h = x;
    bg_pos[bgnm].scr_y.word_pos.h = y + 16;
}

void Family_Init() {
    u8 i;

    for (i = 0; i < 8; i++) {
        fm_pos[i].family_x.long_pos = 0;
        fm_pos[i].family_y.long_pos = 0;
        fm_pos[i].family_x_buff.long_pos = 0;
        fm_pos[i].family_y_buff.long_pos = 0;
    }
}

void Family_Set_R(s8 fmnm, s16 x, s16 y) {
    fm_pos[fmnm].family_x.word_pos.h = x;
    fm_pos[fmnm].family_y.word_pos.h = y;
    fm_pos[fmnm].family_x_buff.word_pos.h = x;
    fm_pos[fmnm].family_y_buff.word_pos.h = y;
}

void Family_Set_W(s8 fmnm, s16 x, s16 y) {
    fm_pos[fmnm].family_x.word_pos.h = x;
    fm_pos[fmnm].family_y.word_pos.h = y;
    fm_pos[fmnm].family_x_buff.word_pos.h = x;
    fm_pos[fmnm].family_y_buff.word_pos.h = y;
}

void Bg_On_R(u16 s_prm) {
    Screen_Switch |= s_prm;
    Screen_Switch_Buffer = Screen_Switch;
}

void Bg_On_W(u16 s_prm) {
    Screen_Switch |= s_prm;
    Screen_Switch_Buffer = Screen_Switch;
}

void Bg_Off_R(u16 s_prm) {
    s_prm = ~s_prm;
    Screen_Switch &= s_prm;
    Screen_Switch_Buffer = Screen_Switch;
}

void Bg_Off_W(u16 s_prm) {
    s_prm = ~s_prm;
    Screen_Switch &= s_prm;
    Screen_Switch_Buffer = Screen_Switch;
}

void Scrn_Renew() {
    Screen_Switch_Buffer = Screen_Switch;
}

void Irl_Family() {
    u8 i;

    for (i = 0; i < 8; i++) {
        fm_pos[i].family_x_buff.long_pos = fm_pos[i].family_x.long_pos;
        fm_pos[i].family_y_buff.long_pos = fm_pos[i].family_y.long_pos;
        bg_pos[i].scr_x_buff.long_pos = bg_pos[i].scr_x.long_pos;
        bg_pos[i].scr_y_buff.long_pos = bg_pos[i].scr_y.long_pos;
    }
}

void Irl_Scrn() {
    s8 i;

    for (i = 0; i < 8; i++) {
        bg_prm[i].bg_h_shift = scrn_adgjust_x + bg_pos[i].scr_x_buff.word_pos.h;
        end_prm[i].bg_h_shift = scrn_adgjust_x + fm_pos[i].family_x_buff.word_pos.h;
        bg_prm[i].bg_v_shift = bg_pos[i].scr_y_buff.word_pos.h - scrn_adgjust_y;
        end_prm[i].bg_v_shift = fm_pos[i].family_y_buff.word_pos.h - scrn_adgjust_y;
    }
}

void Family_Move() {
    u8 assign;
    u8 fam_ix;
    u8 i;
    u8 mask;

    fam_ix = use_family[bg_w.stage];
    mask = 0x80;

    for (i = 0; i < 8; i++, assign = mask >>= 1) {
        if (fam_ix & mask) {
            scr_calc(i);
        }

        (void)assign;
    }

    (void)assign;
}

void Ending_Family_Move() {
    u8 mask_val = ending_use_family[end_w.type];
    u8 assign;
    u8 i;
    u8 mask = 0x80;

    for (i = 0; i < 8; i++, assign = mask >>= 1) {
        if (mask_val & mask) {
            scr_calc2(i);
        }
    }

    (void)assign;
    scr_calc(3);
}

void Bg_Disp_Switch(u8 on_off) {
    bg_disp_off = on_off;
}
