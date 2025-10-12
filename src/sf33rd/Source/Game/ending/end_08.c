/**
 * @file end_08.c
 * Elena's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_800_move();

void end_8000_0000();
void end_8000_0001();
void end_8000_0002();

const s16 timer_8_tbl[4] = { 840, 480, 900, 360 };

const s16 end_8_pos[4][2] = { { 256, 768 }, { 256, 512 }, { 256, 0 }, { 768, 512 } };

void end_08000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x31);
        end_w.timer = timer_8_tbl[end_w.r_no_2];
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if ((end_w.r_no_2) == 4) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_8_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
        }

        end_800_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_800_move() {
    void (*end_800_jp[4])() = { end_8000_0000, end_8000_0001, end_8000_0002, end_8000_0002 };
    bgw_ptr = &bg_w.bgw[0];
    end_800_jp[end_w.r_no_2]();
}

void end_8000_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        effect_E6_init(0x46);
        bgw_ptr->xy[0].disp.pos = end_8_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_8_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->xy[0].cal += 0x8000;

        if (bgw_ptr->xy[0].disp.pos > 448) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1C00000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_8000_0001() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_8_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_8_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(2);
        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0x8000;

        if (bgw_ptr->xy[1].disp.pos < 416) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].disp.pos = 416;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_8000_0002() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_8_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_8_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 2:
            Rewrite_End_Message(3);
            break;

        case 3:
            effect_E6_init(0x47);
            effect_E6_init(0x48);
            end_fade_flag = 1;
            end_fade_timer = timer_8_tbl[end_w.r_no_2] - 120;
            Rewrite_End_Message(4);
            break;
        }

    case 1:
        break;
    }
}
