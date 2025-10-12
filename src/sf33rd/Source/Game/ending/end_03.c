/**
 * @file end_03.c
 * Yun's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/sc_sub.h"

#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"

void end_300_move();
void end_301_move();

void end_300_0000();
void end_300_0002();
void end_300_0003();
void end_300_0004();
void end_300_0005();

void end_301_0003();

const s16 end_3_pos[6][2] = { { 256, 768 }, { 768, 768 }, { 256, 512 }, { 768, 544 }, { 256, 256 }, { 768, 256 } };

const s16 timer_3_tbl[6] = { 660, 420, 1080, 900, 720, 1020 };

void end_03000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        setup_dma_group(0, 1);
        end_w.timer = timer_3_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x32);
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

            end_w.timer = timer_3_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_300_move();
        end_301_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_300_move() {
    void (*end_300_jp[6])() = { end_300_0000, end_300_0000, end_300_0002, end_300_0003, end_300_0004, end_300_0005 };
    bgw_ptr = &bg_w.bgw[0];
    end_300_jp[end_w.r_no_2]();
}

void end_300_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 0:
            Bg_On_W(1);
            effect_E6_init(0x9A);
            Rewrite_End_Message(1);
            break;

        case 1:
            effect_E6_init(0x9B);
            Rewrite_End_Message(2);
            break;
        }
    }
}

void end_300_0002() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(3);
        bgw_ptr->free = 0x258;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        if (Request_Fade(3)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 3:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 20;
            overwrite_panel(0xFFFFFFFF, 0x17);
        }

        break;

    case 4:
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_300_0003() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x9D);
        effect_E6_init(0xB3);
        Rewrite_End_Message(4);
        /* fallthrough */

    case 1:
        if (Request_Fade(2)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
        }

        break;

    case 3:
        bgw_ptr->xy[1].cal -= 0x4000;

        if (bgw_ptr->xy[1].disp.pos < 449) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1C00000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_300_0004() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x9E);
        end_etc_flag = 0;
        effect_E6_init(0x9F);
        Rewrite_End_Message(5);
        bgw_ptr->free = 0x21C;
        bgw_ptr->l_limit = -1;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->free = 0x10;
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 3) {
                bgw_ptr->r_no_1++;
                end_w.timer = 50;
                end_etc_flag = 1;
            }
        }

        break;
    }
}

void end_300_0005() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0xA0);
        Rewrite_End_Message(6);
        end_fade_flag = 1;
        end_fade_timer = timer_3_tbl[end_w.r_no_2] - 120;
    }
}

void end_301_move() {
    void (*end_301_jp[6])() = { end_X_com01, end_X_com01, end_X_com01, end_301_0003, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_301_jp[end_w.r_no_2]();
}

void end_301_0003() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_3_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_3_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
    }
}
