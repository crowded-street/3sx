/**
 * @file bg_160.c
 * The Dojo Of Rindo-kan, Japan
 */

#include "sf33rd/Source/Game/stage/bg160.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff19.h"
#include "sf33rd/Source/Game/effect/eff44.h"
#include "sf33rd/Source/Game/effect/eff94.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void BG160() {
    bgw_ptr = &bg_w.bgw[1];
    bg1602();
    bgw_ptr = &bg_w.bgw[0];
    bg1601();
    bgw_ptr = &bg_w.bgw[2];
    bg1602_sync_common();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg1601() {
    void (*bg1601_jmp[2])() = { bg1601_init00, bg_move_common };
    bg1601_jmp[bgw_ptr->r_no_0]();
}

void bg1601_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg1602() {
    void (*bg1602_jmp[2])() = { bg1602_init00, bg_base_move_common };
    bg1602_jmp[bgw_ptr->r_no_0]();
}

void bg1602_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
    effect_44_init(2);
    effect_19_init();
    effect_94_init(2);
}

void bg1602_sync_common() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->fam_no = 2;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->zuubun = 0;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xF0;
        bgw_ptr->pos_y_work = 0;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->speed_x = 0x12000;
        bgw_ptr->speed_y = 0x10000;
        sync_fam_set3(bgw_ptr->fam_no);
        break;

    case 1:
        bg_x_move_check();
        bg_y_move_check();
        sync_fam_set3(bgw_ptr->fam_no);
        break;
    }
}
