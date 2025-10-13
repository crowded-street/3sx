#include "sf33rd/Source/Game/bns_bg2.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff12.h"
#include "sf33rd/Source/Game/effect/eff35.h"
#include "sf33rd/Source/Game/workuser.h"

void Bonus_bg2() {
    bgw_ptr = &bg_w.bgw[1];
    bns11();
    bgw_ptr = &bg_w.bgw[0];
    bns12();
    bgw_ptr = &bg_w.bgw[2];
    bns13();
    bg_pos_hosei_sub3(2);
    Bg_Family_Set_appoint(2);
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bns11() {
    void (*bns11_jmp[2])() = { bns11_init00, bns11_move };
    bns11_jmp[bgw_ptr->r_no_0]();
}

void bns11_init00() {
    bgw_ptr->r_no_0++;

    if (Player_id == 0) {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = 0x223;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = 0x1EB;
    }

    bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    bg_app = 0;
    effect_05_init();
    effect_12_init(4);

    if (!plw->wu.operator) {
        effect_35_init(2, 1);
    } else {
        effect_35_init(2, 0);
    }

    effect_35_init(0xC, 2);
    effect_35_init(0x12, 3);
    effect_35_init(2, 4);
}

void bns11_move() {
    bg_base_x_move_check();
    bg_base_y_move_check();
}

void bns12() {
    void (*bns12_jmp[2])() = { bns12_init00, bns12_move };
    bns12_jmp[bgw_ptr->r_no_0]();
}

void bns12_init00() {
    bgw_ptr->r_no_0++;

    if (Player_id == 0) {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = 0x211;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = 0x1F5;
    }

    bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bns12_move() {
    bg_x_move_check();
    bg_y_move_check();
}

void bns13() {
    void (*bns13_jmp[2])() = { bns13_init00, bns13_move };
    bns13_jmp[bgw_ptr->r_no_0]();
}

void bns13_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bns13_move() {
    // Do nothing
}
