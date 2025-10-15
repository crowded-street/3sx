/**
 * @file end_09.c
 * Oro's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

void end_900_move();

void end_900_0000();
void end_900_5000();

const s16 timer_9_tbl[6] = { 300, 480, 120, 540, 780, 600 };

const s16 end_9_pos[6][2] = { { 256, 512 }, { 256, 768 }, { 256, 336 }, { 256, 0 }, { 256, 768 }, { 256, 512 } };

void end_09000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        end_w.timer = timer_9_tbl[end_w.r_no_2];
        BGM_Request(0x32);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 == 6) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_9_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_900_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_900_move() {
    void (*end_900_jp[6])() = { end_900_0000, end_900_0000, end_900_0000, end_900_0000, end_900_0000, end_900_5000 };
    bgw_ptr = &bg_w.bgw[0];
    end_900_jp[end_w.r_no_2]();
}

void end_900_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_9_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_9_pos[end_w.r_no_2][1];

        switch (end_w.r_no_2) {
        case 0:
            Bg_On_W(1);
            bgw_ptr->abs_x = 512;
            bgw_ptr->abs_y = 0;
            effect_E6_init(0x24);
            Rewrite_End_Message(1);
            break;

        case 1:
            Rewrite_End_Message(2);
            effect_E6_init(0x25);
            break;

        case 2:
            Rewrite_End_Message(3);
            break;

        case 3:
            effect_E6_init(0x26);
            effect_E6_init(0x27);
            effect_E6_init(0x28);
            effect_E6_init(0x29);
            Rewrite_End_Message(4);
            break;

        case 4:
            Rewrite_End_Message(5);
            effect_E6_init(0x25);
            break;
        }

        break;

    case 1:
        break;
    }
}

void end_900_5000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        effect_E6_init(0x2A);
        bgw_ptr->free = 0x1E;
        bgw_ptr->xy[0].disp.pos = end_9_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_9_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(6);
        end_fade_flag = 1;
        end_fade_timer = timer_9_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= 0x8000;

        if (bgw_ptr->xy[1].disp.pos < 337) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1500000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}
