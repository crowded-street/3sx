/**
 * @file effm5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effm6.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void effect_M5_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
            Sound_SE(ewk->master_id * 0x300 + 0x134);
        }

        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 1) {
                ewk->wu.routine_no[0]++;
                Appear_car_stop[ewk->master_id] = 1;
                set_char_move_init(&ewk->wu, 0, 0x68);

                if (Demo_Flag != 0) {
                    SsRequestPan(0x135, 0x40, 0x40, 0, 2);
                }
            } else {
                add_x_sub(&ewk->wu);
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 1) {
                ewk->wu.routine_no[0]++;
                ewk->wu.old_rno[0] = 0x14;
            } else if (ewk->wu.cg_type == 2) {
                demo_car_flag[ewk->master_id] = 1;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.old_rno[0] = 0x30;

                if (ewk->wu.rl_flag) {
                    ewk->wu.mvxy.a[0].sp = -0x20000;
                    ewk->wu.mvxy.d[0].sp = -0x1000;
                } else {
                    ewk->wu.mvxy.a[0].sp = 0x20000;
                    ewk->wu.mvxy.d[0].sp = 0x1000;
                }
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 4:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0]++;
            } else {
                add_x_sub(&ewk->wu);
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 5:
        ewk->wu.routine_no[0]++;
        demo_car_flag[ewk->master_id] = 0;
        ewk->wu.disp_flag = 0;
        break;

    case 6:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_M5_init(PLW* oya) {
    WORK_Other* ewk;
    s16 ix;
    s16 work;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    demo_car_flag[oya->wu.id] = 0;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 225;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 0;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 103;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_priority = ewk->wu.position_z = 57;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_col_code = oya->wu.my_col_code + 4;
    ewk->wu.sync_suzi = 0;
    ewk->master_id = oya->wu.id;

    if (oya->wu.id) {
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.rl_flag = 0;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work + 168) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work + 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[0], work, 0, 1, 1);
    } else {
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.rl_flag = 1;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work - 168) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work - 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[0], work, 0, 1, 1);
    }

    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    effect_M6_init(ewk);
    return 0;
}
