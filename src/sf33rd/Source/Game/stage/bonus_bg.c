/**
 * @file bonus_bg.c
 * Basketball Minigame Stage
 */

#include "sf33rd/Source/Game/stage/bonus_bg.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void Bonus_bg() {
    bgw_ptr = &bg_w.bgw[1];
    bns02();
    bgw_ptr = &bg_w.bgw[0];
    bns01();
    bgw_ptr = &bg_w.bgw[2];
    bns03();
    bg_pos_hosei_sub3(2);
    Bg_Family_Set_appoint(2);
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bns01() {
    void (*bns01_jmp[2])() = { bns01_init00, bns01_move };
    bns01_jmp[bgw_ptr->r_no_0]();
}

void bns01_init00() {
    bgw_ptr->r_no_0++;

    if (Player_id == 0) {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x1C5;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x209;
    }

    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    bg_app = 0;
    effect_05_init();
}

void bns01_move() {
    bg_base_x_move_check();
    bg_base_y_move_check();
}

void bns02() {
    void (*bns02_jmp[2])() = { bns02_init00, bns02_move };
    bns02_jmp[bgw_ptr->r_no_0]();
}

void bns02_init00() {
    bgw_ptr->r_no_0++;

    if (Player_id == 0) {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x1C5;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x209;
    }

    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bns02_move() {
    bg_x_move_check();
    bg_y_move_check();
}

void bns03() {
    void (*bns03_jmp[2])() = { bns03_init00, bns03_move };
    bns03_jmp[bgw_ptr->r_no_0]();
}

void bns03_init00() {
    bgw_ptr->r_no_0++;

    if (Player_id == 0) {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x1C4;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x20D;
    }

    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bns03_move() {
    // Do nothing
}
