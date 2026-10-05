/**
 * @file eff05.c
 * Stage background objects
 */

#include "sf33rd/Source/Game/effect/eff05.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 scr_obj_num[AREA_COUNT] = {
    [AREA_3S_GILL] = 0,   [AREA_3S_ALEX] = 2,      [AREA_3S_RYU] = 0,         [AREA_3S_YUN] = 0,
    [AREA_3S_DUDLEY] = 0, [AREA_3S_NECRO] = 2,     [AREA_3S_HUGO] = 1,        [AREA_3S_IBUKI] = 0,
    [AREA_3S_ELENA] = 3,  [AREA_3S_ORO] = 1,       [AREA_3S_YANG] = 0,        [AREA_3S_KEN] = 1,
    [AREA_3S_SEAN] = 1,   [AREA_3S_URIEN] = 1,     [AREA_3S_AKUMA] = 1,       [AREA_3S_SHIN_AKUMA] = 1,
    [AREA_3S_CHUNLI] = 2, [AREA_3S_MAKOTO] = 2,    [AREA_3S_Q] = 0,           [AREA_3S_TWELVE] = 2,
    [AREA_3S_REMY] = 4,   [AREA_3S_BONUS_CAR] = 1, [AREA_3S_BONUS_BALLS] = 4, [AREA_NG_GILL] = 5,
    [AREA_NG_ALEX] = 4,   [AREA_NG_RYU_A] = 1,
};

const s16 stg_dum_data_tbl[1] = { 0 };

const s16 stg0100_data_tbl[18] = {
    0, 1, 300, 568, 48, 86, 0, 0, 0, 0, 2, 8492, 528, 47, 68, 19, 0, 0,
};

const s16 stg0A00_data_tbl[9] = {
    0, 3, 300, 512, 16, 88, 10, 0, 0,
};

const s16 stg0500_data_tbl[36] = {
    0, 3, 300, 512, 0, 88, 9, 0, 0, 0, 3, 8492, 560, 0, 88, 10, 0, 0,
    0, 2, 300, 512, 0, 84, 3, 0, 0, 0, 2, 300,  512, 0, 84, 4,  0, 0,
};

const s16 stg1300_data_tbl[36] = {
    0, 3, 300, 512, 0, 88, 21, 0, 0, 0, 3, 8492, 560, 0, 88, 22, 0, 0,
    0, 2, 300, 512, 0, 84, 15, 0, 0, 0, 2, 300,  512, 0, 84, 16, 0, 0,
};

const s16 stg0600_data_tbl[9] = {
    0, 1, 8492, 511, 96, 88, 11, 0, 0,
};

const s16 stg0700_data_tbl[36] = {
    0, 3, 8492, 504, 48,  94, 4, 0, 0, 0, 3, 8492, 504, 112, 94, 8, 0, 0,
    0, 2, 8492, 464, 256, 84, 7, 0, 0, 0, 2, 8492, 512, 352, 84, 8, 0, 0,
};

const s16 stg0800_data_tbl[54] = {
    0, 2, 8492, 496, 48,  85, 1, 0, 0, 0, 7, 8492, 512, 43, 88, 12, 0, 0, 0, 7, 8492, 512, 43, 88, 13, 0, 0,
    0, 2, 8492, 624, 120, 86, 7, 0, 0, 0, 3, 8492, 512, 72, 90, 1,  0, 0, 0, 3, 8492, 512, 72, 90, 2,  0, 0,
};

const s16 stg0900_data_tbl[9] = {
    0, 6, 8492, 352, 16, 10, 5, 0, 0,
};

const s16 stg0c00_data_tbl[9] = {
    0, 6, 8492, 352, 16, 10, 5, 0, 0,
};

const s16 stg0d00_data_tbl[9] = {
    0, 3, 300, 512, 32, 88, 0, 0, 0,
};

const s16 stg0e00_data_tbl[27] = {
    0, 3, 300, 512, 88, 98, 7, 0, 0, 0, 2, 300, 576, 64, 84, 5, 0, 0, 0, 2, 300, 560, 208, 84, 6, 0, 0,
};

const s16 stg1000_data_tbl[18] = {
    0, 3, 8492, 512, 80, 86, 0, 0, 0, 0, 2, 8492, 480, 16, 12, 2, 0, 0,
};

const s16 stg1100_data_tbl[18] = {
    0, 1, 8492, 512, 176, 90, 2, 0, 0, 0, 1, 8492, 512, 240, 90, 3, 0, 0,
};

const s16 stg1400_data_tbl[36] = {
    0, 2, 8492, 504, 11,  10, 0, 0, 0, 0, 1, 8492, 496, 64,  86, 1, 0, 0,
    0, 3, 300,  496, 144, 88, 3, 0, 0, 0, 3, 300,  512, 320, 83, 5, 0, 0,
};

const s16 stg1500_data_tbl[9] = {
    0, 2, 300, 445, 48, 10, 0, 0, 0,
};

const s16 stg1600_data_tbl[36] = {
    0, 2, 300, 624, 0, 10, 2, 0, 0, 0, 2, 8492, 511, 0,  12, 3,  0, 0,
    0, 2, 300, 511, 0, 80, 4, 0, 0, 0, 2, 300,  608, 48, 77, 11, 0, 0,
};

/// New Generation's Gill stage. Colour codes are converted from palette row 0x40 to the PS2 BG palette row 0x12C.
const s16 ng_stg0000_data_tbl[45] = {
    0,  2, 8492, 416, 96, 82, 10,   0,   0,  0,  2,  8492, 528, 79, 82, 9,    0,   0,  0,  2,  8492, 480, 256,
    92, 4, 0,    0,   0,  2,  8492, 512, 64, 83, 22, 0,    0,   0,  2,  8492, 512, 64, 83, 23, 0,    0,
};

/// New Generation's Alex stage. Colour codes are converted from palette row 0x40 to the PS2 BG palette row 0x12C.
const s16 ng_stg0100_data_tbl[36] = {
    0, 2, 8492, 608, 40,  82, 1, 0, 0, 0, 2, 8492, 512, 16, 22, 3,  0, 0,
    1, 2, 8492, 512, 318, 83, 9, 1, 0, 0, 2, 8492, 512, 64, 80, 13, 0, 0,
};

/// First area of New Generation's Ryu stage. Colour codes are converted from palette row 0x40 to the PS2 BG palette row
/// 0x12C. NG syncs the object with the line scroll of layer 1 (sync_suzi 1), which we don't have.
const s16 ng_stg0200_data_tbl[9] = {
    0, 2, 8492, 16, 19, 20, 11, 0, 0,
};

u32* char_add[AREA_COUNT] = {
    [AREA_3S_GILL] = _fnl_char_table,        [AREA_3S_ALEX] = _usa_char_table,
    [AREA_3S_RYU] = _j10_char_table,         [AREA_3S_YUN] = _hkg_char_table,
    [AREA_3S_DUDLEY] = _eng_char_table,      [AREA_3S_NECRO] = _rca_char_table,
    [AREA_3S_HUGO] = _grm_char_table,        [AREA_3S_IBUKI] = _j11_char_table,
    [AREA_3S_ELENA] = _afc_char_table,       [AREA_3S_ORO] = _brz_char_table,
    [AREA_3S_YANG] = _hkg_char_table,        [AREA_3S_KEN] = _usa_char_table,
    [AREA_3S_SEAN] = _brz_char_table,        [AREA_3S_URIEN] = _orm_char_table,
    [AREA_3S_AKUMA] = _jp2_char_table,       [AREA_3S_SHIN_AKUMA] = _jp2_char_table,
    [AREA_3S_CHUNLI] = _chn_char_table,      [AREA_3S_MAKOTO] = _jp3_char_table,
    [AREA_3S_Q] = _usa_char_table,           [AREA_3S_TWELVE] = _rca_char_table,
    [AREA_3S_REMY] = _frc_char_table,        [AREA_3S_BONUS_CAR] = _bns_char_table,
    [AREA_3S_BONUS_BALLS] = _bns_char_table, [AREA_NG_GILL] = _ng_fnl_char_table,
    [AREA_NG_ALEX] = _ng_usa_char_table,     [AREA_NG_RYU_A] = _ng_j10_a_char_table,
};

const s16* scr_obj_data[AREA_COUNT] = {
    [AREA_3S_GILL] = stg_dum_data_tbl,        [AREA_3S_ALEX] = stg0100_data_tbl,
    [AREA_3S_RYU] = stg_dum_data_tbl,         [AREA_3S_YUN] = stg_dum_data_tbl,
    [AREA_3S_DUDLEY] = stg_dum_data_tbl,      [AREA_3S_NECRO] = stg0500_data_tbl,
    [AREA_3S_HUGO] = stg0600_data_tbl,        [AREA_3S_IBUKI] = stg0700_data_tbl,
    [AREA_3S_ELENA] = stg0800_data_tbl,       [AREA_3S_ORO] = stg0900_data_tbl,
    [AREA_3S_YANG] = stg0A00_data_tbl,        [AREA_3S_KEN] = stg0100_data_tbl,
    [AREA_3S_SEAN] = stg0c00_data_tbl,        [AREA_3S_URIEN] = stg0d00_data_tbl,
    [AREA_3S_AKUMA] = stg0e00_data_tbl,       [AREA_3S_SHIN_AKUMA] = stg0e00_data_tbl,
    [AREA_3S_CHUNLI] = stg1000_data_tbl,      [AREA_3S_MAKOTO] = stg1100_data_tbl,
    [AREA_3S_Q] = stg_dum_data_tbl,           [AREA_3S_TWELVE] = stg1300_data_tbl,
    [AREA_3S_REMY] = stg1400_data_tbl,        [AREA_3S_BONUS_CAR] = stg1500_data_tbl,
    [AREA_3S_BONUS_BALLS] = stg1600_data_tbl, [AREA_NG_GILL] = ng_stg0000_data_tbl,
    [AREA_NG_ALEX] = ng_stg0100_data_tbl,     [AREA_NG_RYU_A] = ng_stg0200_data_tbl,
};

void effect_05_move(WORK_Other* ewk) {
    if (obr_no_disp_check() == 0) {
        switch (ewk->wu.routine_no[0]) {
        case 0:
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
            break;

        case 1:
            if (compel_dead_check(ewk) != 0) {
                ewk->wu.routine_no[0]++;
                break;
            }

            if (ewk->wu.hit_stop && !EXE_flag && !Game_pause && !EXE_obroll) {
                char_move(&ewk->wu);
            }

            disp_pos_trans_entry_s(ewk);
            break;

        default:
            all_cgps_put_back(&ewk->wu);
            push_effect_work(&ewk->wu);
            break;
        }
    }
}

s32 effect_05_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt;
    s16 i;
    const s16* data_ptr;

    lp_cnt = scr_obj_num[bg_w.bg_index];
    if (lp_cnt == 0) {
        return 0;
    }

    data_ptr = scr_obj_data[bg_w.bg_index];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 5;
        ewk->wu.work_id = 0x10;
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
