/**
 * @file effe1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"

void effect_E1_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        /* fallthrough */

    case 1:
        ewk->wu.my_mr.size.x = 63;
        ewk->wu.my_mr.size.y = 63;
        break;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

s32 effect_E1_init(s16 id, s16 Time, s16 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0x8D;
    ewk->wu.work_id = 0x10;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 1;
    ewk->wu.dir_timer = Time;
    ewk->wu.char_table[0] = _etc_char_table;
    ewk->wu.my_mr.size.x = 0x3F;
    ewk->wu.my_mr.size.y = 0x3F;

    switch (id) {
    case 0:
        ewk->wu.char_index = 0x59;
        ewk->wu.xyz[0].disp.pos = 0x200;
        ewk->wu.xyz[1].disp.pos = 0x90;
        ewk->wu.my_priority = ewk->wu.position_z = 0x45;
        break;

    case 1:
        ewk->wu.my_priority = ewk->wu.position_z = 0x44;
        ewk->wu.xyz[0].disp.pos = 0x200;
        ewk->wu.xyz[1].disp.pos = 0x90;
        ewk->wu.char_index = 0x5A;
        break;
    }

    return 0;
}
