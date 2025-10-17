/**
 * @file bg_050.c
 * Mosque, Russia
 */

#include "sf33rd/Source/Game/stage/bg050.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void BG050() {
    bgw_ptr = &bg_w.bgw[1];
    bg0502();
    bgw_ptr = &bg_w.bgw[0];
    bg0501();
    bgw_ptr = &bg_w.bgw[2];
    bg050_sync_common();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg0501() {
    void (*bg0501_jmp[2])() = { bg0501_init00, bg_move_common };
    bg0501_jmp[bgw_ptr->r_no_0]();
}

void bg0501_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg0502() {
    void (*bg0502_jmp[2])() = { bg0502_init00, bg_base_move_common };
    bg0502_jmp[bgw_ptr->r_no_0]();
}

void bg0502_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
}

void bg050_sync_common() {
    void (*bg050_sync_jmp[2])() = { bg050_sync_init, bg050_sync_move };
    bg050_sync_jmp[bgw_ptr->r_no_0]();
}

void bg050_sync_init() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xF0;
    bgw_ptr->pos_y_work = 0;
    bgw_ptr->xy[1].disp.pos = 0;
    bgw_ptr->speed_x = 0xE000;
    bgw_ptr->speed_y = 0xE000;
    sync_fam_set3(2);
}

void bg050_sync_move() {
    bg_x_move_check();
    bg_y_move_check();
    sync_fam_set3(2);
}
