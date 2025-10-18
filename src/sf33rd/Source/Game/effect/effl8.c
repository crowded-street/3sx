/**
 * @file effl8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl8.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"

u32 spmv_ng_save[2];

// forward declaration

const s16 pl17_0_00[12];

void effect_L8_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;
    s16* save_old_col_ptr = (s16*)&ewk->wu.zu_flag;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.hit_adrs = pl17_0_00;
        ewk->wu.step_xy_table = (s16*)ColorRAM[(ewk->master_id == 1) * 16];
        ewk->wu.move_xy_table = ewk->wu.step_xy_table + 512;
        save_old_color_data(save_old_col_ptr, ewk->wu.step_xy_table);
        check_new_color_data_L8(&ewk->wu);
        mwk->att_plus = 14;
        spmv_ng_save[ewk->master_id] = mwk->spmv_ng_flag;
        mwk->spmv_ng_flag = (mwk->spmv_ng_flag | 48);
        break;

    case 1:
        if ((mwk->sa->ok == -1) && (ewk->wu.dead_f == 0)) {
            break;
        }

        ewk->wu.routine_no[0] += 1;
        load_old_color_data(save_old_col_ptr, ewk->wu.step_xy_table);
        load_old_color_data(save_old_col_ptr, ewk->wu.move_xy_table);
        mwk->att_plus = 8;
        mwk->spmv_ng_flag = spmv_ng_save[ewk->master_id];
        break;

    case 2:
        ewk->wu.routine_no[0] += 1;
        /* fallthrough */

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void check_new_color_data_L8(WORK* wk) {
    get_new_color_data_L8(wk, (s16*)wk->hit_adrs, wk->step_xy_table);
    get_new_color_data_L8(wk, (s16*)wk->hit_adrs, wk->move_xy_table);
}

void get_new_color_data_L8(WORK* /* unused */, s16* trom, s16* tram) {
    s16 i;
    u16 col;

    for (i = 0; i < 12; i++) {
        col = *trom++;
        *tram++ = palConvSrcToRam(col);
    }
}

void save_old_color_data(s16* wram, s16* tram) {
    s16 i;

    for (i = 0; i < 12; i++) {
        *wram++ = *tram++;
    }
}

void load_old_color_data(s16* wram, s16* tram) {
    s16 i;

    for (i = 0; i < 12; i++) {
        *tram++ = *wram++;
    }
}

s32 effect_L8_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if (wk->player_number != 16) {
        return 0;
    }

    if ((ix = pull_effect_work(6)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 218;
    ewk->wu.work_id = 16;
    ewk->my_master = wk;
    ewk->master_id = wk->wu.id;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_player = wk->player_number;
    return 0;
}

const s16 pl17_0_00[12] = { 0, -278, -439, -569, -668, -1790, -2944, -732, -796, -893, -5054, -9216 };
