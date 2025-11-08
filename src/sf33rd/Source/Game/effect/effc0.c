/**
 * @file effc0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc0.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

const s16 hok_table_ef[8] = { 1, 1, 2, 2, 3, 3, 3, 3 };

const s16 plhos_data[20][3] = { { 2, 136, 4 }, { 40, 92, 3 },  { -4, 104, 2 },  { 2, 92, 5 },   { 40, 88, 5 },
                                { 8, 94, 2 },  { 8, 106, 2 },  { 20, 78, 2 },   { 30, 96, 2 },  { 10, 88, 3 },
                                { 2, 92, 5 },  { -4, 104, 2 }, { -4, 104, 5 },  { 2, 136, 4 },  { -4, 104, 2 },
                                { 6, 90, 2 },  { -4, 104, 2 }, { -41, 127, 5 }, { -4, 104, 4 }, { -4, 104, 4 } };

void effect_C0_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;
    s16 i;
    s16 hok;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0x2020;
        set_char_move_init(&ewk->wu, 0, plhos_data[mwk->player_number][2]);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (mwk->wu.routine_no[1] != 1 || mwk->wu.routine_no[2] != 25) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0 && mwk->sa_stop_flag != 1) {
            if (mwk->cp->lgp > 13) {
                hok = 3;
            } else {
                hok = hok_table_ef[mwk->cp->lgp / 2];
            }

            for (i = 0; i < hok; i++) {
                char_move(&ewk->wu);
            }
        }

        ewk->wu.position_x = mwk->wu.position_x;

        if (mwk->wu.rl_flag) {
            ewk->wu.position_x += plhos_data[mwk->player_number][0];
        } else {
            ewk->wu.position_x -= plhos_data[mwk->player_number][0];
        }

        ewk->wu.position_y = mwk->wu.position_y + plhos_data[mwk->player_number][1];
        ewk->wu.position_z = mwk->wu.position_z - 4;
        sort_push_request8(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_C0_init(PLW* wk, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 120;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->wu.cgromtype = 1;
    ewk->wu.position_x = wk->wu.position_x;
    ewk->wu.position_y = wk->wu.position_y;
    ewk->wu.position_z = wk->wu.position_z - 4;
    ewk->my_master = wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
