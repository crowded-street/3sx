/**
 * @file eff59.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff59.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"

const s16 EFF59_Correct_Data[6][2] = { { 0, 0 }, { 4, 0 }, { 0, 0 }, { 0, 0 }, { 0, -128 }, { 0, 0 } };

const s16 EFF59_ID05_Data[24][4] = {
    { -48, 96, -40, 16 },  { -56, 112, -40, 16 }, { -48, 104, -40, 16 }, { -48, 104, -40, 16 }, { -72, 144, -40, 16 },
    { -72, 144, -40, 16 }, { -56, 112, -40, 16 }, { -56, 112, -40, 16 }, { -72, 135, -40, 16 }, { -48, 96, -40, 16 },
    { -64, 120, -40, 16 }, { -56, 104, -40, 16 }, { -56, 112, -40, 16 }, { -72, 136, -40, 16 }, { -72, 136, -40, 16 },
    { -72, 144, -40, 16 }, { -80, 160, -40, 16 }, { -32, 70, -40, 16 },  { -72, 144, -40, 16 }, { -64, 128, -40, 16 },
    { 0, 0, 0, 0 },        { 0, 0, 0, 0 },        { 0, 0, 0, 0 },        { -72, 144, -40, 16 }
};

void EFF59_Trans(WORK_Other* ewk);
void Check_Under_Name(WORK_Other* ewk);
s32 Check_Break_Into_59(WORK_Other* ewk);

void effect_59_move(WORK_Other* ewk) {
    WORK_Other* mwk = (WORK_Other*)ewk->my_master;

    if (mwk->wu.be_flag == 0) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 3;
        push_effect_work(&ewk->wu);
        return;
    }

    Check_Break_Into_59(ewk);

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        return;

    case 1:
        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 224)) {
            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = mwk->wu.position_x + ewk->wu.vital_new;
        } else {
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[0]++;
        }

        break;

    case 2:
        Check_Under_Name(ewk);

        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 224)) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        break;

    case 3:
        ewk->wu.routine_no[0] = 99;
        return;

    case 4:
        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos;
        ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos - 16;
        EFF59_Trans(ewk);
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = mwk->wu.position_x + ewk->wu.vital_new;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = mwk->wu.position_y + ewk->wu.vital_old;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = mwk->wu.position_z + ewk->wu.direction;
    EFF59_Trans(ewk);
}

void EFF59_Trans(WORK_Other* ewk) {
    if (ewk->wu.dm_vital == 4 || ewk->wu.dm_vital == 5) {
        sort_push_requestA(&ewk->wu);
    } else {
        sort_push_request4(&ewk->wu);
    }
}

void Check_Under_Name(WORK_Other* ewk) {
    WORK_Other* mwk;

    if (ewk->wu.dm_vital != 5) {
        return;
    }

    mwk = (WORK_Other*)ewk->my_master;

    if (ewk->wu.dir_step != mwk->wu.dir_step) {
        ewk->wu.dir_step = mwk->wu.dir_step;
        ewk->wu.shell_ix[0] = EFF59_ID05_Data[mwk->wu.dir_step][0];
        ewk->wu.shell_ix[1] = EFF59_ID05_Data[mwk->wu.dir_step][1];
        ewk->wu.shell_ix[2] = EFF59_ID05_Data[mwk->wu.dir_step][2];
        ewk->wu.shell_ix[3] = EFF59_ID05_Data[mwk->wu.dir_step][3];
    }
}

s32 effect_59_init(WORK_Other* mwk, s16 Synchro_BG, s16 ID, s16 direction) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 59;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2000;
    ewk->wu.my_family = Synchro_BG;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->my_master = (u32*)mwk;
    ewk->wu.char_index = 19;
    ewk->wu.dir_step = ID;
    ewk->wu.dm_vital = ID;
    ewk->wu.direction = direction;
    ewk->wu.vital_new = EFF59_Correct_Data[ID][0];
    ewk->wu.vital_old = EFF59_Correct_Data[ID][1];
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos + ewk->wu.vital_new;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = mwk->wu.xyz[1].disp.pos + ewk->wu.vital_old;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = mwk->wu.xyz[2].disp.pos + ewk->wu.direction;
    ewk->wu.my_col_code = 0x1FF;
    ewk->wu.my_clear_level = 128;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    switch (ID) {
    case 4:
        ewk->wu.my_col_code = 1;
        ewk->wu.my_clear_level = 128;
        ewk->wu.shell_ix[0] = -200;
        ewk->wu.shell_ix[1] = 400;
        ewk->wu.shell_ix[2] = -16;
        ewk->wu.shell_ix[3] = 272;
        break;

    case 5:
        ewk->wu.dm_vital = ID;
        ewk->wu.char_index = 82;
        ewk->wu.dir_step = mwk->wu.dir_step;
        ewk->wu.my_col_code = 1;
        ewk->wu.shell_ix[0] = EFF59_ID05_Data[mwk->wu.dir_step][0];
        ewk->wu.shell_ix[1] = EFF59_ID05_Data[mwk->wu.dir_step][1];
        ewk->wu.shell_ix[2] = EFF59_ID05_Data[mwk->wu.dir_step][2];
        ewk->wu.shell_ix[3] = EFF59_ID05_Data[mwk->wu.dir_step][3];
        break;
    }

    return 0;
}

s32 Check_Break_Into_59(WORK_Other* ewk) {
    if (ewk->wu.dm_vital != 4 || ewk->wu.routine_no[0] == 4) {
        return 0;
    }

    if (Break_Into) {
        ewk->wu.routine_no[0] = 4;
        ewk->wu.my_family = 1;
        ewk->wu.position_z = 8;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        return 1;
    }

    return 0;
}
