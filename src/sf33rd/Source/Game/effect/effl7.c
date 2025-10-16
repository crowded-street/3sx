/**
 * @file effl7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

// forward declaration
const s16 effl7_data_tbl[16];

void effect_L7_move(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    if (Suicide[0] || (ewk->wu.dead_f)) {
        ewk->wu.routine_no[0] = 1;
        ewk->wu.disp_flag = 0;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if ((!EXE_flag) && (!Game_pause)) {
            effl7_move(ewk);
        }

        pl_eff_trans_entry(ewk);
        break;

    case 1:
        ewk->wu.routine_no[0] += 1;
        poison_flag[oya_ptr->id] = 0;
        /* fallthrough */

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effl7_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 0;
        ewk->wu.kage_hy = -10;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 16;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.old_rno[0] = 80;
        cal_initial_speed(&ewk->wu, ewk->wu.old_rno[0], ewk->wu.old_rno[1], ewk->wu.xyz[1].disp.pos);
        break;

    case 1:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1] += 1;
            set_char_move_init(&ewk->wu, 0, 1);
        }

        break;

    default:
        // Do nothing
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1] += 1;
            set_char_move_init(&ewk->wu, 1, ewk->wu.old_rno[2]);
        }

        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            ewk->wu.routine_no[1] += 1;
            ewk->wu.rl_flag ^= 1;
        }

        break;

    case 4:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1] += 1;

            // The original programmers managed to call a func that accepts 5 args with just 4.
            // This means that register t0 has some garbage that was stored there previously
            // by a different func.
            //
            // To match the original behavior run the game in PCSX2, set a breakpoing at
            // 0x234efc and note the value of t0. If it's 0, call set_char_move_init2
            // with 0 in the last arg. If it's non-zero, call with 1.
            // Original line: set_char_move_init2(&ewk->wu, 0, 0, 3);
            fatal_error("This part needs debugging. Check the comment above for details.");

            if (ewk->wu.rl_flag) {
                ewk->wu.mvxy.a[0].sp = 0x20000;
            } else {
                ewk->wu.mvxy.a[0].sp = -0x20000;
            }

            ewk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 5:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (range_x_check3(ewk, 64) == 0) {
            ewk->wu.routine_no[1] += 1;
            ewk->wu.disp_flag = 0;
            ewk->wu.kage_flag = 0;
        }

        break;

    case 6:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.routine_no[0] += 1;
        break;
    }
}

s32 effect_L7_init(WORK* wk, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;
    s16 kind_w;

    if ((wk->work_id == 1) && (((PLW*)wk)->player_number != My_char[wk->id])) {
        return 0;
    }

    if (poison_flag[wk->id]) {
        return 0;
    }

    if (wk->id) {
        if (!(p2sw_0 & 1)) {
            return 0;
        }
    } else if (!(p1sw_0 & 1)) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 217;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 1;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = (u32*)wk;
    ewk->wu.rl_flag = wk->rl_flag;

    if (wk->rl_flag) {
        if (wk->xyz[0].disp.pos < bg_w.bgw[1].wxy[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos - 256;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 32);
        }

        ewk->wu.old_rno[1] = wk->xyz[0].disp.pos - 32;
    } else {
        if (wk->xyz[0].disp.pos > bg_w.bgw[1].wxy[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos + 256;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 32);
        }

        ewk->wu.old_rno[1] = wk->xyz[0].disp.pos + 32;
    }

    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 12;
    ewk->wu.my_priority = 28;
    ewk->wu.position_z = 28;
    ewk->wu.char_table[0] = _etc3_char_table;
    ewk->wu.char_table[1] = _etc_char_table;
    ewk->wu.char_index = 0;
    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    kind_w = random_16();
    ewk->wu.old_rno[2] = effl7_data_tbl[kind_w];
    poison_flag[wk->id] = 1;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

const s16 effl7_data_tbl[16] = { 55, 56, 57, 55, 56, 57, 55, 57, 55, 56, 57, 55, 56, 57, 56, 57 };
