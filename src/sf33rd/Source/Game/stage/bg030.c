/**
 * @file bg_030.c
 * Shopping District, Hong Kong
 */

#include "sf33rd/Source/Game/stage/bg030.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff71.h"
#include "sf33rd/Source/Game/effect/effl2.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void BG030() {
    bgw_ptr = &bg_w.bgw[1];
    bg0301();
    bgw_ptr = &bg_w.bgw[0];
    bg0300();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg0300() {
    void (*bg0300_jmp[2])() = { bg0300_init00, bg_move_common };
    bg0300_jmp[bgw_ptr->r_no_0]();
}

void bg0300_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg0301() {
    void (*bg0301_jmp[2])() = { bg0301_init00, bg_base_move_common };
    bg0301_jmp[bgw_ptr->r_no_0]();
}

void bg0301_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
    effect_71_init();
    effect_L2_init();
}
