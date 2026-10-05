/**
 * @file eff06.c
 * Stage background objects
 */

#include "sf33rd/Source/Game/effect/eff06.h"
#include "common.h"
#include "port/config/config.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 scr_obj_num6[AREA_COUNT] = {
    [AREA_3S_GILL] = 1,   [AREA_3S_ALEX] = 6,      [AREA_3S_RYU] = 4,         [AREA_3S_YUN] = 0,
    [AREA_3S_DUDLEY] = 3, [AREA_3S_NECRO] = 1,     [AREA_3S_HUGO] = 0,        [AREA_3S_IBUKI] = 2,
    [AREA_3S_ELENA] = 8,  [AREA_3S_ORO] = 3,       [AREA_3S_YANG] = 0,        [AREA_3S_KEN] = 4,
    [AREA_3S_SEAN] = 3,   [AREA_3S_URIEN] = 0,     [AREA_3S_AKUMA] = 1,       [AREA_3S_SHIN_AKUMA] = 1,
    [AREA_3S_CHUNLI] = 1, [AREA_3S_MAKOTO] = 4,    [AREA_3S_Q] = 0,           [AREA_3S_TWELVE] = 1,
    [AREA_3S_REMY] = 1,   [AREA_3S_BONUS_CAR] = 0, [AREA_3S_BONUS_BALLS] = 0, [AREA_NG_GILL] = 10,
    [AREA_NG_ALEX] = 8,   [AREA_NG_RYU_A] = 4,
};

const s16 st0000_data_tbl[9] = {
    0, 2, 8492, 640, 48, 82, 0, 0, 0,
};

const s16 st0100_data_tbl[54] = {
    0, 1, 300,  648, 48, 86, 1,  0, 0, 0, 1, 300,  728, 48, 86, 2,  0, 0, 0, 1, 300,  792, 48, 86, 3,  0, 0,
    0, 2, 8492, 648, 16, 10, 18, 0, 0, 0, 2, 8492, 416, 48, 70, 20, 0, 0, 0, 2, 8492, 264, 48, 72, 21, 0, 0,
};

const s16 st0B00_data_tbl[36] = {
    0, 1, 300, 648, 48, 86, 1, 0, 0, 0, 1, 300, 728, 48,  86, 2,  0, 0,
    0, 1, 300, 792, 48, 86, 3, 0, 0, 0, 2, 300, 463, 193, 83, 22, 0, 0,
};

const s16 st0200_data_tbl[36] = {
    0, 6, 8492, 160, 64, 86, 0, 0, 0, 0, 6, 8492, 224, 64, 86, 1, 0, 0,
    0, 6, 8492, 336, 64, 86, 2, 0, 0, 0, 6, 8492, 432, 64, 86, 3, 0, 0,
};

const s16 st0A00_data_tbl[54] = {
    0, 3, 300, 448, 16, 88, 9,  0, 0, 0, 3, 300, 528, 16, 88, 11, 0, 0, 0, 3, 300, 608, 16, 88, 12, 0, 0,
    0, 3, 300, 688, 16, 88, 13, 0, 0, 0, 3, 300, 768, 16, 88, 14, 0, 0, 0, 3, 300, 848, 16, 88, 15, 0, 0,
};

const s16 st0400_data_tbl[27] = {
    0, 2, 8492, 672, 24, 10, 0, 0, 0, 0, 2, 300, 416, 72, 82, 1, 0, 0, 0, 2, 300, 192, 0, 10, 12, 0, 0,
};

const s16 st0500_data_tbl[63] = {
    0,   3, 8492, 432, 0, 82,  8,   0, 0,   0,   2, 300, 160, 0, 84,  0,   0, 0,   0,   2, 300,
    256, 0, 84,   1,   0, 0,   0,   2, 300, 352, 0, 84,  2,   0, 0,   0,   2, 300, 672, 0, 84,
    5,   0, 0,    0,   2, 300, 768, 0, 84,  6,   0, 0,   0,   2, 300, 864, 0, 84,  7,   0, 0,
};

const s16 st1300_data_tbl[63] = {
    0,   3, 8492, 432, 0, 82,  20,  0, 0,   0,   2, 300, 160, 0, 84,  12,  0, 0,   0,   2, 300,
    256, 0, 84,   13,  0, 0,   0,   2, 300, 352, 0, 84,  14,  0, 0,   0,   2, 300, 672, 0, 84,
    17,  0, 0,    0,   2, 300, 768, 0, 84,  18,  0, 0,   0,   2, 300, 864, 0, 84,  19,  0, 0,
};

const s16 st0700_data_tbl[18] = {
    0, 6, 8492, 288, 80, 93, 0, 0, 0, 0, 6, 8492, 416, 48, 93, 1, 0, 0,
};

const s16 st0800_data_tbl[99] = {
    0, 7, 8492, 256, 43,  88, 10, 0, 0, 0, 7, 8492, 384, 43,  88, 11, 0, 0, 0, 7, 8492, 752, 43, 88, 16, 0, 0,
    0, 2, 8492, 192, 48,  86, 0,  0, 0, 0, 2, 8492, 576, 48,  86, 2,  0, 0, 0, 2, 8492, 800, 48, 86, 3,  0, 0,
    0, 6, 8492, 640, 128, 85, 6,  0, 0, 0, 6, 8492, 368, 160, 85, 7,  0, 0, 0, 2, 8492, 336, 88, 86, 6,  0, 0,
    0, 6, 8492, 514, 136, 86, 8,  0, 0, 0, 6, 8492, 722, 56,  86, 9,  0, 0,
};

const s16 st0900_data_tbl[27] = {
    0, 3, 8492, 464, 64, 86, 6, 0, 0, 0, 7, 8492, 512, 96, 88, 7, 0, 0, 0, 6, 8492, 832, 16, 10, 5, 0, 0,
};

const s16 st0c00_data_tbl[27] = {
    0, 3, 8492, 464, 64, 86, 0, 0, 0, 0, 7, 8492, 512, 96, 88, 1, 0, 0, 0, 6, 8492, 832, 16, 10, 5, 0, 0,
};

const s16 st0e00_data_tbl[9] = {
    0, 2, 8492, 592, 16, 10, 0, 0, 0,
};

const s16 st1000_data_tbl[18] = {
    0, 2, 8492, 448, 16, 14, 1, 0, 0, 0, 2, 8492, 928, 32, 72, 3, 0, 0,
};

const s16 st1100_data_tbl[36] = {
    0, 1, 8492, 256, 64,  90, 0, 0, 0, 0, 1, 8492, 384, 128, 90, 1, 0, 0,
    0, 1, 8492, 624, 128, 90, 4, 0, 0, 0, 3, 8492, 784, 128, 10, 7, 0, 0,
};

const s16 st1400_data_tbl[9] = {
    0, 3, 300, 496, 64, 88, 4, 0, 0,
};

/// New Generation's Gill stage. Colour codes are converted from palette row 0x40 to the PS2 BG palette row 0x12C, both
/// with the 0x2000 flag (0x2040 -> 8492) and without it (0x40 -> 300).
const s16 ng_st0000_data_tbl[90] = {
    1,  2,   8492, 182, 80, 80,   8,    1,   0,  1,  2,    300, 784, 80, 82,   2,   0,   0,   0,  2,   8492, 304, 80,
    80, 0,   1,    0,   0,  2,    8492, 592, 80, 80, 1,    0,   0,   0,  2,    300, 704, 256, 82, 3,   0,    0,   1,
    2,  300, 224,  80,  81, 5,    0,    0,   1,  2,  8492, 368, 288, 81, 6,    0,   0,   1,   2,  300, 190,  272, 82,
    7,  0,   0,    0,   2,  8492, 256,  64,  83, 21, 0,    0,   0,   2,  8492, 768, 64,  83,  24, 0,   0,
};

/// New Generation's Alex stage. Colour codes are converted from palette rows 0x40 and 0x5C to the PS2 BG palette rows
/// 0x12C and 0x148.
const s16 ng_st0100_data_tbl[72] = {
    1, 3, 8492, 674,  113,  104, 2,  0,  0, 1, 2, 8520, 895,  32,   20,  33, 1,  0, 1, 2, 8492, 784,  64,  83, 0,  0,
    0, 0, 2,    8492, 368,  16,  22, 4,  0, 0, 0, 2,    8492, 688,  16,  22, 5,  0, 0, 0, 2,    8492, 864, 16, 22, 6,
    0, 0, 0,    2,    8492, 272, 56, 83, 7, 0, 0, 1,    1,    8492, 752, 86, 90, 8, 0, 0,
};

/// First area of New Generation's Ryu stage. Colour codes are converted from palette rows 0x40 and 0x63 to the PS2 BG
/// palette rows 0x12C and 0x14F, both with the 0x2000 flag and without it. NG syncs these objects with the line scroll
/// of layer 1 (sync_suzi 1), which we don't have.
const s16 ng_st0200_data_tbl[36] = {
    1, 2, 8492, 303,  122, 76, 10, 1, 0, 1, 2, 8492, 239, 192, 77, 13, 1, 0,
    1, 2, 8527, -288, 64,  74, 20, 1, 0, 1, 2, 335,  256, 48,  74, 21, 1, 0,
};

const s16* scr_obj_data6[AREA_COUNT] = {
    [AREA_3S_GILL] = st0000_data_tbl,
    [AREA_3S_ALEX] = st0100_data_tbl,
    [AREA_3S_RYU] = st0200_data_tbl,
    [AREA_3S_YUN] = stg_dum_data_tbl,
    [AREA_3S_DUDLEY] = st0400_data_tbl,
    [AREA_3S_NECRO] = st0500_data_tbl,
    [AREA_3S_HUGO] = stg_dum_data_tbl,
    [AREA_3S_IBUKI] = st0700_data_tbl,
    [AREA_3S_ELENA] = st0800_data_tbl,
    [AREA_3S_ORO] = st0900_data_tbl,
    [AREA_3S_YANG] = st0A00_data_tbl,
    [AREA_3S_KEN] = st0B00_data_tbl,
    [AREA_3S_SEAN] = st0c00_data_tbl,
    [AREA_3S_URIEN] = st0100_data_tbl,
    [AREA_3S_AKUMA] = st0e00_data_tbl,
    [AREA_3S_SHIN_AKUMA] = st0e00_data_tbl,
    [AREA_3S_CHUNLI] = st1000_data_tbl,
    [AREA_3S_MAKOTO] = st1100_data_tbl,
    [AREA_3S_Q] = st0500_data_tbl,
    [AREA_3S_TWELVE] = st1300_data_tbl,
    [AREA_3S_REMY] = st1400_data_tbl,
    [AREA_3S_BONUS_CAR] = stg_dum_data_tbl,
    [AREA_3S_BONUS_BALLS] = stg_dum_data_tbl,
    [AREA_NG_GILL] = ng_st0000_data_tbl,
    [AREA_NG_ALEX] = ng_st0100_data_tbl,
    [AREA_NG_RYU_A] = ng_st0200_data_tbl,
};

void effect_06_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (ewk->wu.hit_stop && !EXE_flag && !Game_pause && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_06_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt = scr_obj_num6[bg_w.bg_index];
    s16 i;
    const s16* data_ptr;

    if (lp_cnt == 0) {
        return 0;
    }

    if (Config_GetBool(CFG_DRAW_PLAYERS_ABOVE_HUD)) {
        switch (bg_w.bg_index) {
        case AREA_3S_MAKOTO:
            lp_cnt -= 1; // Remove large tree on the right
            break;
        }
    }

    data_ptr = scr_obj_data6[bg_w.bg_index];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 6;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        ewk->wu.type = i;
        ewk->wu.dead_f = *data_ptr++;
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.hit_stop = *data_ptr++; // Animate every frame
        ewk->wu.sync_suzi = *data_ptr++;
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
