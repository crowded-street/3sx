/**
 * @file eff80.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff80.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void effect_80_move(WORK_Other* ewk) {
    WORK_Other* mwk = (WORK_Other*)ewk->my_master;

    if (mwk->wu.be_flag == 0) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 3;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        if (Ck_Range_Out_S(ewk, 2, 96)) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        break;

    case 2:
        ewk->wu.routine_no[0] = 99;
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.disp_flag = Disp_Command_Name[ewk->master_id][ewk->master_player];
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = mwk->wu.position_x;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = mwk->wu.position_y;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = mwk->wu.position_z - 1;
    sort_push_request4(&ewk->wu);
}

s32 effect_80_init(WORK_Other* mwk, s16 PL_id, s16 Plate_id, s16 Target_BG) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 80;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x90;
    ewk->wu.my_family = Target_BG + 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->master_id = PL_id;
    ewk->master_player = Plate_id;
    ewk->my_master = (u32*)mwk;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.char_index = 7;
    ewk->wu.dir_step = Plate_id + (My_char[PL_id] * 3) + (PL_id * 60);
    return 0;
}
