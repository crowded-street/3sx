#include "sf33rd/Source/Game/EFFH9.h"
#include "common.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/workuser.h"

const CONN bbbs_ball[4][3] = {
    { { 153, 0, 0, 32464 }, { 142, 0, 0, 32464 }, { 172, 8, 0, 32488 } },
    { { -141, 0, 0, 32464 }, { -152, 0, 0, 32464 }, { -174, 8, 0, 32488 } },
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

void effH9_trans(WORK* ewk);
void nokori_ball_effH9(WORK_Other_CONN* ewk, s16 num);

void effect_H9_move(WORK_Other_CONN* ewk) {
#if defined(TARGET_PS2)
    void nokori_ball_effH9(WORK_Other_CONN * ewk, s32 num);
#endif

    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_cgnum = 0;
            ewk->wu.position_z = ewk->wu.my_priority = 9;
            ewk->wu.direction = 0;
            ewk->wu.dir_timer = 0;
            nokori_ball_effH9(ewk, ewk->wu.direction);
            break;

        case 1:
            if (Game_pause || EXE_flag) {
                break;
            }

            if (--ewk->wu.dir_timer > 0) {
                break;
            }

            ewk->wu.dir_timer = 3;
            ewk->wu.direction++;
            nokori_ball_effH9(ewk, ewk->wu.direction);

            if (ewk->wu.direction >= Bonus_Game_Work) {
                ewk->wu.routine_no[0] = 1;
                ewk->wu.routine_no[1] = 0;
            }

            break;
        }

        effH9_trans(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        nokori_ball_effH9(ewk, Bonus_Game_Work);
        effH9_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effH9_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[2].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[2].wxy[1].disp.pos;
    sort_push_request3(ewk);
}

void nokori_ball_effH9(WORK_Other_CONN* ewk, s16 num) {
    ewk->conn[0].chr = (num % 10) + 32464;
    ewk->conn[1].chr = (num / 10) + 32464;
}

s32 effect_H9_init(PLW* wk) {
    WORK_Other_CONN* ewk;
    s16 ix;
    s16 i;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 179;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = 3;
    ewk->wu.cgromtype = 1;
    ewk->wu.type = wk->wu.rl_flag;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 73;
    ewk->num_of_conn = 3;

    if (wk->wu.rl_flag) {
        ix = 1;
    } else {
        ix = 0;
    }

    for (i = 0; i < 3; i++) {
        ewk->conn[i] = bbbs_ball[ix][i];
    }

    return 0;
}
