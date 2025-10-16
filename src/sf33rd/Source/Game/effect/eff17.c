/**
 * @file eff17.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff17.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"

void effect_17_move(WORK_Other* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
    } else if (Menu_Cursor_Y[0] == ewk->wu.type) {
        EFF17_Bowan(ewk);
    } else {
        ewk->wu.my_clear_level = 128;
        ewk->wu.routine_no[1] = 0;
    }

    sort_push_request4(&ewk->wu);
}

void EFF17_Bowan(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.my_clear_level -= 4;

        if (ewk->wu.my_clear_level < 20) {
            ewk->wu.routine_no[1]++;
            ewk->wu.my_clear_level = 20;
        }

        break;

    case 1:
        ewk->wu.my_clear_level += 4;

        if (ewk->wu.my_clear_level > 127) {
            ewk->wu.routine_no[1]--;
            ewk->wu.my_clear_level = 127;
        }

        break;
    }
}
