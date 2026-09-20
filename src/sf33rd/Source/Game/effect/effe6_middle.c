/**
 * @file effe6_middle.c
 * Middle state handlers for effect E6.
 */

#include "sf33rd/Source/Game/effect/effe6_middle.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effe6_internal.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_00.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static void update_effe6_0011_fall(WORK_Other* ewk) {
    if (ewk->wu.old_rno[6] < end_w.r_no_2) {
        ewk->wu.routine_no[2] = 99;
    } else if (ewk->wu.type == 30) {
        ewk->wu.xyz[1].cal -= 0x8000;

        if (ewk->wu.xyz[1].disp.pos < 65) {
            ewk->wu.routine_no[2] = 99;
            ewk->wu.xyz[1].cal = 0x400000;
        }
    }
}

void effe6_0011(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        effe6_init_common(ewk);
        ewk->wu.old_rno[2] = 30;
        disp_pos_trans_entry(ewk);
        break;

    case 1:
        ewk->wu.old_rno[2]--;

        if (ewk->wu.old_rno[2] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 2:
        update_effe6_0011_fall(ewk);
        disp_pos_trans_entry(ewk);
        break;
    }
}

static void update_effe6_0013_motion(WORK_Other* ewk) {
    add_y_sub(&ewk->wu);

    if (ewk->wu.type == 50) {
        if (ewk->wu.xyz[1].disp.pos > 272) {
            ewk->wu.routine_no[2] = 99;
        }
    } else if (ewk->wu.xyz[1].disp.pos < 32) {
        ewk->wu.routine_no[2] = 99;
    }

    disp_pos_trans_entry(ewk);
}

void effe6_0013(WORK_Other* ewk) {
    mark_effe6_for_cleanup_if_stale(ewk);

    switch (ewk->wu.routine_no[1]) {
    case 0:
        effe6_init_common(ewk);
        disp_pos_trans_entry(ewk);

        if (ewk->wu.type == 50) {
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0x400;
            break;
        }

        ewk->wu.mvxy.a[1].sp = 0;
        ewk->wu.mvxy.d[1].sp = -0x400;
        break;

    case 1:
        update_effe6_0013_motion(ewk);
        break;
    }
}

static void update_effe6_0015_approach(WORK_Other* ewk) {
    if (ewk->wu.old_rno[6] == end_w.r_no_2) {
        ewk->wu.routine_no[1] = 3;
        ewk->wu.xyz[0].disp.pos = 544;
        ewk->wu.xyz[1].disp.pos = 48;
    } else {
        ewk->wu.old_rno[2]--;

        if (ewk->wu.old_rno[2] <= 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.xyz[0].disp.pos = 544;
            ewk->wu.xyz[1].disp.pos = 48;
        } else {
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);
        }
    }

    disp_pos_trans_entry(ewk);
}

static void update_effe6_0015_animation(WORK_Other* ewk) {
    if (ewk->wu.old_rno[6] == end_w.r_no_2) {
        ewk->wu.routine_no[1] = 3;
    } else {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1] = 3;
        }
    }
}

void effe6_0015(WORK_Other* ewk) {
    mark_effe6_for_cleanup_if_stale(ewk);

    switch (ewk->wu.routine_no[1]) {
    case 0:
        effe6_init_common(ewk);
        ewk->wu.old_rno[2] = 40;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[2], 544, 48, 2, 2 });
        break;

    case 1:
        update_effe6_0015_approach(ewk);
        break;

    case 2:
        update_effe6_0015_animation(ewk);

        /* fallthrough */

    case 3:
        disp_pos_trans_entry(ewk);
        break;
    }
}
