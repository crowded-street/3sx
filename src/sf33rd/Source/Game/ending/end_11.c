/**
 * @file end_11.c
 * Ken's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_b00_move();
void end_b01_move();

void end_b00_0000();
void end_b00_1000();
void end_b00_3000();

void end_b01_0000();
void end_b01_3000();

const s16 timer_b_tbl[7] = { 240, 480, 180, 420, 600, 1080, 720 };

const s16 end_b_pos[7][2] = { { 256, 288 }, { 256, 768 }, { 768, 288 }, { 768, 768 },
                              { 256, 512 }, { 768, 512 }, { 768, 512 } };

void end_11000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x31);
        end_w.timer = timer_b_tbl[end_w.r_no_2];
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

            end_w.timer = timer_b_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        if (bg_w.quake_y_index > 0) {
            bg_w.quake_y_index--;
        }

        end_b00_move();
        end_b01_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_b00_move() {
    void (*end_b00_jp[7])() = { end_b00_0000, end_b00_1000, end_b00_0000, end_b00_3000,
                                end_b00_0000, end_b00_0000, end_b00_0000 };
    bgw_ptr = &bg_w.bgw[0];
    end_b00_jp[end_w.r_no_2]();
}

void end_b00_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_b_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;

        switch (end_w.r_no_2) {
        case 0:
            Bg_Off_W(1);
            effect_E6_init(0x60);
            Rewrite_End_Message(1);
            break;

        case 2:
            Bg_Off_W(1);
            effect_E6_init(0x62);
            Rewrite_End_Message(0);
            break;

        case 4:
            effect_E6_init(0x65);
            effect_E6_init(0x66);
            effect_E6_init(0x67);
            effect_E6_init(0x68);
            Rewrite_End_Message(3);
            break;

        case 5:
            effect_E6_init(0x69);
            effect_E6_init(0x6A);
            effect_E6_init(0x6D);
            effect_E6_init(0x6E);
            effect_E6_init(0x6B);
            Rewrite_End_Message(4);
            break;

        case 6:
            effect_E6_init(0x6C);
            Rewrite_End_Message(5);
            end_fade_flag = 1;
            end_fade_timer = timer_b_tbl[end_w.r_no_2] - 120;
            break;
        }

        break;

    case 1:
        break;
    }
}

void end_b00_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_b_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        end_etc_flag = 0;
        effect_E6_init(0x61);
        Rewrite_End_Message(0);
        break;

    case 1:
        if (end_etc_flag) {
            bgw_ptr->r_no_1++;
            end_w.timer = 10;
        }

        /* fallthrough */

    case 2:
        break;
    }
}

void end_b00_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_b_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = 0;
        Bg_On_W(1);
        bg_w.quake_y_index = 24;
        bgw_ptr->free = 2;
        effect_E6_init(0x63);
        Rewrite_End_Message(2);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->free = 2;

            if (bg_w.quake_y_index <= 0) {
                bgw_ptr->r_no_1++;
                bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
                bgw_ptr->abs_y = 0;
                effect_E6_init(0x64);
                break;
            }
            bgw_ptr->xy[1].disp.pos = (s16)quake_y_tbl[bg_w.quake_y_index] + end_b_pos[end_w.r_no_2][0];
            bgw_ptr->abs_y = quake_y_tbl[bg_w.quake_y_index];
        }

        break;

    case 2:
        break;
    }
}

void end_b01_move() {
    void (*end_b01_jp[7])() = { end_b01_0000, end_X_com01, end_b01_0000, end_b01_3000,
                                end_X_com01,  end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_b01_jp[end_w.r_no_2]();
}

void end_b01_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_b_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];

        if (end_w.r_no_2 == 1) {
            bgw_ptr->speed_y = 0x100000;
            break;
        }

        bgw_ptr->speed_y = -0x40000;
        break;

    case 1:
        bgw_ptr->r_no_1++;
        Bg_On_W(2U);
        /* fallthrough */

    case 2:
        bgw_ptr->xy[1].cal += bgw_ptr->speed_y;
        bgw_ptr->abs_y += bgw_ptr->speed_y;

        if (bgw_ptr->xy[1].disp.pos < 33) {
            bgw_ptr->xy[1].disp.pos = 288;
        }

        break;
    }
}

void end_b01_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_b_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Bg_On_W(2U);
        break;

    case 1:
        if (bg_w.quake_y_index) {
            bgw_ptr->xy[1].disp.pos = (s16)quake_y_tbl[bg_w.quake_y_index] + end_b_pos[end_w.r_no_2][0];
            bgw_ptr->abs_y = quake_y_tbl[bg_w.quake_y_index];
            break;
        }

        bgw_ptr->r_no_1++;
        bgw_ptr->xy[1].disp.pos = end_b_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = 0;
        break;

    case 2:
        break;
    }
}
