#include "sf33rd/Source/Game/bg100.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff29.h"
#include "sf33rd/Source/Game/effect/eff44.h"
#include "sf33rd/Source/Game/ta_sub.h"

void BG100() {
    bgw_ptr = &bg_w.bgw[1];
    bg1001();
    bgw_ptr = &bg_w.bgw[0];
    bg1000();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg1000() {
    void (*bg1000_jmp[2])() = { bg1000_init00, bg_move_common };
    bg1000_jmp[bgw_ptr->r_no_0]();
}

void bg1000_init00() {
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->r_no_0++;
    bgw_ptr->zuubun = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos - bg_w.pos_offset;
    bgw_ptr->hos_xy[0].disp.low = 0;
    effect_29_init();
}

void bg1001() {
    void (*bg1001_jmp[2])() = { bg1001_init00, bg_base_move_common };
    bg1001_jmp[bgw_ptr->r_no_0]();
}

void bg1001_init00() {
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->r_no_0++;
    bgw_ptr->zuubun = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos - bg_w.pos_offset;
    bgw_ptr->hos_xy[0].disp.low = 0;
    bgw_ptr->wxy[1].cal = 0;
    bgw_ptr->xy[1].cal = 0;
    effect_05_init();
    effect_06_init();
    effect_44_init(5);
}
