/**
 * @file eff49.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff49.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

u8 Setup_Char_49(WORK_Other* ewk);

void effect_49_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        if (ewk->wu.dmcal_m == Continue_Count[LOSER]) {
            break;
        }

        ewk->wu.dmcal_m = Continue_Count[LOSER];

        if (Continue_Count[LOSER] < 0) {
            ewk->wu.routine_no[0]++;
        } else {
            SsRequest(0xA7);
            ewk->wu.dir_step = Setup_Char_49(ewk);
            set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        }

        break;

    case 2:
        if (Ck_Range_Out_S(ewk, 1, 64)) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            return;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

s32 effect_49_init(s16 vital_new) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 49;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.direction = 1;
    ewk->wu.vital_new = vital_new;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 84;
    ewk->wu.dmcal_m = Continue_Count[LOSER];
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[1].wxy[1].disp.pos + 8;
    ewk->wu.position_z = 15;

    if (vital_new == 4) {
        ix = Continue_Count[LOSER] & 0xF0;
        ix >>= 4;
        ewk->wu.dir_step = ix;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 450;
    } else {
        ix = Continue_Count[LOSER] & 0xF;
        ewk->wu.dir_step = ix;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 594;
    }

    return 0;
}

u8 Setup_Char_49(WORK_Other* ewk) {
    s16 xx;

    if (ewk->wu.vital_new == 4) {
        xx = Continue_Count[LOSER] & 0xF0;
        return (xx >>= 4);
    }

    return Continue_Count[LOSER] & 0xF;
}
