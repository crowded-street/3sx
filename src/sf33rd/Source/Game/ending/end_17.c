/**
 * @file end_17.c
 * Makoto's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"

void end_1100_move();
void end_1101_move();
void end_1102_move();

void end_1100_common();
void end_1100_3();
void end_1100_6();

void end_1101_2();
void end_1101_3();

const s16 timer_11_tbl[7] = { 660, 480, 120, 180, 480, 1260, 420 };

const s16 end_11_pos[7][2] = { { 256, 768 }, { 768, 768 }, { 768, 768 }, { 256, 768 },
                               { 768, 512 }, { 256, 256 }, { 768, 256 } };

const s16 end_11_115_pos[4][3] = { { 768, 256, 1 }, { 256, 0, 2 }, { 768, 0, 2 }, { 768, 256, 1 } };

void end_17000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x31);
        end_w.timer = timer_11_tbl[end_w.r_no_2];
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 7) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_11_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_1100_move();
        end_1101_move();
        end_1102_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1100_move() {
    void (*end_1100_move_jp[7])() = { end_1100_common, end_1100_common, end_1100_common, end_1100_3,
                                      end_1100_common, end_1100_common, end_1100_6 };
    bgw_ptr = &bg_w.bgw[0];
    end_1100_move_jp[end_w.r_no_2]();
}

void end_1100_common() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_11_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_11_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 0:
            Bg_On_W(1);
            effect_E6_init(0);
            effect_E6_init(1);
            Rewrite_End_Message(1);
            break;

        case 1:
            Bg_Off_W(1);
            Rewrite_End_Message(2);
            break;

        case 2:
            Bg_On_W(1);
            Rewrite_End_Message(3);
            effect_E6_init(0x81);
            effect_E6_init(0x82);
            break;

        case 4:
            Bg_On_W(1);
            Rewrite_End_Message(5);
            break;

        case 5:
            effect_E6_init(3);
            effect_E6_init(4);
            effect_E6_init(5);
            Rewrite_End_Message(6);
            break;
        }

        break;

    case 1:
        break;
    }
}

void end_1100_3() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_Off_W(1);
        bgw_ptr->xy[0].disp.pos = end_11_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_11_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(2);
        effect_E6_init(0x83);
        break;

    case 1:
        bg_w.bgw[0].xy[1].cal += 0xFFFD0000;
        break;

    case 2:
        break;
    }
}

void end_1100_6() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->l_limit = 0;
        bgw_ptr->xy[0].disp.pos = end_11_115_pos[bgw_ptr->l_limit][0];
        bgw_ptr->xy[1].disp.pos = end_11_115_pos[bgw_ptr->l_limit][1];
        bgw_ptr->r_limit = end_11_115_pos[bgw_ptr->l_limit][2];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(6);
        effect_E6_init(8);
        Rewrite_End_Message(7);
        end_fade_flag = 1;
        end_fade_timer = timer_11_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->r_limit--;

        if (bgw_ptr->r_limit <= 0) {
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 4) {
                bgw_ptr->l_limit = 0;
            }

            bgw_ptr->xy[0].disp.pos = end_11_115_pos[bgw_ptr->l_limit][0];
            bgw_ptr->xy[1].disp.pos = end_11_115_pos[bgw_ptr->l_limit][1];
            bgw_ptr->r_limit = end_11_115_pos[bgw_ptr->l_limit][2];
        }

        break;
    }
}

void end_1101_move() {
    void (*end_1101_move_jp[7])() = { end_X_com01, end_X_com01, end_1101_2, end_1101_2,
                                      end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_1101_move_jp[end_w.r_no_2]();
}

void end_1101_2() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = bgw_ptr->abs_x = 512;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 2:
            bgw_ptr->speed_y = 0;

            if (bgw_ptr->fam_no == 1) {
                bgw_ptr->speed_x = -0x8000;
                break;
            }

            bgw_ptr->speed_x = 0x8000;
            break;

        case 3:
            bgw_ptr->speed_x = 0;
            bgw_ptr->speed_y = 0x4000;
            break;
        }

        break;

    case 1:
        bgw_ptr->xy[0].cal += bgw_ptr->speed_x;
        bgw_ptr->xy[1].cal += bgw_ptr->speed_y;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_1102_move() {
    void (*end_1102_move_jp[7])() = { end_X_com01, end_X_com01, end_1101_2, end_1101_3,
                                      end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[2];
    end_1102_move_jp[end_w.r_no_2]();
}

void end_1101_3() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(4);
        bgw_ptr->xy[0].disp.pos = end_11_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_11_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        bg_w.bgw[2].xy[1].cal += -0x30000;
        break;

    case 2:
        break;
    }
}
