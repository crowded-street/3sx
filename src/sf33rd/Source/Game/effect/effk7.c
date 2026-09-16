/**
 * @file effk7.c
 * Code related to twelve's X.C.O.P.Y. super
 */

#include "sf33rd/Source/Game/effect/effk7.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/meta_col.h"

void K7_move_type_0(WORK_Other* ewk, PLW* mwk);
s16 K7_mt0_rebirth_check(PLW* mwk);

static s32 should_end_effect(const WORK_Other* ewk, const PLW* mwk) {
    return ewk->wu.dead_f != 0 || mwk->metamor_index != ewk->wu.myself;
}


static void k7_restore_master(const WORK_Other* ewk, PLW* mwk) {
    mwk->metamorphose = 0;
    mwk->metamor_over = 0;
    mwk->att_plus = 8;
    mwk->def_plus = 8;
    mwk->wu.my_col_mode = ewk->wu.my_col_mode;
    mwk->wu.my_col_code = ewk->wu.my_col_code;
    mwk->wu.disp_flag = 1;
}

void effect_K7_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] = 1;
        break;

    case 1:
        if (should_end_effect(ewk, mwk)) {
            ewk->wu.routine_no[0] = 3;
            metamor_color_restore(mwk->wu.id);
            k7_restore_master(ewk, mwk);
        } else if (EXE_flag == 0 && Game_pause == 0) {
            K7_move_type_0(ewk, mwk);
        }

        break;

    case 2:
        k7_restore_master(ewk, mwk);
        /* fallthrough */

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void k7_become_opponent(WORK_Other* ewk, PLW* mwk) {
    if (mwk->wu.cg_type != 20) {
        return;
    }

    ewk->wu.routine_no[1] = 1;
    ewk->wu.direction = mwk->player_number;
    ewk->wu.charset_id = mwk->wu.charset_id;
    mwk->player_number = ((PLW*)mwk->wu.target_adrs)->player_number;
    mwk->wu.charset_id = ((PLW*)mwk->wu.target_adrs)->wu.charset_id;
    set_base_data_metamorphose(mwk, mwk->wu.id + 1 & 1);
    mwk->att_plus = 10;
    mwk->def_plus = 6;

    if (mwk->wu.operator == 0) {
        Next_Be_Free(mwk);
    }
}

static s32 k7_wait_super_end(WORK_Other* ewk, PLW* mwk) {
    if (mwk->dead_flag != 0) {
        ewk->wu.routine_no[1] = 9;
        return 1;
    }

    if (mwk->sa->ok == -1) {
        return 1;
    }

    ewk->wu.routine_no[1] = 3;
    mwk->metamor_over = 1;
    return 0;
}

static void k7_rebirth(WORK_Other* ewk, PLW* mwk) {
    if (K7_mt0_rebirth_check(mwk) == 0) {
        return;
    }

    if (pcon_rno[0] != 1) {
        ewk->wu.routine_no[1] = 9;
        return;
    }

    ewk->wu.routine_no[1] = 4;
    mwk->wu.routine_no[1] = 4;
    mwk->wu.routine_no[2] = 33;
    mwk->wu.routine_no[3] = 0;
    mwk->wu.cg_type = 0;
    mwk->wu.cg_hit_ix = 0;
    mwk->wu.cg_ja = mwk->wu.hit_ix_table[mwk->wu.cg_hit_ix];
    set_jugde_area(&mwk->wu);
}

static void k7_restore_self(WORK_Other* ewk, PLW* mwk) {
    if (mwk->wu.cg_type != 30) {
        return;
    }

    ewk->wu.routine_no[1] = 5;
    mwk->player_number = ewk->wu.direction;
    mwk->wu.charset_id = ewk->wu.charset_id;
    set_base_data_metamorphose(mwk, mwk->wu.id);
    metamor_color_restore(mwk->wu.id);

    if (mwk->wu.operator == 0) {
        Next_Be_Free(mwk);
    }
}

static void K7_move_type_0_late(WORK_Other* ewk, PLW* mwk) {
    switch (ewk->wu.routine_no[1]) {
    case 4:
        k7_restore_self(ewk, mwk);
        break;

    case 5:
        ewk->wu.routine_no[0] = 2;
        break;
    }
}

void K7_move_type_0(WORK_Other* ewk, PLW* mwk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        k7_become_opponent(ewk, mwk);
        break;

    case 1:
        if (mwk->wu.cg_type != 30) {
            break;
        }

        ewk->wu.routine_no[1] = 2;
        metamor_color_trans(mwk->wu.id);
        metamor_color_copy(mwk->wu.id);
        mwk->metamorphose = 1;
        /* fallthrough */

    case 2:
        if (k7_wait_super_end(ewk, mwk)) {
            break;
        }

        /* fallthrough */

    case 3:
        k7_rebirth(ewk, mwk);
        break;

    default:
        K7_move_type_0_late(ewk, mwk);
        break;
    }
}

s16 K7_mt0_rebirth_check(PLW* mwk) {
    s16 num = 0;

    switch (mwk->wu.routine_no[1]) {
    case 0:
        if (pcon_dp_flag) {
            if (mwk->wu.routine_no[2] == 1 && mwk->wu.routine_no[3] != 0) {
                num = 1;
            }
        } else if (mwk->guard_flag != 3 && !mwk->wu.hit_stop) {
            num = 1;
        }

        break;
    }

    return num;
}

void K7_muriyari_metamor_rebirth(PLW* wk) {
    if (wk->metamorphose == 0) {
        return;
    }

    wk->player_number = My_char[wk->wu.id];
    wk->wu.charset_id = plid_data[My_char[wk->wu.id]];
    set_base_data_metamorphose(wk, wk->wu.id);
    metamor_color_restore(wk->wu.id);
    wk->metamor_over = 0;
    wk->metamorphose = 0;
}

s32 effect_K7_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if (test_flag) {
        return -1;
    }

    if ((ix = pull_effect_work(6)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 207;
    ewk->wu.work_id = 16;
    ewk->wu.type = 0;
    ewk->my_master = wk;
    wk->metamor_index = ewk->wu.myself;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2000;

    if (wk->wu.id) {
        ewk->wu.my_col_code += 16;
    }

    return 0;
}
