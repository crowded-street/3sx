/**
 * @file eff65.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff65.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

void effect_65_move(WORK_Other* ewk) {
    ewk->wu.my_clear_level = 0x80;

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (vm_w.Connect[ewk->wu.type] == 0) {
            set_char_move_init(&ewk->wu, 0, 0x46);
            break;
        }

        ewk->wu.routine_no[0] = 1;
        /* fallthrough */

    case 1:
        if (vm_w.Connect[ewk->wu.type] == 0) {
            set_char_move_init(&ewk->wu, 0, 0x46);
        } else if (ewk->wu.type == Menu_Cursor_X[0]) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.my_clear_level = 0;
            set_char_move_init2(&ewk->wu, 0, 0x46, 3, 0);
        } else {
            set_char_move_init2(&ewk->wu, 0, 0x46, 2, 0);
        }

        break;

    default:
        if (vm_w.Connect[ewk->wu.type] == 0) {
            ewk->wu.routine_no[0] = 0;
            set_char_move_init(&ewk->wu, 0, 0x46);
        } else if (ewk->wu.type != Menu_Cursor_X[0]) {
            ewk->wu.routine_no[0] = 1;
            set_char_move_init2(&ewk->wu, 0, 0x46, 2, 0);
        } else {
            ewk->wu.my_clear_level = 0;
            char_move(&ewk->wu);
        }

        break;
    }

    sort_push_request4(&ewk->wu);
}
