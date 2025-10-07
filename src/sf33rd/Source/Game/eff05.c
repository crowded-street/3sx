#include "sf33rd/Source/Game/eff05.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

const s16 scr_obj_num[22] = { 0, 2, 0, 0, 0, 2, 1, 0, 3, 1, 0, 1, 1, 1, 1, 2, 2, 0, 2, 4, 1, 4 };

const s16 stg_dum_data_tbl[1] = { 0 };

const s16 stg0100_data_tbl[16] = { 0, 1, 300, 568, 48, 86, 0, 0, 0, 2, 8492, 528, 47, 68, 19, 0 };

const s16 stg0A00_data_tbl[8] = { 0, 3, 300, 512, 16, 88, 10, 0 };

const s16 stg0500_data_tbl[32] = { 0, 3, 300, 512, 0, 88, 9, 0, 0, 3, 8492, 560, 0, 88, 10, 0,
                                   0, 2, 300, 512, 0, 84, 3, 0, 0, 2, 300,  512, 0, 84, 4,  0 };

const s16 stg1300_data_tbl[32] = { 0, 3, 300, 512, 0, 88, 21, 0, 0, 3, 8492, 560, 0, 88, 22, 0,
                                   0, 2, 300, 512, 0, 84, 15, 0, 0, 2, 300,  512, 0, 84, 16, 0 };

const s16 stg0600_data_tbl[8] = { 0, 1, 8492, 511, 96, 88, 11, 0 };

const s16 stg0700_data_tbl[32] = { 0, 3, 8492, 504, 48,  94, 4, 0, 0, 3, 8492, 504, 112, 94, 8, 0,
                                   0, 2, 8492, 464, 256, 84, 7, 0, 0, 2, 8492, 512, 352, 84, 8, 0 };

const s16 stg0800_data_tbl[48] = { 0, 2, 8492, 496, 48, 85, 1,  0, 0, 7, 8492, 512, 43,  88, 12, 0,
                                   0, 7, 8492, 512, 43, 88, 13, 0, 0, 2, 8492, 624, 120, 86, 7,  0,
                                   0, 3, 8492, 512, 72, 90, 1,  0, 0, 3, 8492, 512, 72,  90, 2,  0 };

const s16 stg0900_data_tbl[8] = { 0, 6, 8492, 352, 16, 10, 5, 0 };

const s16 stg0c00_data_tbl[8] = { 0, 6, 8492, 352, 16, 10, 5, 0 };

const s16 stg0d00_data_tbl[8] = { 0, 3, 300, 512, 32, 88, 0, 0 };

const s16 stg0e00_data_tbl[24] = { 0,  3,  300, 512, 88, 98, 7,   0,   0,   2,  300, 576,
                                   64, 84, 5,   0,   0,  2,  300, 560, 208, 84, 6,   0 };

const s16 stg1000_data_tbl[16] = { 0, 3, 8492, 512, 80, 86, 0, 0, 0, 2, 8492, 480, 16, 12, 2, 0 };

const s16 stg1100_data_tbl[16] = { 0, 1, 8492, 512, 176, 90, 2, 0, 0, 1, 8492, 512, 240, 90, 3, 0 };

const s16 stg1400_data_tbl[32] = { 0, 2, 8492, 504, 11,  10, 0, 0, 0, 1, 8492, 496, 64,  86, 1, 0,
                                   0, 3, 300,  496, 144, 88, 3, 0, 0, 3, 300,  512, 320, 83, 5, 0 };

const s16 stg1500_data_tbl[8] = { 0, 2, 300, 445, 48, 10, 0, 0 };

const s16 stg1600_data_tbl[32] = { 0, 2, 300, 624, 0, 10, 2, 0, 0, 2, 8492, 511, 0,  12, 3,  0,
                                   0, 2, 300, 511, 0, 80, 4, 0, 0, 2, 300,  608, 48, 77, 11, 0 };

u32* char_add[22] = { _fnl_char_table, _usa_char_table, _j10_char_table, _hkg_char_table, _eng_char_table,
                      _rca_char_table, _grm_char_table, _j11_char_table, _afc_char_table, _brz_char_table,
                      _hkg_char_table, _usa_char_table, _brz_char_table, _orm_char_table, _jp2_char_table,
                      _chn_char_table, _jp3_char_table, _usa_char_table, _rca_char_table, _frc_char_table,
                      _bns_char_table, _bns_char_table };

const s16* scr_obj_data[22] = { stg_dum_data_tbl, stg0100_data_tbl, stg_dum_data_tbl, stg_dum_data_tbl,
                                stg_dum_data_tbl, stg0500_data_tbl, stg0600_data_tbl, stg0700_data_tbl,
                                stg0800_data_tbl, stg0900_data_tbl, stg0A00_data_tbl, stg0100_data_tbl,
                                stg0c00_data_tbl, stg0d00_data_tbl, stg0e00_data_tbl, stg1000_data_tbl,
                                stg1100_data_tbl, stg_dum_data_tbl, stg1300_data_tbl, stg1400_data_tbl,
                                stg1500_data_tbl, stg1600_data_tbl };

void effect_05_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif
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
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif
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
        ewk->wu.sync_suzi = *data_ptr++;

        suzi_offset_set(ewk);

        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
