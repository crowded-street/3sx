/**
 * @file end_00.c
 * Gill's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_000_move();
void end_001_move();

void end_000_0000();
void end_000_0001();
void end_000_0002();
void end_000_0003();
void end_000_0004();

void end_001_0004();

// sbss
u8 fade_prio;
s16 gill_quake_flag;
s16 gill_quake_flag2;

// rodata
const s16 timer_0_tbl[5] = { 720, 1140, 720, 720, 960 };

const s16 gill_time[10] = { 120, 120, 300, 420, 120, 90, 30, 180, 60, 1 };

const u8 pika_prio[2] = { 30, 100 };

const s16 gill_stop_pos[1] = { 180 };

const s16 end_0_pos[5][2] = { { 256, 768 }, { 768, 768 }, { 256, 512 }, { 768, 512 }, { 256, 112 } };

const s16 end00_quake_tbl[32] = { -1, -2, -4, -2, 0, -2, 0, 0, 0, 2,  4, 1, 3, 0, -2, 1,
                                  0,  -2, -4, -5, 1, 3,  2, 1, 0, -1, 2, 4, 0, 3, -4, 2 };

const s16 end00_quake_tbl2[32] = { 0, -1, -2, -1, 0, -1, 0, 0, 0, 1,  2, 1, 2, 0, -1, 0,
                                   0, -1, -2, -3, 0, 2,  1, 0, 0, -1, 1, 2, 0, 2, -2, 1 };

const s16 end00_quake_timer[32] = { 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 8,
                                    4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 8, 4, 8 };

void end_00000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_0_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x31);
        Bg_Off_W(2);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 5) {
                ToneDown(0xFF, 0);
                FadeInit();
                end_w.r_no_1++;
                end_w.end_flag = 1;
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                Bg_Off_W(0xF);
                break;
            }

            end_w.timer = timer_0_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_000_move();
        end_001_move();
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;

    case 2:
        ToneDown(0xFF, 0);
        break;
    }
}

void end_000_move() {
    void (*end_000_jp[5])() = { end_000_0000, end_000_0001, end_000_0002, end_000_0003, end_000_0004 };
    bgw_ptr = &bg_w.bgw[0];
    end_000_jp[end_w.r_no_2]();
}

void end_000_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_0_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = 0;
        Bg_On_W(1);
        effect_E6_init(0xA8);
        Rewrite_End_Message(1);
        gill_quake_flag = gill_quake_flag2 = 1;
        break;
    }
}

s16 end_0_1_time[1] = { 360 };

void end_000_0001() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_0_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = 0;
        Bg_On_W(1);
        effect_E6_init(0xA9);
        effect_E6_init(0xAB);
        Rewrite_End_Message(2);
        bgw_ptr->free = end_0_1_time[0];
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 1) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = gill_time[0];
            effect_E6_init(0xAA);
        }

        break;

    case 2:
        if (!bgw_ptr->free--) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
            fade_prio = pika_prio[1];
            bgw_ptr->free = gill_time[7];
            bgw_ptr->l_limit = bgw_ptr->r_limit = 0;
        }

        break;

    case 3:
        bgw_ptr->free--;

        if (bgw_ptr->free < 1) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = gill_time[1];
        }

        break;

    case 4:
        bgw_ptr->r_no_1++;
        end_etc_flag = 1;
        break;

    case 5:
        if (!bgw_ptr->free--) {
            bgw_ptr->r_no_1++;
            Fade_Flag = 0;
            bgw_ptr->free = 300;
        }

        break;

    case 6:
        if (!bgw_ptr->free--) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 7:
        if (Request_Fade(3)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 8:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 0x28;
            overwrite_panel(0xFFFFFFFF, 0x17);
        }

        break;

    case 9:
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
            end_w.timer = 0;
        }

        break;
    }
}

void end_000_0002() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFFFFFFFF, 0x17);

        if (Request_Fade(2)) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
            bgw_ptr->xy[1].disp.pos = end_0_pos[end_w.r_no_2][1];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = 0;
            effect_E6_init(0xAC);
            end_no_cut = 1;
        }

        break;

    case 1:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            Rewrite_End_Message(3);
        }

        break;

    case 2:
        break;
    }
}

void end_000_0003() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_0_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0xAD);
        Rewrite_End_Message(4);
        break;

    case 1:
        break;
    }
}

void end_000_0004() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_0_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(5);
        end_fade_flag = 1;
        end_fade_timer = timer_0_tbl[end_w.r_no_2] - 120;
        bgw_ptr->speed_y = 0x440000 / gill_time[2];
        bgw_ptr->l_limit = gill_time[2];
        break;

    case 1:
        bgw_ptr->xy[1].cal += bgw_ptr->speed_y;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (!bgw_ptr->l_limit--) {
            bgw_ptr->r_no_1++;
            bgw_ptr->l_limit = gill_time[8];
        }

        break;

    case 2:
        bgw_ptr->r_no_1++;
        effect_E6_init(0xB0);
        bgw_ptr->frame_deff = 0;
        bgw_ptr->r_limit = 1;
        /* fallthrough */

    case 3:
        bgw_ptr->r_no_1++;
        bgw_ptr->free = gill_time[5];

        if (gill_quake_flag && (bgw_ptr->r_limit--, bgw_ptr->r_limit < 1)) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->r_limit = end00_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[1].disp.pos = gill_stop_pos[0];
            bgw_ptr->xy[1].disp.pos += end00_quake_tbl[bgw_ptr->frame_deff];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        }

        /* fallthrough */

    case 4:
        bgw_ptr->free--;
        bgw_ptr->r_no_1++;
        end_etc_flag = 2;
        FadeInit();
        bgw_ptr->max_x_limit = 0;
        bgw_ptr->max_x_limit = gill_stop_pos[0] << 16;
        bgw_ptr->speed_y = 0x480000 / gill_time[3];
        bgw_ptr->l_limit = gill_time[3];
        bgw_ptr->xy[1].disp.pos = gill_stop_pos[0];
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (gill_quake_flag && (bgw_ptr->r_limit--, bgw_ptr->r_limit < 1)) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->r_limit = end00_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[1].disp.pos = gill_stop_pos[0];
            bgw_ptr->xy[1].disp.pos += end00_quake_tbl[bgw_ptr->frame_deff];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        }

        /* fallthrough */

    case 5:
        bgw_ptr->max_x_limit += bgw_ptr->speed_y;
        bgw_ptr->xy[1].cal = bgw_ptr->max_x_limit;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (bgw_ptr->l_limit < 30) {
            FadeOut(1, 8, 8);
        }

        if (!(bgw_ptr->l_limit--)) {
            bgw_ptr->r_no_1++;
            ToneDown(0xFF, 0);
            FadeInit();
        }

        if (gill_quake_flag && (bgw_ptr->r_limit--, bgw_ptr->r_limit < 1)) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->r_limit = end00_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[1].disp.pos += end00_quake_tbl2[bgw_ptr->frame_deff];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        }

        break;

    case 6:
        ToneDown(0xFF, 0);
        end_w.timer = 0;
        bgw_ptr->xy[1].disp.pos = 248;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (gill_quake_flag && (bgw_ptr->r_limit--, bgw_ptr->r_limit < 1)) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->r_limit = end00_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[1].disp.pos += end00_quake_tbl2[bgw_ptr->frame_deff];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        }

        break;
    }
}

void end_001_move() {
    void (*end_001_jp[5])() = { end_X_com01, end_X_com01, end_X_com01, end_X_com01, end_001_0004 };
    bgw_ptr = &bg_w.bgw[1];
    end_001_jp[end_w.r_no_2]();
}

void end_001_0004() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0xAE);
        effect_E6_init(0xAF);
        bgw_ptr->speed_y = 0x340000 / gill_time[2];
        bgw_ptr->l_limit = gill_time[2];
        break;

    case 1:
        bgw_ptr->xy[1].cal += bgw_ptr->speed_y;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (!bgw_ptr->l_limit--) {
            bgw_ptr->r_no_1++;
            bgw_ptr->frame_deff = 0;
            bgw_ptr->l_limit = gill_time[8];
        }

        break;

    case 2:
        bgw_ptr->r_no_1++;
        break;

    case 3:
        if (end_etc_flag) {
            bgw_ptr->r_no_1++;
            bgw_ptr->r_limit = gill_time[6];
        }

        /* fallthrough */

    case 4:
        bgw_ptr->r_no_1++;
        /* fallthrough */

    case 5:
    case 6:
        bgw_ptr->r_limit--;

        if (bgw_ptr->r_limit < 1) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->r_limit = end00_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[0].disp.pos = end_0_pos[end_w.r_no_2][0];
            bgw_ptr->xy[1].disp.pos = 48;
            bgw_ptr->xy[1].disp.pos += end00_quake_tbl2[bgw_ptr->frame_deff];
            bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
            bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        }

        break;
    }
}
