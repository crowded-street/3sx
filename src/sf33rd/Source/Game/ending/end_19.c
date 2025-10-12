/**
 * @file end_19.c
 * Twelve's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_1900_move();

void end_1900_0();
void end_1900_common();

const s16 timer_19_tbl[6] = { 360, 720, 660, 780, 600, 300 };

const s16 end_19_pos[6][2] = { { 512, 768 }, { 256, 512 }, { 256, 512 }, { 768, 512 }, { 256, 256 }, { 256, 256 } };

void end_19000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x32);
        end_w.timer = timer_19_tbl[end_w.r_no_2];
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

            end_w.timer = timer_19_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_1900_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1900_move() {
    void (*end_1900_move_jp[6])() = { end_1900_0,      end_1900_common, end_1900_common,
                                      end_1900_common, end_1900_common, end_1900_common };
    bgw_ptr = &bg_w.bgw[0];
    end_1900_move_jp[end_w.r_no_2]();
}

void end_1900_0() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_19_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_19_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = 0;
        bgw_ptr->speed_x = 0x10000;
        effect_E6_init(0x84);
        effect_E6_init(0x85);
        effect_E6_init(0x86);
        effect_E6_init(0x87);
        Rewrite_End_Message(1);
        bgw_ptr->free = 0x28;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos < 320) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1400000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 3:
        break;
    }
}

void end_1900_common() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_19_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_19_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 1:
            effect_E6_init(0x88);
            effect_E6_init(0x89);
            effect_E6_init(0x8A);
            effect_E6_init(0x8B);
            effect_E6_init(0x8C);
            effect_E6_init(0x8D);
            Rewrite_End_Message(2);
            break;

        case 2:
            Rewrite_End_Message(3);
            break;

        case 3:
            Rewrite_End_Message(4);
            break;

        case 4:
            effect_E6_init(0x8F);
            Rewrite_End_Message(5);
            break;

        case 5:
            effect_E6_init(0x90);
            Rewrite_End_Message(0);
            end_fade_flag = 1;
            end_fade_timer = timer_19_tbl[end_w.r_no_2] - 120;
            break;
        }

    case 1:
        break;
    }
}
