/**
 * @file end_10.c
 * Yang's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"

void end_1000_move();
void end_1001_move();
void end_1002_move();
void end_1003_move();

void end_1000_0000();
void end_1000_1000();
void end_1000_2000();
void end_1000_4000();

void end_1001_1000();

void end_1002_1000();

void end_1003_1000();
void end_1003_2000();

const s16 timer_10_tbl[6] = { 780, 720, 420, 780, 420, 660 };

const s16 end_10_pos[6][2] = { { 256, 768 }, { 256, 512 }, { 256, 432 }, { 256, 432 }, { 256, 0 }, { 256, 256 } };

void end_10000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_10_tbl[end_w.r_no_2];
        common_end_init01();
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

            end_w.timer = timer_10_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
            bg_w.bgw[5].r_no_1 = 0;
        }

        end_1000_move();
        end_1001_move();
        end_1002_move();
        end_1003_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1000_move() {
    void (*end_1000_jp[6])() = { end_1000_0000, end_1000_1000, end_1000_2000,
                                 end_1000_2000, end_1000_4000, end_1000_2000 };
    bgw_ptr = &bg_w.bgw[0];
    end_1000_jp[end_w.r_no_2]();
}

void end_1000_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        effect_E6_init(0x4C);
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->free = 4;
        bgw_ptr->l_limit = 0;
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 4;
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 6) {
                bgw_ptr->l_limit = 0;
            }
        }

        break;
    }
}

void end_1000_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->pos_x_work = 512;
        bgw_ptr->pos_y_work = bgw_ptr->xy[1].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        bgw_ptr->speed_x = 0;
        bgw_ptr->speed_y = 0xD800;
        Rewrite_End_Message(2);
        bgw_ptr->free = 0xB4;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y;

        if (bg_w.bgw[1].r_no_1 >= 3) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}

void end_1000_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];

        switch (end_w.r_no_2) {
        case 2:
            bgw_ptr->abs_x = 512;
            bgw_ptr->abs_y = 0;
            effect_E6_init(0x50);
            effect_E6_init(0x5C);
            Rewrite_End_Message(3);
            break;

        case 3:
            effect_E6_init(0x8E);
            effect_E6_init(0x51);
            Rewrite_End_Message(4);
            break;

        case 5:
            effect_E6_init(0x52);
            effect_E6_init(0x53);
            Rewrite_End_Message(6);
            end_fade_flag = 1;
            end_fade_timer = timer_10_tbl[end_w.r_no_2] - 120;
            break;
        }

        break;

    case 1:
        break;
    }
}

void end_1000_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        effect_E6_init(0x9C);
        effect_E6_init(0xA5);
        bgw_ptr->free = 8;
        bgw_ptr->l_limit = 0;
        Rewrite_End_Message(5);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 8;
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 5) {
                bgw_ptr->l_limit = 0;
            }
        }

        break;
    }
}

void end_1001_move() {
    void (*end_101_jp[6])() = { end_X_com01, end_1001_1000, end_X_com01, end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_101_jp[end_w.r_no_2]();
}

void end_1001_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x4D);
        bgw_ptr->speed_y = 0x8000;
        bgw_ptr->free = 0xB4;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y;

        if (bgw_ptr->xy[1].disp.pos < 401) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1900000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}

void end_1002_move() {
    void (*end_102_jp[6])() = { end_X_com01, end_1002_1000, end_X_com01, end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[2];
    end_102_jp[end_w.r_no_2]();
}

void end_1002_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x4E);
        bgw_ptr->speed_y = 0xB000;
        bgw_ptr->free = 0xB4;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y;

        if (bg_w.bgw[1].r_no_1 >= 3) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}

void end_1003_move() {
    void (*end_103_jp[6])() = { end_X_com01, end_1003_1000, end_1003_2000, end_X_com01, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[5];
    end_103_jp[end_w.r_no_2]();
}

void end_1003_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x4F);
        bgw_ptr->free = 0xB4;
        end_bg_pos_hosei(5);
        end_fam_set(5);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
        }

        end_bg_pos_hosei(5);
        end_fam_set(5);
        break;

    case 2:
        bgw_ptr->xy[1].cal -= 0x4000;

        if (bg_w.bgw[1].r_no_1 >= 3) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        /* fallthrough */

    case 3:
        end_bg_pos_hosei(5);
        end_fam_set(5);
        break;
    }
}

void end_1003_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_10_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_10_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        /* fallthrough */

    case 1:
        end_bg_pos_hosei(5);
        end_fam_set(5);
        break;
    }
}
