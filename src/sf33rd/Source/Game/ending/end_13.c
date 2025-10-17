/**
 * @file end_13.c
 * Urien's Ending
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

void end_d00_move();
void end_d01_move();

void end_d00_1000();
void end_d00_2000();
void end_d00_3000();
void end_d00_4000();
void end_d00_6000();
void end_d00_7000();

void end_d01_4000();

const s16 timer_d_tbl[8] = { 120, 240, 600, 780, 600, 420, 360, 660 };

const s16 end_d_pos[8][2] = { { 256, 0 },   { 256, 768 }, { 256, 768 }, { 256, 512 },
                              { 192, 256 }, { 256, 512 }, { 256, 0 },   { 256, 0 } };

void end_13000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_d_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x32);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 == 1) {
                effect_E6_init(0x32);
                effect_E6_init(0x33);
            }

            if (end_w.r_no_2 >= 8) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_d_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_d00_move();
        end_d01_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_d00_move() {
    void (*end_d00_jp[8])() = { end_d00_1000, end_d00_1000, end_d00_2000, end_d00_3000,
                                end_d00_4000, end_d00_1000, end_d00_6000, end_d00_7000 };
    bgw_ptr = &bg_w.bgw[0];
    end_d00_jp[end_w.r_no_2]();
}

void end_d00_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_d_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_d_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 0:
            break;

        case 1:
            effect_E6_init(0x34);
            break;

        case 5:
            Bg_Off_W(1);
            Rewrite_End_Message(3);
            break;
        }

        break;

    case 1:
        break;
    }
}

void end_d00_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        if (Request_Fade(3)) {
            end_no_cut = 1;
            bgw_ptr->r_no_1++;
        }

        break;

    case 1:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 20;
            overwrite_panel(0xFFFFFFFF, 0x17);
            break;
        }

        break;

    default:
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_d00_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->xy[0].disp.pos = end_d_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_d_pos[end_w.r_no_2][1];
        effect_E6_init(0x35);
        /* fallthrough */

    case 1:
        overwrite_panel(0xFFFFFFFF, 0x17);

        if (Request_Fade(2)) {
            end_no_cut = 1;
            bgw_ptr->r_no_1++;
            Rewrite_End_Message(1);
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
        }

        break;
    }
}

void end_d00_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_d_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_d_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        Rewrite_End_Message(2);
        effect_E6_init(0x36);
        effect_E6_init(0x37);
        break;

    case 1:
        bgw_ptr->xy[0].cal += 0x8000;

        if (bgw_ptr->xy[0].disp.pos > 304) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1300000;
            bgw_ptr->free = 10;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_d00_6000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_d_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_d_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x4A);
        bgw_ptr->free = 0x3C;
        Rewrite_End_Message(0);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            end_etc_flag = 0;
            effect_E6_init(0x4B);
            bgw_ptr->free = 0;
        }

        break;

    case 2:
        if (!end_etc_flag) {
            bgw_ptr->free++;
            bgw_ptr->free &= 1;

            if (bgw_ptr->free) {
                bgw_ptr->xy[1].disp.pos += 8;
                bgw_ptr->abs_y += 8;
                break;
            }

            bgw_ptr->xy[1].disp.pos -= 8;
            bgw_ptr->abs_y -= 8;
            break;
        }

        bgw_ptr->r_no_1++;
        end_w.timer = 30;
        break;

    case 3:
        bgw_ptr->free++;
        bgw_ptr->free &= 1;

        if (bgw_ptr->free) {
            bgw_ptr->xy[1].disp.pos += 8;
            bgw_ptr->abs_y += 8;
            break;
        }

        bgw_ptr->xy[1].disp.pos -= 8;
        bgw_ptr->abs_y -= 8;
        break;
    }
}

void end_d00_7000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_d_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_d_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(4);
        end_fade_flag = 1;
        end_fade_timer = timer_d_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        break;
    }
}

void end_d01_move() {
    void (*end_d01_jp[8])() = { end_X_com01,  end_X_com01, end_X_com01, end_X_com01,
                                end_d01_4000, end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_d01_jp[end_w.r_no_2]();
}

void end_d01_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        break;
    }
}
