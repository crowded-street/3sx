/**
 * @file eff55.c
 * Parrot in Oro's cave from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff55.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2144 // NG 0x2058
#define PERCH_X 672
#define PERCH_Y 114
#define PERCH_PRIORITY 86
#define RETURN_PRIORITY 80
#define RETURN_START_X 96
#define RETURN_START_Y 168
#define RETURN_TIME 160
#define RIGHT_EDGE 0x390
#define LEFT_EDGE 0x60

#define CHAR_PERCHED 20
#define CHAR_FLYING 21
#define CHAR_LANDING 26

static void eff55_perch(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_PERCHED);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

static void eff55_fly(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        // Take off to the right
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_FLYING);
        ewk->wu.mvxy.a[0].sp = 0x18000;
        ewk->wu.mvxy.a[1].sp = 0x80;
        ewk->wu.mvxy.d[0].sp = 0x2800;
        ewk->wu.mvxy.d[1].sp = 0x200;
        break;

    case 1:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos > RIGHT_EDGE) {
            if ((random_16() & 1) == 0) {
                ewk->wu.routine_no[1] = 2;
                ewk->wu.old_rno[0] = 352;
            } else {
                ewk->wu.routine_no[1] = 4;
                ewk->wu.old_rno[0] = 864;
            }

            ewk->wu.disp_flag = 0;
        }

        break;

    case 2:
        // Off screen, then fly back to the left across the stage
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1] = 3;
            ewk->wu.disp_flag = 1;
            ewk->wu.rl_flag = 1;
            ewk->wu.mvxy.a[0].sp = -0x18000;
            ewk->wu.mvxy.a[1].sp = 0x180;
            ewk->wu.mvxy.d[0].sp = -0x2800;
            ewk->wu.mvxy.d[1].sp = 0x210;
        }

        break;

    case 3:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < LEFT_EDGE) {
            ewk->wu.rl_flag = 0;
            ewk->wu.routine_no[1] = 4;
            ewk->wu.old_rno[0] = 352;
            ewk->wu.disp_flag = 0;
        }

        break;

    case 4:
        // Off screen, then fly from the left back to the perch
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1] = 5;
            ewk->wu.disp_flag = 1;
            ewk->wu.xyz[0].disp.pos = RETURN_START_X;
            ewk->wu.xyz[1].disp.pos = RETURN_START_Y;
            ewk->wu.old_rno[0] = RETURN_TIME;
            cal_all_speed_data(&ewk->wu, RETURN_TIME, PERCH_X, PERCH_Y, 2, 2);
            ewk->wu.position_z = ewk->wu.my_priority = RETURN_PRIORITY;
        }

        break;

    case 5:
        char_move(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[1] = 0;
            set_char_move_init(&ewk->wu, 0, CHAR_LANDING);
            ewk->wu.xyz[0].disp.pos = PERCH_X;
            ewk->wu.xyz[1].disp.pos = PERCH_Y;
            ewk->wu.position_z = ewk->wu.my_priority = PERCH_PRIORITY;
            break;
        }

        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);
        break;
    }
}

static void eff55_land(WORK_Other* ewk) {
    char_move(&ewk->wu);

    if (ewk->wu.cg_type != 0) {
        ewk->wu.routine_no[0] = 0;
        ewk->wu.routine_no[1] = 0;
    }
}

void effect_ng55_move(WORK_Other* ewk) {
    void (*const states[3])(WORK_Other*) = { eff55_perch, eff55_fly, eff55_land };

    if (!EXE_flag && !Game_pause) {
        states[ewk->wu.routine_no[0]](ewk);
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

s32 effect_ng55_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG55_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = PERCH_X;
    ewk->wu.xyz[1].disp.pos = PERCH_Y;
    ewk->wu.my_priority = ewk->wu.position_z = PERCH_PRIORITY;
    ewk->wu.hit_stop = 0;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_brz_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    set_char_move_init(&ewk->wu, 0, CHAR_PERCHED); // NG sets the animation on the first move
    suzi_offset_set(ewk);
    return 0;
}
