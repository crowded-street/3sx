/**
 * @file bg_070.c
 * A Road In Kyoto, Japan
 */

#include "sf33rd/Source/Game/stage/bg070.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff94.h"
#include "sf33rd/Source/Game/effect/effj8.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void BG070() {
    bgw_ptr = &bg_w.bgw[1];
    bg0702();
    bgw_ptr = &bg_w.bgw[0];
    bg0701();
    bgw_ptr = &bg_w.bgw[2];
    bg0704();
    bgw_ptr = &bg_w.bgw[5];
    bg0703();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg0701() {
    void (*bg0701_jmp[2])() = { bg0701_init00, bg0701_move00 };
    bg0701_jmp[bgw_ptr->r_no_0]();
}

void bg0701_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->xy[1].disp.pos = bgw_ptr->wxy[1].disp.pos = bgw_ptr->pos_y_work = 0x10;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0xE3;
}

void bg0701_move00() {
    bg_x_move_check();
    bg_y_move_check();
}

void bg0702() {
    void (*bg0702_jmp[2])() = { bg0702_init00, bg_base_move_common };
    bg0702_jmp[bgw_ptr->r_no_0]();
}

void bg0702_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_06_init();
    effect_94_init(3);
    effect_J8_init();
}

void bg0703() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0 += 1;
        bgw_ptr->fam_no = 5;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->xy[0].disp.low = bgw_ptr->xy[1].disp.low = 0;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->pos_y_work = 0;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xC0;
        bgw_ptr->speed_x = 0xE000;
        bgw_ptr->speed_y = 0xF800;
        sync_fam_set3(5);
        break;

    case 1:
        bg_x_move_check();
        bg_y_move_check();
        sync_fam_set3(5);
        break;
    }
}

void bg0704() {
    void (*bg0704_jmp[2])() = { bg0704_init00, bg0704_move00 };
    bg0704_jmp[bgw_ptr->r_no_0]();
}

void bg0704_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->xy[1].disp.pos = bgw_ptr->wxy[1].disp.pos = bgw_ptr->pos_y_work = 0x10;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
}

void bg0704_move00() {
    bg_x_move_check();
    bg_y_move_check();
}
