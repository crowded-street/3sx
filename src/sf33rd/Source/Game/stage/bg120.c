/**
 * @file bg_120.c
 * Santos Harbor, Brazil
 */

#include "sf33rd/Source/Game/stage/bg120.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff12.h"
#include "sf33rd/Source/Game/effect/eff55.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void BG120() {
    bgw_ptr = &bg_w.bgw[1];
    bg1202();
    bgw_ptr = &bg_w.bgw[0];
    bg1201();
    bgw_ptr = &bg_w.bgw[2];
    bg_fam0C00();
    bgw_ptr = &bg_w.bgw[6];
    bg_fam0C00();
    bgw_ptr = &bg_w.bgw[5];
    bg_fam0C00();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg1201() {
    void (*bg1201_jmp[2])() = { bg1201_init00, bg_move_common };
    bg1201_jmp[bgw_ptr->r_no_0]();
}

void bg1201_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg1202() {
    void (*bg1202_jmp[2])() = { bg1202_init00, bg_base_move_common };
    bg1202_jmp[bgw_ptr->r_no_0]();
}

void bg1202_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
    effect_55_init();
    effect_12_init(2);
}

void bg_fam0C00() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->pos_y_work = 0;

        switch (bgw_ptr->fam_no) {
        case 2:
            bgw_ptr->speed_x = 0xC000;
            bgw_ptr->speed_y = 0x10000;
            break;
        case 6:
            bgw_ptr->speed_x = 0x9000;
            bgw_ptr->speed_y = 0x10000;
            break;
        default:
            bgw_ptr->speed_x = 0x12000;
            bgw_ptr->speed_y = 0x10000;
            break;
        }

        bgw_ptr->xy[0].disp.low = bgw_ptr->xy[1].disp.low = 0;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xF0;
        sync_fam_set3(bgw_ptr->fam_no);
        break;

    default:
        // Do nothing

    case 1:
        bg_x_move_check();
        bg_y_move_check();
        sync_fam_set3(bgw_ptr->fam_no);
        break;
    }
}
