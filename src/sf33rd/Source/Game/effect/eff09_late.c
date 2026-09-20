/**
 * @file eff09_late.c
 * Effect 09 late lifecycle phases 17000 through 20000.
 */

#include "sf33rd/Source/Game/effect/eff09_late.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effb4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

extern const s16 eff09_data2[43][8];
extern const s32 eff09_19000_tbl[3][4];

static s32 eff09_late_updates_enabled(void) {
    return !EXE_flag && !Game_pause;
}

static void load_eff09_19000_speeds(WORK_Other* ewk) {
    ewk->wu.mvxy.a[0].sp = eff09_19000_tbl[ewk->wu.routine_no[1]][0];
    ewk->wu.mvxy.d[0].sp = eff09_19000_tbl[ewk->wu.routine_no[1]][1];
    ewk->wu.mvxy.a[1].sp = eff09_19000_tbl[ewk->wu.routine_no[1]][2];
    ewk->wu.mvxy.d[1].sp = eff09_19000_tbl[ewk->wu.routine_no[1]][3];
    ewk->wu.routine_no[1]++;
}

static void show_eff09_late_effect(WORK_Other* ewk) {
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void initialize_eff09_17000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.xyz[1].disp.pos += base_y_pos;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void advance_eff09_17000(WORK_Other* ewk, const WORK* oya_ptr) {
    if (eff09_late_updates_enabled()) {
        char_move(&ewk->wu);

        if (oya_ptr->cg_type == 9) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        } else if (oya_ptr->routine_no[1] != 4 || oya_ptr->routine_no[2] != 30) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }
    }

    ewk->wu.xyz[0].disp.pos = oya_ptr->xyz[0].disp.pos;

    if (oya_ptr->rl_flag) {
        ewk->wu.xyz[0].disp.pos -= eff09_data2[21][2];
    } else {
        ewk->wu.xyz[0].disp.pos += eff09_data2[21][2];
    }

    disp_pos_trans_entry_rs(ewk);
}

void eff09_17000(WORK_Other* ewk) {
    WORK* oya_ptr;

    if (obr_no_disp_check()) {
        return;
    }

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_17000(ewk);
        break;

    case 1:
        advance_eff09_17000(ewk, oya_ptr);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_18000(WORK_Other* ewk, const WORK* oya_ptr) {
    ewk->wu.routine_no[1]++;
    show_eff09_late_effect(ewk);

    if (oya_ptr->rl_flag) {
        if (oya_ptr->xyz[0].disp.pos < bg_w.bgw[1].wxy[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = oya_ptr->xyz[0].disp.pos - 256;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 32);
        }
    } else if (oya_ptr->xyz[0].disp.pos > bg_w.bgw[1].wxy[0].disp.pos) {
        ewk->wu.xyz[0].disp.pos = oya_ptr->xyz[0].disp.pos + 256;
    } else {
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 32);
    }

    ewk->wu.xyz[1].disp.pos = base_y_pos + 160;
    ewk->wu.old_rno[0] = 35;
    ewk->wu.old_rno[1] = oya_ptr->xyz[1].disp.pos + 106 + base_y_pos;
    cal_all_speed_data(
        &ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], oya_ptr->xyz[0].disp.pos, ewk->wu.old_rno[1], 0, 0 }
    );
}

static void advance_eff09_18000(WORK_Other* ewk) {
    if (eff09_late_updates_enabled()) {
        char_move(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        } else {
            add_x_sub2(&ewk->wu);
            add_y_sub2(&ewk->wu);
        }
    }
}

void eff09_18000(WORK_Other* ewk) {
    WORK* oya_ptr;

    if (obr_no_disp_check()) {
        return;
    }

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_18000(ewk, oya_ptr);
        break;

    case 1:
        advance_eff09_18000(ewk);
        disp_pos_trans_entry_rs(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 0;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_19000(WORK_Other* ewk) {
    show_eff09_late_effect(ewk);
    load_eff09_19000_speeds(ewk);
}

static s32 eff09_19000_reached_floor(WORK_Other* ewk) {
    if (!eff09_late_updates_enabled()) {
        return 0;
    }

    add_x_sub(&ewk->wu);
    add_y_sub(&ewk->wu);
    return ewk->wu.xyz[1].disp.pos < 64;
}

static void advance_eff09_19000_bounce(WORK_Other* ewk) {
    if (eff09_19000_reached_floor(ewk)) {
        ewk->wu.xyz[1].cal = 0x3F0000;
        load_eff09_19000_speeds(ewk);
    }
}

static void advance_eff09_19000_exit(WORK_Other* ewk) {
    if (eff09_19000_reached_floor(ewk)) {
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 0;
    }
}

void eff09_19000(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_19000(ewk);
        disp_pos_trans_entry(ewk);
        return;

    case 1:
    case 2:
        advance_eff09_19000_bounce(ewk);
        disp_pos_trans_entry(ewk);
        return;

    case 3:
        advance_eff09_19000_exit(ewk);
        disp_pos_trans_entry(ewk);
        return;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void advance_eff09_20000_left(WORK_Other* ewk, WORK* oya_ptr) {
    s16 pos_work;

    pos_work = bg_w.bgw[1].wxy[0].disp.pos - bg_w.pos_offset;
    pos_work -= 64;

    if (ewk->wu.xyz[0].disp.pos < pos_work) {
        oya_ptr->cmwk[1] = 1;
        ewk->wu.routine_no[1]++;
    }
}

static void advance_eff09_20000_right(WORK_Other* ewk, WORK* oya_ptr) {
    s16 pos_work;

    pos_work = bg_w.bgw[1].wxy[0].disp.pos + bg_w.pos_offset;
    pos_work += 64;

    if (ewk->wu.xyz[0].disp.pos > pos_work) {
        oya_ptr->cmwk[1] = 1;
        ewk->wu.routine_no[1]++;
    }
}

static void advance_eff09_20000(WORK_Other* ewk, WORK* oya_ptr) {
    if (eff09_late_updates_enabled()) {
        char_move(&ewk->wu);

        if (oya_ptr->id) {
            advance_eff09_20000_left(ewk, oya_ptr);
        } else {
            advance_eff09_20000_right(ewk, oya_ptr);
        }
    }
}

void eff09_20000(WORK_Other* ewk) {
    WORK* oya_ptr;

    if (test_flag) {
        ewk->wu.routine_no[1] = 99;
    }

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 5;
        ewk->wu.kage_hy = base_y_pos + 2;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 11;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        disp_pos_trans_entry(ewk);
        break;

    case 1:
        advance_eff09_20000(ewk, oya_ptr);
        disp_pos_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}
