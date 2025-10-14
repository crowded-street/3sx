/**
 * @file end_02.c
 * Ryu's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_200_move();
void end_201_move();
void end_202_move();

void end_200_0000();
void end_200_1000();
void end_200_2000();
void end_200_3000();

void end_201_1000();
void end_201_3000();

void end_202_1000();
void end_202_3000();
void end_202_4000();

const s16 timer_2_tbl[5] = { 480, 660, 120, 240, 780 };

const s16 end_2_pos[5][2] = { { 256, 768 }, { 320, 256 }, { 512, 0 }, { 320, 256 }, { 320, 256 } };

void end_02000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        end_w.timer = timer_2_tbl[end_w.r_no_2];
        BGM_Request(0x31);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 5) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_2_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_200_move();
        end_201_move();
        end_202_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_200_move() {
    void (*end_200_jp[5])() = { end_200_0000, end_200_1000, end_200_2000, end_200_3000, end_200_3000 };
    bgw_ptr = &bg_w.bgw[0];
    end_200_jp[end_w.r_no_2]();
}

void end_200_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Rewrite_End_Message(1);
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        bgw_ptr->free = 0x1E;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->speed_y = 0xC000;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y;

        if (bgw_ptr->xy[1].disp.pos < 513) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x2000000;
        }

        break;
    }
}

void end_200_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Rewrite_End_Message(2);
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        bgw_ptr->speed_x = 0x4000;
        bgw_ptr->free = 3;
        bgw_ptr->l_limit = 0;
        break;

    case 1:
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos < 193) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x7C);
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_200_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        Rewrite_End_Message(3);
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x7D);
        effect_E6_init(0x7E);
        break;

    case 1:
        end_X_com01();
        break;
    }
}

void end_200_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];

        switch (end_w.r_no_2) {
        case 3:
            Rewrite_End_Message(4);
            bgw_ptr->free = 3;
            bgw_ptr->l_limit = 0;
            break;

        case 4:
            Rewrite_End_Message(5);
            end_fade_flag = 1;
            end_fade_timer = timer_2_tbl[end_w.r_no_2] - 120;
            break;
        }

    case 1:
        break;
    }
}

void end_201_move() {
    void (*end_201_jp[5])() = { end_X_com01, end_201_1000, end_X_com01, end_201_3000, end_201_3000 };
    bgw_ptr = &bg_w.bgw[1];
    end_201_jp[end_w.r_no_2]();
}

void end_201_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->speed_x = 0x4000;
        bgw_ptr->free = 4;
        bgw_ptr->l_limit = 0;
        break;

    case 1:
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos < 193) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_201_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];

        if (end_w.r_no_2 == 3) {
            bgw_ptr->free = 4;
            bgw_ptr->l_limit = 0;
        }

        break;

    case 1:
    case 2:
        break;
    }
}

void end_202_move() {
    void (*end_202_jp[5])() = { end_X_com01, end_202_1000, end_X_com01, end_202_3000, end_202_4000 };
    bgw_ptr = &bg_w.bgw[2];
    end_202_jp[end_w.r_no_2]();
}

void end_202_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->speed_x = 0xC000;
        effect_E6_init(0x7B);
        break;

    case 1:
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos < 129) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_202_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        effect_E6_init(0x7F);
        bgw_ptr->speed_x = 0x8000;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 1:
        bgw_ptr->xy[0].cal += bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos >= 560) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_202_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_2_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_2_pos[end_w.r_no_2][1];
        effect_E6_init(0x80);
        break;

    case 1:
        break;
    }
}
