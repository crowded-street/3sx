/**
 * @file eff01.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff01.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"

const s16 parts_colmd_table[2] = { 0x4000, 0x0 };

const s16 parts_colcd_table[14] = {
    0x2000, 0x0, 0x6, 0x2000, 0x4, 0x2020, 0x4, 0x4, 0x0, 0x6, 0x5, 0x4, 0x203C, 0x202A
};

void get_new_parts_data(WORK_Other* ewk, PLW* mwk);
void set_parts_disp_flag(WORK_Other* ewk, PLW* mwk);

void effect_01_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.cgromtype = mwk->cgromtype;
        ewk->wu.cg_number = ewk->wu.old_cgnum = 0;
        ewk->wu.blink_timing = mwk->blink_timing;
        ewk->wu.cg_olc.olc_ix[ewk->wu.type] = 0;
        return;

    case 1:
        if (ewk->wu.dead_f == 1 || mwk->olc_work_ix[ewk->wu.type] != ewk->wu.myself) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        if (mwk->cg_olc.olc_ix[ewk->wu.type] == 0) {
            ewk->wu.cg_olc.olc_ix[ewk->wu.type] = 0;
            return;
        }

        if (!Game_pause && !EXE_flag) {
            if (ewk->wu.cg_olc.olc_ix[ewk->wu.type] != mwk->cg_olc.olc_ix[ewk->wu.type]) {
                ewk->wu.cg_olc.olc_ix[ewk->wu.type] = ewk->wu.cg_ix = mwk->cg_olc.olc_ix[ewk->wu.type];
                ewk->wu.now_koc = ewk->wu.cg_ix;

                if (ewk->wu.type == 0 && ((PLW*)mwk)->player_number == 0 && mwk->rl_flag) {
                    ewk->wu.now_koc++;
                }

                get_new_parts_data(ewk, (PLW*)mwk);
            } else if (((PLW*)mwk)->sa_stop_flag == 0) {
                if (--ewk->wu.cg_ctr == 0) {
                    if (ewk->wu.overlap_char_tbl->parts_nix) {
                        ewk->wu.cg_ix = ewk->wu.overlap_char_tbl->parts_nix;
                    } else {
                        ewk->wu.cg_ix++;
                    }

                    ewk->wu.now_koc = ewk->wu.cg_ix;
                    get_new_parts_data(ewk, (PLW*)mwk);
                }
            }

            if (ewk->wu.cg_number == 0) {
                break;
            }

            ewk->wu.position_x = mwk->position_x;
            ewk->wu.position_y = mwk->position_y;
            ewk->wu.position_z = mwk->position_z;
            ewk->wu.rl_flag = mwk->rl_flag;
            ewk->wu.cg_flip = ewk->wu.overlap_char_tbl->parts_flip & 3;

            if (ewk->wu.overlap_char_tbl->parts_flip & 4) {
                ewk->wu.cg_flip ^= mwk->cg_flip;

                if (mwk->cg_flip & 1) {
                    ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                }
            }

            if (ewk->wu.rl_flag) {
                ewk->wu.position_x -= ewk->wu.overlap_char_tbl->parts_hos_x;
            } else {
                ewk->wu.position_x += ewk->wu.overlap_char_tbl->parts_hos_x;
            }

            ewk->wu.position_y += ewk->wu.overlap_char_tbl->parts_hos_y;

            if (ewk->wu.overlap_char_tbl->parts_flip & 4 && mwk->cg_flip & 2) {
                ewk->wu.position_y -= ewk->wu.overlap_char_tbl->parts_hos_y * 2;
            }

            if (ewk->wu.overlap_char_tbl->parts_prio == 2) {
                ewk->wu.position_z -= (ewk->wu.type + 1) * 2;
            } else {
                ewk->wu.position_z += (ewk->wu.type + 1) * 2;
            }
        }

        if (ewk->wu.cg_number == 0) {
            break;
        }

        set_parts_disp_flag(ewk, (PLW*)mwk);

        if (ewk->wu.overlap_char_tbl->parts_colcd == 0) {
            ewk->wu.my_col_code = mwk->my_col_code;
            ewk->wu.extra_col = mwk->extra_col;
            ewk->wu.extra_col_2 = mwk->extra_col_2;
        }

        if (ewk->wu.overlap_char_tbl->parts_mts) {
            ewk->wu.my_mts = 14;
        } else {
            ewk->wu.my_mts = mwk->my_mts;
        }

        sort_push_request(&ewk->wu);
        break;

    case 2:
    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void get_new_parts_data(WORK_Other* ewk, PLW* mwk) {
    ewk->wu.now_koc = ewk->wu.cg_ix;

    if (ewk->wu.type == 0 && mwk->player_number == 0 && mwk->wu.rl_flag) {
        ewk->wu.now_koc++;
    }

    ewk->wu.overlap_char_tbl = mwk->wu.overlap_char_tbl + ewk->wu.now_koc;
    ewk->wu.cg_ctr = ewk->wu.overlap_char_tbl->parts_timer;

    if (ewk->wu.overlap_char_tbl->parts_colmd) {
        if (ewk->wu.overlap_char_tbl->parts_colmd == 1) {
            ewk->wu.my_col_mode = ((WORK*)mwk->wu.target_adrs)->my_col_mode;
        } else {
            ewk->wu.my_col_mode = parts_colmd_table[ewk->wu.overlap_char_tbl->parts_colmd];
        }
    } else {
        ewk->wu.my_col_mode = mwk->wu.my_col_mode;
    }

    if (ewk->wu.overlap_char_tbl->parts_colcd) {
        ewk->wu.extra_col = 0;
        ewk->wu.extra_col_2 = 0;
        ewk->wu.my_col_code = parts_colcd_table[ewk->wu.overlap_char_tbl->parts_colcd];

        if (!(ewk->wu.my_col_code & 0x2000)) {
            ewk->wu.my_col_code += mwk->wu.my_col_code;
        }
    } else {
        ewk->wu.my_col_code = mwk->wu.my_col_code;
        ewk->wu.my_col_code = mwk->wu.my_col_code;
        ewk->wu.extra_col = mwk->wu.extra_col;
        ewk->wu.extra_col_2 = mwk->wu.extra_col_2;
    }

    ewk->wu.cg_number = ewk->wu.overlap_char_tbl->parts_char;
}

void set_parts_disp_flag(WORK_Other* ewk, PLW* mwk) {
    switch (ewk->wu.overlap_char_tbl->parts_disp) {
    case 1:
        if (mwk->wu.disp_flag) {
            ewk->wu.disp_flag = 1;
            break;
        }

        ewk->wu.disp_flag = 0;
        break;

    case 2:
        if (mwk->wu.disp_flag) {
            ewk->wu.disp_flag = 2;
            break;
        }

        ewk->wu.disp_flag = 0;
        break;

    case 11:
        ewk->wu.disp_flag = 1;
        break;

    case 12:
        ewk->wu.disp_flag = 2;
        break;

    default:
        ewk->wu.disp_flag = mwk->wu.disp_flag;
        break;
    }
}

s32 effect_01_init(WORK* wk, u8 koolc) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(1)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 1;
    ewk->wu.work_id = 32;
    ewk->wu.type = koolc;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.blink_timing = wk->blink_timing;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    ewk->wu.my_mts = wk->my_mts;
    wk->olc_work_ix[koolc] = ewk->wu.myself;
    return 0;
}
