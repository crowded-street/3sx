/**
 * @file effc3.c
 * Bridge of Elena's stage from New Generation. It falls when the screen starts to scroll down after the break.
 */

#include "sf33rd/Source/Game/effect/ng/effc3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
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
        if (bg_w.bgw[1].r_no_1 == 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.mvxy.a[1].sp = -0x68000;
            ewk->wu.mvxy.d[1].sp = -0xA80;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            if (bg_w.bgw[1].r_no_1 < 2) {
                add_y_sub_ng(&ewk->wu);
            } else {
                ewk->wu.routine_no[0]++;
            }
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    bg_w.bridge.y = ewk->wu.xyz[1].disp.pos;
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
