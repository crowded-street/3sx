/**
 * @file effa1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_A1_move(WORK_Other* ewk) {
    s16 x;

    ewk->wu.disp_flag = 0;

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.be_flag == 0) {
        return;
    }

    ewk->wu.my_clear_level = Flash_Synchro;

    if (ewk->wu.type) {
        x = Cursor_Limit[1] - Cursor_Limit[0];

        if (x > 4) {
            ewk->wu.disp_flag = 1;
        }
    } else if (Cursor_Limit[0] > 0) {
        ewk->wu.disp_flag = 1;
    }

    sort_push_request4(&ewk->wu);
}
