#include "sf33rd/Source/Game/bg000.h"
#include "common.h"
#include "sf33rd/Source/Game/EFF44.h"
#include "sf33rd/Source/Game/EFF60.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/eff06.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

#include "sf33rd/Source/Game/animation/appear.h"

void BG000() {
    bgw_ptr = &bg_w.bgw[1];
    bg0001();
    bgw_ptr = &bg_w.bgw[0];
    bg0000();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

void bg0001() {
    void (*bg0002_jmp[3])() = { bg0001_init00, bg0000_demo, bg_base_move_common };
    bg0002_jmp[bgw_ptr->r_no_0]();
}

void bg0001_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x1D0;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
    Gill_Appear_Flag = gill_appear_check();

    if (Gill_Appear_Flag == 0) {
        make_texcash_work(0x10);
        setup_GILL_Opening_Ceremony();
    }

    effect_06_init();
    effect_44_init(7);
    effect_60_init(2);

    if (bg_w.area) {
        bgw_ptr->r_no_0 = 2;
        return;
    } else if (Gill_Appear_Flag) {
        bgw_ptr->r_no_0 = 2;
        return;
    }

    if (plw->player_number == 0) {
        bgw_ptr->u_line = 0;
        bgw_ptr->xy[0].cal += bgw_ptr->speed_x * 0xE0;
        bgw_ptr->old_pos_x = bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        return;
    }

    bgw_ptr->u_line = 1;
    bgw_ptr->xy[0].cal -= bgw_ptr->speed_x * 0xC0;
    bgw_ptr->old_pos_x = bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
}

void bg0000() {
    void (*bg0000_jmp[3])() = { bg0000_init00, bg0000_demo, bg_move_common };
    bg0000_jmp[bgw_ptr->r_no_0]();
}

void bg0000_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x1D0;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    if (Gill_Appear_Flag) {
        bgw_ptr->r_no_0 = 2;
        bg_app = 0;
        return;
    }

    bg_app = 1;

    if (plw->player_number == 0) {
        bgw_ptr->u_line = 0;
        bgw_ptr->xy[0].cal += bgw_ptr->speed_x * 0xE0;
        bgw_ptr->old_pos_x = bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        return;
    }

    bgw_ptr->u_line = 1;
    bgw_ptr->xy[0].cal -= bgw_ptr->speed_x * 0xC0;
    bgw_ptr->old_pos_x = bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
}

void bg0000_demo() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->free = 0x1E;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1 += 1;
        }

        break;

    case 2:
        if (bgw_ptr->u_line) {
            bgw_ptr->wxy[0].cal += bgw_ptr->speed_x;

            if (bgw_ptr->wxy[0].disp.pos > 0x1D0) {
                bgw_ptr->r_no_1 += 1;
                bgw_ptr->wxy[0].disp.pos = 0x1D0;
                bgw_ptr->wxy[0].disp.low = 0;
                bgw_ptr->xy[0].cal = bgw_ptr->wxy[0].cal;
                bgw_ptr->old_pos_x = 0x1D0;
                break;
            }
        } else {
            bgw_ptr->wxy[0].cal -= bgw_ptr->speed_x;

            if (bgw_ptr->wxy[0].disp.pos < 0x1D0) {
                bgw_ptr->r_no_1 += 1;
                bgw_ptr->wxy[0].disp.pos = 0x1D0;
                bgw_ptr->wxy[0].disp.low = 0;
                bgw_ptr->xy[0].cal = bgw_ptr->wxy[0].cal;
                bgw_ptr->old_pos_x = 0x1D0;
                break;
            }
        }
        break;

    case 3:
        bg_app = 0;
        bgw_ptr->r_no_0 = 2;
        break;
    }
}
