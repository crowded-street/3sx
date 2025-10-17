/**
 * @file end_16.c
 * Chun-Li's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/sys_sub.h"

void end_1600_move();

void end_1600_0000();
void end_1600_1000();
void end_1600_2000();
void end_1600_3000();
void end_1600_3100();
void end_1600_5000();

void end_16_col_change();

const s16 timer_16_tbl[6] = { 660, 360, 360, 660, 660, 240 };

const s16 end_16_pos[7][2] = { { 256, 768 }, { 256, 768 }, { 768, 256 }, { 256, 256 },
                               { 256, 0 },   { 256, 0 },   { 768, 0 } };

void end_16000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        c_kakikae = 0;
        c_number = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x31);
        end_w.timer = timer_16_tbl[end_w.r_no_2];
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 6) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_16_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
        }

        end_1600_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1600_move() {
    void (*end_1600_move_jp[6])() = { end_1600_0000, end_1600_1000, end_1600_2000,
                                      end_1600_3000, end_1600_3100, end_1600_5000 };
    bgw_ptr = &bg_w.bgw[0];
    end_1600_move_jp[end_w.r_no_2]();
}

void end_1600_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->xy[0].cal += 0x8000;

        if (bgw_ptr->xy[0].disp.pos >= 480) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1E00000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_1600_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        /* fallthrough */

    case 1:
        if (Request_Fade(1)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 10;
            overwrite_panel(0xFF000000, 0x17);
        }

        break;

    case 3:
        overwrite_panel(0xFF000000, 0x17);
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            end_w.timer = 0;
        }

        break;

    default:
        overwrite_panel(0xFF000000, 0x17);
        break;
    }
}

void end_1600_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFF000000, 0x17);
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        /* fallthrough */

    case 1:
        overwrite_panel(0xFF000000, 0x17);

        if (Request_Fade(0)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
            Rewrite_End_Message(2);
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
        }

        break;

    case 3:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bgw_ptr->xy[1].disp.pos >= 384) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1800000;
        }

        break;
    }
}

void end_1600_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        effect_E6_init(0x56);
        Rewrite_End_Message(3);
        bgw_ptr->l_limit2 = 2;
        bgw_ptr->l_limit = 0;
        c_kakikae = 1;
        break;

    case 1:
        end_16_col_change();
        break;
    }
}

void end_1600_3100() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        effect_E6_init(0x57);
        Rewrite_End_Message(4);
        c_kakikae = 2;
        break;

    case 1:
        end_16_col_change();
        break;
    }
}

void end_1600_5000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        c_kakikae = 0;
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        effect_E6_init(0x58);
        Rewrite_End_Message(5);
        bgw_ptr->free = 2;
        bgw_ptr->rewrite_flag = 0;
        bgw_ptr->l_limit2 = 2;
        bgw_ptr->l_limit = 0;
        end_fade_flag = 1;
        end_fade_timer = timer_16_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->free = 2;
            bgw_ptr->rewrite_flag ^= 1;

            if (bgw_ptr->rewrite_flag) {
                bgw_ptr->xy[0].disp.pos = 768;
                break;
            }

            bgw_ptr->xy[0].disp.pos = 256;
        }

        break;
    }
}

void end_16_col_change() {
    bgw_ptr->l_limit2--;

    if (bgw_ptr->l_limit2 <= 0) {
        bgw_ptr->l_limit2 = 1;
        bgw_ptr->l_limit++;
        bgw_ptr->l_limit &= 1;
        c_number = bgw_ptr->l_limit;
    }
}
