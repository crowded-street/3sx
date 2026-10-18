/**
 * @file effc3.c
 * Bridge of Elena's stage from New Generation.
 *
 * FIXME: NG also keeps the bridge's y in 0x0201c258 for the players, falls with the bridge when it breaks (states 2-3
 * of variant 0) and has variant 1, which drops the bridge in after the players in the second area.
 */

#include "sf33rd/Source/Game/effect/ng/effc3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x212C // NG 0x2040
#define BRIDGE_X 512
#define BRIDGE_Y 40

void effect_ngc3_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = BRIDGE_X;
        ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = BRIDGE_Y;
        break;

    case 1:
        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ngc3_init(u8 variant) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NGC3_ID;
    ewk->wu.type = variant;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 0;
    ewk->wu.charset_id = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.my_priority = ewk->wu.position_z = 76;
    ewk->wu.char_index = 0;
    ewk->wu.xyz[0].disp.low = 0;
    ewk->wu.xyz[1].disp.low = 0;
    ewk->wu.char_table[0] = _ng_afc_a_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
