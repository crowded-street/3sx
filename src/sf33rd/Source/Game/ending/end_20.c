/**
 * @file end_20.c
 * Remy's Ending
 */

#include "common.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_2000_move();
void end_2001_move();

void end_2000_0000();
void end_2000_0001();
void end_2000_0002();
void end_2000_0003();
void end_2000_0005();

void end_2001_0000();
void end_2001_0002();
void end_2001_0003();
void end_2001_0004();
void end_2001_0005();

void sea_write();
void sea_trans(u16 num, f64 arg2);

const s16 timer_20_tbl[6] = { 360, 420, 780, 720, 240, 780 };

const s16 end_20_pos[6][2] = { { 256, 704 }, { 256, 304 }, { 256, 448 }, { 256, 816 }, { 768, 768 }, { 256, 224 } };

void end_20000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        end_w.timer = timer_20_tbl[end_w.r_no_2];
        BGM_Request(0x31);
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

            end_w.timer = timer_20_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_2000_move();
        end_2001_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_2000_move() {
    void (*end_2000_jp[6])() = { end_2000_0000, end_2000_0001, end_2000_0002,
                                 end_2000_0003, end_2000_0002, end_2000_0005 };
    bgw_ptr = &bg_w.bgw[0];
    end_2000_jp[end_w.r_no_2]();
}

void end_2000_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        effect_E6_init(0x3E);
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->xy[0].cal += 0x8000;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->xy[1].cal += 0x4000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_2000_0001() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(2);
        effect_E6_init(0x3F);
        effect_E6_init(0x40);
        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0x4000;

        if (bgw_ptr->xy[1].disp.pos < 272) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1100000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_2000_0002() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);

        switch (end_w.r_no_2) {
        case 2:
            Rewrite_End_Message(3);
            break;

        case 4:
            Rewrite_End_Message(5);
            effect_E6_init(0x42);
            effect_E6_init(0x43);
            break;
        }

        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        break;
    }
}

void end_2000_0003() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_Off_W(1);
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(4);
        effect_E6_init(0x91);
        effect_E6_init(0x92);
        effect_E6_init(0x93);
        effect_E6_init(0x94);
        effect_E6_init(0x95);
        break;

    case 1:
        bgw_ptr->xy[0].cal -= 0x4000;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->xy[1].cal += 0x6000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_2000_0005() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x44);
        effect_E6_init(0x45);
        Rewrite_End_Message(6);
        end_fade_flag = 1;
        end_fade_timer = timer_20_tbl[end_w.r_no_2] - 120;
        bgw_ptr->free = 0x5A;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bgw_ptr->xy[1].disp.pos > 544) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x2200000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}

void end_2001_move() {
    void (
        *end_2001_jp[6])() = { end_2001_0000, end_X_com01, end_2001_0002, end_2001_0003, end_2001_0004, end_2001_0005 };
    bgw_ptr = &bg_w.bgw[1];
    end_2001_jp[end_w.r_no_2]();
}

void end_2001_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        effect_E6_init(0x38);
        break;

    case 1:
        break;
    }
}

void end_2001_0002() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        effect_E6_init(0x41);
        break;

    case 1:
        break;
    }
}

void end_2001_0003() {
    ppgSetupCurrentDataList(&ppgAkeList);
    ppgSetupCurrentPaletteNumber(NULL, 0);

    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        ls_cnt1 = 0;
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        /* fallthrough */

    case 1:
        sea_write();
        break;
    }
}

void end_2001_0004() {
    switch (bgw_ptr->r_no_1) {
    case 0:
    case 1:
        end_X_com01();
        break;
    }
}

void end_2001_0005() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_20_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_20_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        /* fallthrough */

    case 1:
    case 2:
        break;
    }
}

void sea_write() {
    u16 j;
    u16 k;
    SEA_WORK work;

    e_line_step = 64;

    for (j = 0; j < 512; j++) {
        k = ls_cnt1 + (j * 4) & 0xFF;
        work.s.xx = e_line_step * rate_256_table[k][0] >> 4;

        if (No_Trans) {
            continue;
        }

        sea_trans(j, (f32)work.o.xh);
    }

    ls_cnt1 = (ls_cnt1 + 2) & 0x1FF;
}

void sea_trans(u16 num, f64 arg2) {
    f32 suzi = arg2;
    Polygon poly[4];

    poly[0].col = poly[1].col = poly[2].col = poly[3].col = -1;
    poly[0].u = poly[1].u = 0.0f;
    poly[2].u = poly[3].u = 1.0f;
    poly[0].v = poly[2].v = num / 512.0f;
    poly[1].v = poly[3].v = (num + 1) / 512.0f;
    poly[0].x = poly[1].x = (-64.0f + suzi) * Frame_Zoom_X;
    poly[2].x = poly[3].x = (448.0f + suzi) * Frame_Zoom_X;
    poly[0].y = poly[2].y = (32.0f + num) * Frame_Zoom_Y;
    poly[1].y = poly[3].y = (32.0f + (num + 1)) * Frame_Zoom_Y;
    poly[0].z = poly[1].z = poly[2].z = poly[3].z = PrioBase[84];
    njDrawTexture(poly, 4, 228, 0);
}
