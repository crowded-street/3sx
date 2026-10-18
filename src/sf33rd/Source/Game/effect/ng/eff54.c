/**
 * @file eff54.c
 * Feathers that the vulture on Elena's New Gen stage sheds when it takes off
 */

#include "sf33rd/Source/Game/effect/ng/eff54.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2143 // NG 0x2057
#define CHAR_INDEX 24

typedef struct FeatherStep {
    s16 time;
    s32 speed_x;
    s32 speed_y;
    s32 accel_x;
    s32 accel_y;
} FeatherStep;

static const FeatherStep feather_steps[3] = {
    { 12, 0x20000, 0x18000, 0x4000, 0x6000 },
    { 30, 0, 0, 0, 0 },
    { 0, 0, -0x100, -0x100, -0x200 },
};

static void set_motion(WORK_Other* ewk, const FeatherStep* step) {
    ewk->wu.mvxy.a[0].sp = step->speed_x;
    ewk->wu.mvxy.a[1].sp = step->speed_y;
    ewk->wu.mvxy.d[0].sp = step->accel_x;
    ewk->wu.mvxy.d[1].sp = step->accel_y;
}

void effect_ng54_move(WORK_Other* ewk) {
    const FeatherStep* step;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
        step = &feather_steps[0];
        ewk->wu.direction = step->time;
        set_motion(ewk, step);
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.direction--;

            if (ewk->wu.direction < 0) {
                ewk->wu.direction = feather_steps[1].time;
                ewk->wu.routine_no[0]++;
            } else {
                char_move(&ewk->wu);
                add_x_sub_ng(&ewk->wu);
                add_y_sub_ng(&ewk->wu);
            }
        }

        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.direction--;

            if (ewk->wu.direction < 0) {
                step = &feather_steps[2];
                ewk->wu.direction = step->time;
                set_motion(ewk, step);
                ewk->wu.routine_no[0]++;
            }
        }

        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);

            if (ewk->wu.xyz[1].disp.pos < 0) {
                ewk->wu.routine_no[0]++;
            }
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    disp_pos_trans_entry(ewk);
}

s32 effect_ng54_init(WORK* wk, u8 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG54_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.charset_id = 0;
    ewk->my_master = wk;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = (wk->xyz[0].disp.pos + 0x15) & 0x3FF;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = 0x5F;
    ewk->wu.my_priority = ewk->wu.position_z = 10;
    ewk->wu.char_table[0] = _ng_afc_a_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
