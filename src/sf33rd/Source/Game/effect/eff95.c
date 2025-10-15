/**
 * @file eff95.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff95.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

s16 RND_95;
s16 END_OF_95;

const s16 eff95_data_tbl[10][4] = { { 0, 0, 0, 0 }, { 0, 0, 1, 1 }, { 2, 2, 3, 3 }, { 4, 4, 5, 5 }, { 6, 6, 6, 7 },
                                    { 7, 7, 8, 8 }, { 8, 9, 9, 9 }, { 9, 9, 9, 9 }, { 9, 9, 9, 9 }, { 9, 9, 9, 9 } };

void effect_95_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!Continue_Count_Down[LOSER]) {
            ewk->wu.routine_no[0]++;
        }

        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        if (ewk->wu.dmcal_m != (Continue_Count[LOSER])) {
            if (!(ewk->wu.dmcal_m = Continue_Count[LOSER])) {
                if (Continue_Cut[Loser_id]) {
                    ewk->wu.routine_no[0] = 3;
                    ewk->wu.dir_step = 0;
                    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
                } else {
                    ewk->wu.old_rno[5] = 6;
                    ewk->wu.old_rno[6] = 6;
                    ewk->wu.dir_step = 9;
                    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
                }
            } else if (Continue_Count[LOSER] < 0) {
                ewk->wu.routine_no[0] = 3;
                ewk->wu.dir_step = 0;
                set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
            } else {
                ewk->wu.old_rno[5] = 6;
                ewk->wu.old_rno[6] = 6;
                ewk->wu.dir_step = 9;
                set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
            }
        } else {
            switch (ewk->wu.vital_new) {
            case 4:
                ewk->wu.old_rno[5] = ewk->wu.old_rno[5] - 1;

                if (ewk->wu.old_rno[5] <= 0) {
                    ewk->wu.old_rno[5] = 6;
                    ewk->wu.dir_step = ewk->wu.dir_step - 1;

                    if (ewk->wu.dir_step <= 0) {
                        ewk->wu.dir_step = 0;
                    }

                    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
                }

                break;

            case 8:
                ewk->wu.old_rno[6] = ewk->wu.old_rno[6] - 1;

                if (ewk->wu.old_rno[6] <= 0) {
                    ewk->wu.old_rno[6] = 6;
                }

                RND_95 = (random_16() >> 1) & 3;
                ewk->wu.dir_step = eff95_data_tbl[ewk->wu.old_rno[6]][RND_95];
                set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
                break;

            default:
                RND_95 = (random_16() >> 1) & 7;
                RND_95 = RND_95 + 3;

                if (RND_95 > 9) {
                    RND_95 = 0;
                }

                ewk->wu.dir_step = RND_95;
                set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
                break;
            }
        }

        if (Break_Into) {
            ewk->wu.routine_no[0] = 2;
        }

        break;

    case 2:
        break;

    case 3:
        if (!Ck_Range_Out_S(ewk, 1, 64)) {
            break;
        }

        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

s32 effect_95_init(s16 vital_new) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 95;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_col_code = 0x2090;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.direction = 3;
    ewk->wu.vital_new = vital_new;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 85;
    ewk->wu.dmcal_m = Continue_Count[LOSER];
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[1].wxy[1].disp.pos + 152;
    ewk->wu.position_z = 15;

    switch (vital_new) {
    case 1:
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 594;
        break;

    case 2:
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 570;
        break;

    case 8:
        ewk->wu.old_rno[6] = 6;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 538;
        break;

    case 4:
        END_OF_95 = 10;
        ewk->wu.old_rno[5] = 6;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 514;
        break;
    }

    ewk->wu.dir_step = 9;
    return 0;
}
