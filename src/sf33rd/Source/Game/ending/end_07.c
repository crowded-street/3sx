/**
 * @file end_07.c
 * Ibuki's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFB0.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_700_move();
void end_701_move();

void end_700_0000();
void end_700_2000();

const s16 timer_7_tbl[4] = { 480, 840, 840, 960 };

const s16 end_7_pos[4][2] = { { 256, 768 }, { 768, 768 }, { 768, 512 }, { 256, 256 } };

void end_07000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_7_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x31);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 4) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_7_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_700_move();
        end_701_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_700_move() {
    void (*end_700_jp[4])() = { end_700_0000, end_700_0000, end_700_2000, end_700_0000 };
    bgw_ptr = &bg_w.bgw[0];
    end_700_jp[end_w.r_no_2]();
}

void end_701_move() {
    void (*end_701_jp[4])() = { end_X_com01, end_700_0000, end_X_com01, end_700_0000 };
    bgw_ptr = &bg_w.bgw[1];
    end_701_jp[end_w.r_no_2]();
}

void end_700_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_7_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_7_pos[end_w.r_no_2][1];

        if (bgw_ptr->fam_no == 0) {
            switch (end_w.r_no_2) {
            case 0:
                effect_E6_init(0x5B);
                Rewrite_End_Message(1);
                break;

            case 1:
                effect_B0_init();
                Rewrite_End_Message(2);
                break;

            case 3:
                bgw_ptr->abs_x = 512;
                effect_E6_init(0x5A);
                Rewrite_End_Message(4);
                end_fade_flag = 1;
                end_fade_timer = timer_7_tbl[end_w.r_no_2] - 120;
                break;
            }
        }

        break;

    case 1:
        break;
    }
}

void end_700_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_7_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_7_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        effect_E6_init(0x59);
        Rewrite_End_Message(3);
        break;

    case 1:
        bgw_ptr->xy[0].cal -= 0x8000;

        if (bgw_ptr->xy[0].disp.pos < 289) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1200000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;
    }
}
