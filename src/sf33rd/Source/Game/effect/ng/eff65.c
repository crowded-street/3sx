/**
 * @file eff65.c
 * Horse-drawn carriage of the first area of Dudley's stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff65.h"
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

#define CHAR_INDEX 10
#define START_X 0x1A0
#define START_Y 0x40
#define START_SIZE 55
#define END_X 0x30

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void eff65_disp(WORK_Other* ewk) {
    if (!range_x_check_ng(ewk)) {
        return;
    }

    suzi_sync_pos_set(ewk);
    sort_push_request4(&ewk->wu);
}

static void eff65_char_move(WORK_Other* ewk) {
    char_move(&ewk->wu);

    switch (ewk->wu.cg_type) {
    case 1:
        ewk->wu.xyz[1].disp.pos++;
        break;

    case 2:
        ewk->wu.xyz[1].disp.pos--;
        break;

    default:
        return;
    }

    ewk->wu.cg_type = 0;
}

void effect_ng65_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
        ewk->wu.old_rno[0] = 600;
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = START_X;
        ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = START_Y;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.mvxy.a[0].sp = -0x12000;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.a[1].sp = 0x800;
        ewk->wu.mvxy.d[1].sp = 0;
        ewk->wu.my_mr.size.x = ewk->wu.my_mr.size.y = START_SIZE;
        break;

    case 1:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            // Waiting offscreen
            if (!is_paused()) {
                ewk->wu.old_rno[0]--;

                if (ewk->wu.old_rno[0] < 0) {
                    ewk->wu.routine_no[1]++;
                    ewk->wu.old_rno[0] = 196;
                }
            }

            break;

        case 1:
            // Driving in
            if (!is_paused()) {
                eff65_char_move(ewk);
                add_x_sub_ng(&ewk->wu);
                ewk->wu.old_rno[0]--;

                if (ewk->wu.old_rno[0] < 0) {
                    ewk->wu.routine_no[1]++;
                    ewk->wu.old_rno[0] = 0;
                    ewk->wu.mvxy.a[0].sp = -0x10000;
                    ewk->wu.mvxy.d[0].sp = 0;
                    ewk->wu.mvxy.a[1].sp = 0x1800;
                    ewk->wu.mvxy.d[1].sp = 0;
                }
            }

            eff65_disp(ewk);
            break;

        case 2:
            // Driving off into the distance
            if (ewk->wu.xyz[0].disp.pos < END_X) {
                ewk->wu.routine_no[0] = 0;
                break;
            }

            if (!is_paused()) {
                char_move(&ewk->wu);
                add_x_sub_ng(&ewk->wu);
                add_y_sub_ng(&ewk->wu);
                ewk->wu.old_rno[0] = (ewk->wu.old_rno[0] + 1) & 7;

                if (ewk->wu.old_rno[0] == 7 && ewk->wu.my_mr.size.x != 0) {
                    ewk->wu.my_mr.size.x--;
                    ewk->wu.my_mr.size.y--;
                }
            }

            eff65_disp(ewk);
            break;
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng65_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG65_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.dead_f = 1;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 0x2145; // NG 0x2059
    ewk->wu.my_priority = ewk->wu.position_z = 79;
    ewk->wu.char_table[0] = _ng_eng_char_table;
    ewk->wu.sync_suzi = 1;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = START_X;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = START_Y;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
