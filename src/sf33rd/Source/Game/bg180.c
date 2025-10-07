#include "sf33rd/Source/Game/bg180.h"
#include "common.h"
#include "sf33rd/Source/Game/EFF22.h"
#include "sf33rd/Source/Game/EFF44.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/eff05.h"
#include "sf33rd/Source/Game/eff06.h"
#include "sf33rd/Source/Game/ta_sub.h"

void BG180() {
    bgw_ptr = &bg_w.bgw[1];
    bg1802();
    bgw_ptr = &bg_w.bgw[0];
    bg1801();
    bgw_ptr = &bg_w.bgw[2];
    bg180_sync_common();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg1801() {
    void (*bg1801_jmp[2])() = { bg1801_init00, bg1801_move };
    bg1801_jmp[bgw_ptr->r_no_0]();
}

void bg1801_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

void bg1801_move() {
    bg_x_move_check();
    bg_y_move_check();
}

void bg1802() {
    void (*bg1802_jmp[2])() = { bg1802_init00, bg1802_move };
    bg1802_jmp[bgw_ptr->r_no_0]();
}

void bg1802_init00() {
#if defined(TARGET_PS2)
    s32 effect_22_init();
#endif

    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
    effect_22_init();
    effect_44_init(4);
}

void bg1802_move() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

void bg180_sync_common() {
    void (*bg1800_sync_jmp[2])() = { bg180_sync_init, bg180_sync_move };
    bg1800_sync_jmp[bgw_ptr->r_no_0]();
}

void bg180_sync_init() {
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

void bg180_sync_move() {
    bg_x_move_check();
    bg_y_move_check();
    sync_fam_set3(2);
}
