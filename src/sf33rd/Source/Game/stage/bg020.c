/**
 * @file bg_020.c
 * Suzaku Castle Rooftop, Japan
 */

#include "sf33rd/Source/Game/stage/bg020.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff78.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void BG020() {
    bgw_ptr = &bg_w.bgw[1];
    bg0202();
    bgw_ptr = &bg_w.bgw[0];
    bg0201();
    bgw_ptr = &bg_w.bgw[2];
    bg0201();
    bgw_ptr = &bg_w.bgw[5];
    bg020_sync_common();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set_appoint(1);
    Bg_Family_Set_2_appoint(0);
    Bg_Family_Set_appoint(2);
}

void bg0201() {
    void (*bg0201_jmp[2])() = { bg0201_init00, bg_move_common };
    bg0201_jmp[bgw_ptr->r_no_0]();
}

void bg0201_init00() {
    bgw_ptr->r_no_0 += 1;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg0202() {
    void (*bg0202_jmp[2])() = { bg0202_init00, bg_base_move_common };
    bg0202_jmp[bgw_ptr->r_no_0]();
}

void bg0202_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_06_init();
    effect_78_init();
}

void bg020_sync_common() {
    void (*bg020_sync_jmp[2])() = { bg020_sync_init, bg020_sync_move };
    bg020_sync_jmp[bgw_ptr->r_no_0]();
}

void bg020_sync_init() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xF0;
    bgw_ptr->pos_y_work = 0;
    bgw_ptr->xy[1].disp.pos = 0;
    bgw_ptr->speed_x = 0xE000;
    bgw_ptr->speed_y = 0xE000;
    sync_fam_set3(5);
}

void bg020_sync_move() {
    bg_x_move_check();
    bg_y_move_check();
    sync_fam_set3(5);
}
