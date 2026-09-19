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
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_state.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

Vertex scrDrawPos[4];
ColoredVertex bgpoly[4];
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

BG bg_w;
RW_DATA rw_dat[20];

typedef struct {
    s32 x;
    s32 y;
    s32 xs;
    s32 ys;
} ChipRect;

typedef struct {
    s32 bgnum;
    s32 gixbase;
    s32* xx;
    s32* yy;
    s32 unused;
    s32 ofsPal;
    PPGDataList* curDataList;
} ScreenDraw;

typedef struct {
    u8 bgnm;
    s32* xx;
    s32* yy;
    s32 global_index;
    s32 pal_offset;
    PPGDataList* data_list;
} StageDrawContext;

static void bgRWWorkUpdate();
static void select_bg_list_for_reindexed_chip(s32 global_index_real);
static s32 remap_stage03_player_chip(s32 global_index_real);
static void advance_stage03_flash_state();
static void advance_stage03_player_rw_state();
static s32 remap_stage19_default_chip(s32 global_index_real);
static void advance_stage19_flash_state();
static void advance_stage19_loop_state();
static bool is_exe_or_pause_active();
static bool should_update_rw_work(u8 bgnm);
static s32 remap_ending_nosekae_chip(s32 global_index_real);
static void bgDrawOneScreen(const ScreenDraw* screen);
static void bgDrawOneChip(const ChipRect* rect, s32 gbix, u32 vtxCol, s32 ofsPal);
static void bgAkebonoDraw();
static void ppgCalScrPosition(s32 x, s32 y, s32 xs, s32 ys);

static void select_bg_list_for_reindexed_chip(s32 global_index_real) {
    if (ppgCheckTextureNumber(0, global_index_real) == 0) {
        if (ppgCheckTextureNumber(&ppgRwBgTex, global_index_real)) {
            ppgSetupCurrentDataList(&ppgRwBgList);
        } else {
            ppgSetupCurrentDataList(&ppgAkeList);
        }
    }
}

static s32 remap_stage03_player_chip(s32 global_index_real) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (global_index_real == rw_dat[i + 1].rwgbix) {
            global_index_real = rw_dat[i + 1].gbix;
            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }
            break;
        }
    }

    return global_index_real;
}

static void advance_stage03_flash_state() {
    rw_dat[0].rw_cnt--;

    if (rw_dat[0].rw_cnt != 0) {
        return;
    }

    if (stage_flash == 0) {
        rw_dat[0].rwd_ptr += 14;
        rw3col_ptr++;
        rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

        if (rw_dat[0].rw_cnt != -1) {
            return;
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

        return;
    }

    rw_dat[0].rwd_ptr += 14;
    rw3col_ptr++;
    rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

    if (rw_dat[0].rw_cnt != -1) {
        return;
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
}

static void reload_rw_slot(RW_DATA* slot) {
    if (slot->rwd_ptr[0] == -1) {
        slot->rwd_ptr = slot->brw_ptr;
        slot->rw_cnt = *slot->rwd_ptr++;
        slot->gbix = *slot->rwd_ptr++;
    } else {
        slot->rw_cnt = *slot->rwd_ptr++;
        slot->gbix = *slot->rwd_ptr++;
    }
}

static void advance_stage03_player_rw_state() {
    s32 i;

    for (i = 0; i < 4; i = i + 1) {
        rw_dat[i + 1].rw_cnt--;

        if (rw_dat[i + 1].rw_cnt == 0) {
            reload_rw_slot(&rw_dat[i + 1]);
        }
    }
}

static s32 remap_stage19_default_chip(s32 global_index_real) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (global_index_real == rw_gbix[i]) {
            global_index_real = *(rw_dat[0].rwd_ptr + i + 1);

            if (!ppgCheckTextureNumber(0, global_index_real)) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }

            break;
        }
    }

    return global_index_real;
}

static void select_stage19_flash_sequence() {
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
}

static void advance_stage19_flash_state() {
    rw_dat[0].rw_cnt--;

    if (rw_dat[0].rw_cnt != 0) {
        return;
    }

    rw_dat[0].rwd_ptr += 5;
    rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];

    if (rw_dat[0].rw_cnt != -1) {
        return;
    }

    stage_ftimer--;

    if (stage_ftimer != 0) {
        rw_dat[0].rwd_ptr = rw_dat[0].brw_ptr;
        rw_dat[0].rw_cnt = rw_dat[0].rwd_ptr[0];
        return;
    }

    stage_flash = random_16_bg();
    stage_ftimer = random_16_bg();
    select_stage19_flash_sequence();
}

static void advance_stage19_loop_state() {
    rw_dat[1].rw_cnt--;

    if (rw_dat[1].rw_cnt != 0) {
        return;
    }

    reload_rw_slot(&rw_dat[1]);
}

static s32 remap_ending_c_chip_in_range(s32 global_index_real, s32 first, s32 limit) {
    s32 i;

    for (i = first; i < limit; i++) {
        if (global_index_real == rw_dat[i].rwgbix) {
            global_index_real = rw_dat[i].rwd_ptr[c_number];

            if (!ppgCheckTextureNumber(0, global_index_real)) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }

            break;
        }
    }

    return global_index_real;
}

static bool is_exe_or_pause_active() {
    return (EXE_flag != 0 || Game_pause != 0);
}

static bool should_update_rw_work(u8 bgnm) {
    return (EXE_flag == 0 && Game_pause == 0 && rw_bg_flag[bgnm] && rw_num);
}

static s32 remap_ending_nosekae_chip(s32 global_index_real) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (gouki_end_gbix[i] == global_index_real) {
            global_index_real = gouki_end_nosekae[nosekae - 1][i];
            select_bg_list_for_reindexed_chip(global_index_real);
            break;
        }
    }

    return global_index_real;
}

static s32 remap_ending_g_chip_in_range(s32 global_index_real, s32 first, s32 limit, u8 column) {
    s32 i;

    for (i = first; i < limit; i++) {
        if (global_index_real == rw_dat[i].rwgbix) {
            global_index_real = rw_dat[i].rwd_ptr[column];
            select_bg_list_for_reindexed_chip(global_index_real);
            break;
        }
    }

    return global_index_real;
}

static void draw_chip_and_restore_list(const StageDrawContext* context, const ChipRect* rect, s32 gbix, u32 vtxCol) {
    bgDrawOneChip(rect, gbix, vtxCol, context->pal_offset);
    ppgSetupCurrentDataList(context->data_list);
}

static s32 remap_stage03_background_chip(s32 global_index_real, u32* vtxColor) {
    s32 i;

    for (i = 0; i < 13; i++) {
        if (global_index_real == rw_gbix[i]) {
            global_index_real = *(rw_dat[0].rwd_ptr + i + 1);
            *vtxColor = *rw3col_ptr;

            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }
            break;
        }
    }

    return global_index_real;
}

static void draw_stage03_tiles(const StageDrawContext* context) {
    s32 x;
    s32 y;
    s32 global_index_real;
    u32 vtxColor;

    for (y = context->yy[0]; y < context->yy[1]; y += 128) {
        for (x = context->xx[0]; x < context->xx[1]; x += 128) {
            global_index_real = context->global_index + (((y >> 7) << 3) + (x >> 7));
            vtxColor = 0xFFFFFFFF;

            if (context->bgnm == 0) {
                global_index_real = remap_stage03_player_chip(global_index_real);
            } else {
                global_index_real = remap_stage03_background_chip(global_index_real, &vtxColor);
            }

            draw_chip_and_restore_list(context, &(ChipRect){ x, y, 128, 128 }, global_index_real, vtxColor);
        }
    }
}

static void draw_stage02_tiles(const StageDrawContext* context, u32 vtxColor) {
    s32 x;
    s32 y;
    s32 global_index_real;

    for (y = context->yy[0]; y < context->yy[1]; y += 128) {
        for (x = context->xx[0]; x < context->xx[1]; x += 128) {
            global_index_real = context->global_index + (((y >> 7) << 3) + (x >> 7));

            if (context->bgnm == 1) {
                global_index_real += yang_ix_plus;
            }

            if (ppgCheckTextureNumber(0, global_index_real) == 0) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }
            draw_chip_and_restore_list(context, &(ChipRect){ x, y, 128, 128 }, global_index_real, vtxColor);
        }
    }
}

static void draw_stage04_suzi(u8 bgnm) {
    s32 x;
    s32 suzi_pos;

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
}

static void prepare_stage_tile_bounds(u8 bgnm, s32 xx[2], s32 yy[2]) {
    Vec3 point[2];
    s32 x;

    njUnitMatrix(0);
    njScale(0, 1.0f, -1.0f, 1.0f);
    njTranslate(0, 0.0f, -1024.0f, 0.0f);
    njTranslate(0, (s16)bg_prm[bgnm].bg_h_shift, (s16)bg_prm[bgnm].bg_v_shift, 0.0f);
    njScale(0, 1.0f, -1.0f, 1.0f);
    njTranslate(0, 0.0f, -224.0f, 0.0f);
    njScale(0, 1.0f / scr_sc, 1.0f / scr_sc, 1.0f);
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
    njScale(0, scr_sc, scr_sc, 1.0);
    njTranslate(0, 0, 224.0, 0);
    njScale(0, 1.0, -1.0, 1.0);
    njTranslate(0, (s16)-bg_prm[bgnm].bg_h_shift, (s16)-bg_prm[bgnm].bg_v_shift, 0);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
    njTranslate(0, 0, 1024.0, PrioBase[bg_priority[bgnm]]);
    njScale(0, 1.0, -1.0, 1.0);
}

static s32 advance_stage02_state(u8 bgnm) {
    if (is_exe_or_pause_active()) {
        return 1;
    }

    if (bgnm != 1) {
        return 0;
    }

    yang_timer--;

    if (yang_timer != 0) {
        return 0;
    }

    yang_timer = 4;
    yang_ix++;

    if (yang_ix == 4) {
        yang_ix = 0;
    }

    yang_ix_plus = yang_ix << 5;
    return 0;
}

static s32 advance_stage19_state(u8 bgnm) {
    if (is_exe_or_pause_active()) {
        return 1;
    }

    if (bgnm != 1) {
        return 0;
    }

    advance_stage19_flash_state();
    advance_stage19_loop_state();
    return 0;
}

static s32 advance_stage03_state(u8 bgnm) {
    if (is_exe_or_pause_active()) {
        return 1;
    }

    if (bgnm == 0) {
        advance_stage03_player_rw_state();
        return 0;
    }

    advance_stage03_flash_state();
    return 0;
}

static s32 remap_stage19_chip(u8 bgnm, s32 global_index_real) {
    if (bgnm == 1) {
        if (rw_dat[1].rwgbix == global_index_real) {
            global_index_real = rw_dat[1].gbix;

            if (!ppgCheckTextureNumber(0, global_index_real)) {
                ppgSetupCurrentDataList(&ppgRwBgList);
            }
        } else {
            global_index_real = remap_stage19_default_chip(global_index_real);
        }
    }

    return global_index_real;
}

static void draw_remapped_tiles(const StageDrawContext* context, s32 (*remap)(u8, s32)) {
    s32 x;
    s32 y;
    s32 global_index_real;

    for (y = context->yy[0]; y < context->yy[1]; y += 128) {
        for (x = context->xx[0]; x < context->xx[1]; x += 128) {
            global_index_real = context->global_index + (((y >> 7) << 3) + (x >> 7));
            global_index_real = remap(context->bgnm, global_index_real);

            draw_chip_and_restore_list(context, &(ChipRect){ x, y, 128, 128 }, global_index_real, -1);
        }
    }
}

static void draw_stage19_tiles(const StageDrawContext* context) {
    draw_remapped_tiles(context, remap_stage19_chip);
}

static s32 remap_ending_g_chip(u8 bgnm, s32 global_index_real) {
    if (nosekae != 0) {
        global_index_real = remap_ending_nosekae_chip(global_index_real);
    }

    if (bgnm == 0) {
        if (g_kakikae[0]) {
            global_index_real = remap_ending_g_chip_in_range(global_index_real, 0, 12, g_number[0]);
        }

        if (g_kakikae[1]) {
            global_index_real = remap_ending_g_chip_in_range(global_index_real, 12, 20, g_number[1]);
        }
    }

    return global_index_real;
}

static void draw_ending_g_tiles(const StageDrawContext* context) {
    draw_remapped_tiles(context, remap_ending_g_chip);
}

static s32 remap_ending_c_chip(u8 bgnm, s32 global_index_real) {
    if (bgnm == 0) {
        switch (c_kakikae) {
        case 1:
            global_index_real = remap_ending_c_chip_in_range(global_index_real, 0, 8);
            break;

        case 2:
            global_index_real = remap_ending_c_chip_in_range(global_index_real, 8, 16);
        }
    }

    return global_index_real;
}

static void draw_ending_c_tiles(const StageDrawContext* context) {
    draw_remapped_tiles(context, remap_ending_c_chip);
}

static void draw_ending_stage7(const StageDrawContext* context) {
    bgDrawOneScreen(&(ScreenDraw){ context->bgnm, context->global_index, &context->xx[0], &context->yy[0], -1,
                                   context->pal_offset, context->data_list });

    if (EXE_flag != 0) {
        return;
    }

    if (Game_pause != 0) {
        return;
    }

    if (rw_bg_flag[context->bgnm] && rw_num) {
        bgRWWorkUpdate();
    }

    scr_calc2(context->bgnm);
}

static void draw_later_special_stage(const StageDrawContext* context) {
    switch (tokusyu_stage) {
    case 5:
        draw_ending_g_tiles(context);

        scr_calc2(context->bgnm);
        break;

    case 6:
        draw_ending_c_tiles(context);

        scr_calc2(context->bgnm);
        break;

    case 7:
        draw_ending_stage7(context);
        break;

    case 4:
        draw_stage04_suzi(context->bgnm);

        /* fallthrough */

    default:
        bgDrawOneScreen(&(ScreenDraw){ context->bgnm, context->global_index, &context->xx[0], &context->yy[0], -1,
                                       context->pal_offset, context->data_list });

        if (should_update_rw_work(context->bgnm)) {
            bgRWWorkUpdate();
        }

        break;
    }
}

static s32 draw_and_advance_judgment_stage(const StageDrawContext* context) {
    u32 vtxColor;

    if (judge_flag == 1 && context->bgnm == 1) {
        vtxColor = 0xFFA0A0A0;
    } else {
        vtxColor = 0xFFFFFFFF;
    }

    draw_stage02_tiles(context, vtxColor);
    return advance_stage02_state(context->bgnm);
}

static s32 draw_early_special_stage(const StageDrawContext* context) {
    switch (tokusyu_stage) {
    case 1:
        draw_stage03_tiles(context);

        if (advance_stage03_state(context->bgnm)) {
            return 1;
        }
        break;

    case 2:
        if (draw_and_advance_judgment_stage(context)) {
            return 1;
        }
        break;

    case 3:
        draw_stage19_tiles(context);

        if (advance_stage19_state(context->bgnm)) {
            return 1;
        }
        break;

    default:
        draw_later_special_stage(context);
        break;
    }

    return 0;
}

void scr_trans(u8 bgnm) {
    PPGDataList* curDataList;
    StageDrawContext draw_context;
    s32 xx[2];
    s32 yy[2];
    s32 global_index;
    s32 palOffset;

    prepare_stage_tile_bounds(bgnm, xx, yy);

    palOffset = bgPalCodeOffset[bgnm];

    if (ending_flag == 0 && bgnm == 3) {
        ppgSetupCurrentDataList(&ppgAkeList);
        bgAkebonoDraw();
        return;
    }

    global_index = (bgnm * 64) + 100;
    ppgSetupCurrentDataList(&ppgBgList[bgnm]);
    curDataList = &ppgBgList[bgnm];

    draw_context.bgnm = bgnm;
    draw_context.xx = xx;
    draw_context.yy = yy;
    draw_context.global_index = global_index;
    draw_context.pal_offset = palOffset;
    draw_context.data_list = curDataList;

    if (draw_early_special_stage(&draw_context)) {
        return;
    }
}

void bgRWWorkUpdate() {
    s32 i;

    for (i = 0; i < rw_num; i++) {
        rw_dat[i].rw_cnt--;

        if (rw_dat[i].rw_cnt == 0) {
            reload_rw_slot(&rw_dat[i]);
        }
    }
}

static s32 remap_screen_chip(s32 bgnum, s32 gbix) {
    s32 i;

    if (!(rw_bg_flag[bgnum] && rw_num)) {
        return gbix;
    }

    for (i = 0; i < rw_num; i++) {
        if (bgnum != rw_dat[i].bg_num || gbix != rw_dat[i].rwgbix) {
            continue;
        }

        gbix = rw_dat[i].gbix;
        if (!(ppgCheckTextureNumber(0, gbix))) {
            ppgSetupCurrentDataList(&ppgRwBgList);
        }
        break;
    }

    return gbix;
}

void bgDrawOneScreen(const ScreenDraw* screen) {
    s32 x, y, gbix;

    for (y = screen->yy[0]; y < screen->yy[1]; y += 128) {
        for (x = screen->xx[0]; x < screen->xx[1]; x += 128) {
            gbix = ((y >> 7) << 3) + (x >> 7) + screen->gixbase;

            gbix = remap_screen_chip(screen->bgnum, gbix);

            bgDrawOneChip(&(ChipRect){ x, y, 128, 128 }, gbix, -1, screen->ofsPal);
            ppgSetupCurrentDataList(screen->curDataList);
        }
    }
}

static bool is_bg_chip_outside_screen() {
    return (scrDrawPos->x >= 384.0f) || (scrDrawPos[3].x < 0.0f) || (scrDrawPos->y >= 224.0f) ||
           (scrDrawPos[3].y < 0.0f);
}

void bgDrawOneChip(const ChipRect* rect, s32 gbix, u32 vtxCol, s32 ofsPal) {
    if ((No_Trans == 0) && ppgCheckTextureNumber(0, gbix)) {
        ppgCalScrPosition(rect->x, rect->y, rect->xs, rect->ys);

        if (is_bg_chip_outside_screen()) {
            return;
        }

        ppgWriteQuadUseTrans(scrDrawPos, &(PPGQuadTransArgs){vtxCol, 0, gbix, 0, 0, ofsPal});
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
        ppgWriteQuadUseTrans(scrDrawPos, &(PPGQuadTransArgs){0xFFFFFFFF, NULL, i, i, 0, 0});
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
    njScale(NULL, scr_sc, scr_sc, 1.0f);
    njTranslate(NULL, 0.0f, 224.0f, 0.0f);
    njScale(NULL, 1.0f, -1.0f, 1.0f);
    njTranslate(NULL, (s16)-bg_prm[bgnm].bg_h_shift, (s16)-bg_prm[bgnm].bg_v_shift, 0.0f);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
}

void scr_calc2(u8 bgnm) {
    njUnitMatrix(NULL);
    njScale(NULL, scr_sc, scr_sc, 1.0f);
    njTranslate(NULL, 0.0f, 224.0f, 0.0f);
    njScale(NULL, 1.0f, -1.0f, 1.0f);
    njTranslate(NULL, (s16)-end_prm[bgnm + 1].bg_h_shift, (s16)-end_prm[bgnm + 1].bg_v_shift, 0.0f);
    njGetMatrix(&BgMATRIX[bgnm + 1]);
}

void Pause_Family_On() {
    njUnitMatrix(0);
    njTranslate(0, 0, 224, 0);
    njScale(0, 1, -1, 1);
    njGetMatrix(&BgMATRIX[8]);
}

void Zoomf_Init() {
    zoom_add = 64;
    scr_sc = 1.0f;
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

static s32 zoom_adgjust_offset(u16 pos) {
    u16 buff;

    if (zoom_add >= 0x40) {
        buff = zoom_add;
        buff -= 0x40;
        buff *= pos;
        buff >>= 6;
        buff &= 0x1FF;
        return -buff;
    } else {
        buff = 0x40;
        buff -= zoom_add;
        buff *= pos;
        buff >>= 6;
        buff &= 0x1FF;
        return buff;
    }
}

static void adjust_frame_x(u16 pos_x) {
    scrn_adgjust_x = zoom_adgjust_offset(pos_x);
}

static void fix_frame_y_adjustment() {
    if (scrn_adgjust_y == -0x14) {
        scrn_adgjust_y += 1;
    }
}

static void adjust_frame_y(u16 pos_y) {
    scrn_adgjust_y = zoom_adgjust_offset(pos_y + 0x15);

    fix_frame_y_adjustment();
}

void Frame_Adgjust(u16 pos_x, u16 pos_y) {
    adjust_frame_x(pos_x);
    adjust_frame_y(pos_y);
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
