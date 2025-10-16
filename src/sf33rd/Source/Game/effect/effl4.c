/**
 * @file effl4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl4.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 effl4_data_tbl[24] = { 143, 96,  73, 9,  223, 120, 73, 10, 303, 104, 73, 11,
                                 367, 112, 73, 12, 399, 144, 73, 13, 487, 120, 73, 14 };

void effect_L4_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        if (!obr_no_disp_check() && range_x_check3(ewk, 208)) {
            ewk->wu.position_x = (ewk->wu.xyz[0].disp.pos & 0xFFFF) + 1;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
            sort_push_request4(&ewk->wu);
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_L4_init() {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = effl4_data_tbl;

    for (i = 0; i < 6; i++) {
        if ((ix = pull_effect_work(3)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 214;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 300;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.position_z = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.char_table[0] = _frc_char_table;
    }

    return 0;
}
