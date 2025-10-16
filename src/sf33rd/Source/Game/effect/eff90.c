/**
 * @file eff90.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff90.h"
#include "common.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"

s32 Check_Disp_90(WORK_Other_CONN* ewk);
s16 Check_Disp_Pos_90(WORK_Other_CONN* ewk);

void effect_90_move(WORK_Other_CONN* ewk) {
    Check_Pos_OBJ2((WORK_Other*)ewk);

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->master_priority == 0xFF) {
        ewk->wu.my_clear_level = 0;
    } else if (Menu_Cursor_Y[1] == ewk->master_priority) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 0x80;
    }

    ewk->wu.disp_flag = Check_Disp_90(ewk);
    ewk->wu.position_y = Check_Disp_Pos_90(ewk);
    sort_push_request3(&ewk->wu);
}

s32 Check_Disp_90(WORK_Other_CONN* ewk) {
    if (ewk->wu.dir_step == 2 || ewk->wu.dir_step == 6) {
        return 1;
    }

    if (ewk->master_priority < Cursor_Limit[0]) {
        return 0;
    }

    if (ewk->master_priority > Cursor_Limit[0] + 3) {
        return 0;
    }

    return 1;
}

s16 Check_Disp_Pos_90(WORK_Other_CONN* ewk) {
    s16 y;

    if (ewk->wu.dir_step == 2 || ewk->wu.dir_step == 6) {
        return ewk->wu.position_y;
    }

    y = (bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 158) - (ewk->master_priority * 20);
    y += Cursor_Limit[0] * 20;

    return y;
}
