/**
 * @file eff124.c
 * Yun's skateboard after he jumps off it in his intro on his stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff124.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define BOARD_CHAR_INDEX 6

static const s32 bounce_speeds[5][4] = {
    { 0, 0, 0, 0 },
    { -0x40000, 0x10000, 0, -0x6000 },
    { -0x38000, 0x30000, -0x2000, -0x6000 },
    { -0x30000, 0x10000, -0x1000, -0x6000 },
    { -0x28000, 0, -0x200, 0 },
};

static void set_bounce_speeds(WORK_Other* ewk) {
    const s32* speeds = bounce_speeds[ewk->wu.routine_no[0]];

    ewk->wu.mvxy.a[0].sp = speeds[0];
    ewk->wu.mvxy.a[1].sp = speeds[1];
    ewk->wu.mvxy.d[0].sp = speeds[2];
    ewk->wu.mvxy.d[1].sp = speeds[3];
}

static void set_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
}

void effect_ng124_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        set_bounce_speeds(ewk);
        break;

    case 1:
    case 2:
    case 3:
        if (!EXE_flag && !Game_pause) {
            if (ewk->wu.cg_type != 2) {
                char_move(&ewk->wu);
            }

            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);

            if (ewk->wu.xyz[1].disp.pos < 0) {
                // FIXME: NG plays its sound 0x139 on the first bounce. NG's sounds aren't mapped to 3S's yet.
                ewk->wu.routine_no[0]++;
                set_bounce_speeds(ewk);
                char_move_z(&ewk->wu);
            }
        }

        set_position(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 4:
        if (!EXE_flag && !Game_pause) {
            add_x_sub_ng(&ewk->wu);
            char_move(&ewk->wu);

            if (ewk->wu.xyz[0].disp.pos < 0x80) {
                ewk->wu.routine_no[0]++;
            }
        }

        set_position(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 5:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng124_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG124_ID;
    ewk->wu.work_id = 32;
    ewk->wu.type = wk->wu.id;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = BOARD_CHAR_INDEX;
    ewk->my_master = wk;
    ewk->master_id = wk->wu.id;
    ewk->wu.my_col_mode = wk->wu.my_col_mode;
    ewk->wu.my_col_code = wk->wu.my_col_code;
    ewk->wu.my_mts = wk->wu.my_mts;
    ewk->wu.my_priority = ewk->wu.position_z = 16;
    ewk->wu.xyz[0].cal = wk->wu.xyz[0].cal;
    ewk->wu.xyz[1].cal = wk->wu.xyz[1].cal;
    ewk->wu.char_table[0] = _direct_03_char_table;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = -9;
    ewk->wu.kage_hy = -11;
    ewk->wu.kage_char = 11;
    ewk->wu.kage_prio = ewk->wu.position_z + 1;
    return 0;
}
