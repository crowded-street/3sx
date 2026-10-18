/**
 * @file eff56.c
 * Flock of flamingos far away in the sky of Elena's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff56.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"


#define CHAR_INDEX 16
#define COLOR_CODE 0x143 // NG 0x57
#define START_X -256
#define START_Y 112
#define END_X 511
#define SPEED 0x2500
#define SIZE 12
#define FIRST_DELAY 108
#define DELAY 600

static void eff56_wait(WORK_Other* ewk) {
    ewk->wu.old_rno[0]--;

    if (ewk->wu.old_rno[0] < 0) {
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
    }
}

static void eff56_fly(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = ewk->wu.my_mr.size.y = SIZE;
        ewk->wu.mvxy.a[0].sp = SPEED;
        ewk->wu.mvxy.d[0].sp = 0;
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < END_X) {
            add_x_sub_ng(&ewk->wu);
        } else {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        ewk->wu.routine_no[0] = 0;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.xyz[0].disp.pos = START_X;
        ewk->wu.xyz[1].disp.pos = START_Y;
        ewk->wu.disp_flag = 0;
        ewk->wu.old_rno[0] = DELAY;
        set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
        break;
    }
}

void effect_ng56_move(WORK_Other* ewk) {
    void (*const states[2])(WORK_Other*) = { eff56_wait, eff56_fly };

    if (compel_dead_check(ewk)) {
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (!EXE_flag && !Game_pause) {
        states[ewk->wu.routine_no[0]](ewk);
    }

    // NG skips the check of `obr_no_disp_check` here
    if (range_x_check_ng(ewk)) {
        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
    }
}

s32 effect_ng56_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG56_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.charset_id = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.sync_suzi = 1;
    ewk->wu.my_family = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = START_X;
    ewk->wu.xyz[1].disp.pos = START_Y;
    ewk->wu.my_mr_flag = 1;
    ewk->wu.my_priority = ewk->wu.position_z = 81;
    ewk->wu.old_rno[0] = FIRST_DELAY;
    ewk->wu.disp_flag = 0;
    ewk->wu.char_table[0] = _ng_afc_a_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
    suzi_offset_set(ewk);
    return 0;
}
