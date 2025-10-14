/**
 * @file end_06.c
 * Hugo's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_600_move();
void end_601_move();

void end_600_0000();
void end_600_1000();
void end_600_2000();
void end_600_3000();
void end_600_4000();
void end_600_5000();

void end_601_0000();
void end_601_1000();
void end_601_2000();
void end_601_3000();

const s16 timer_6_tbl[6] = { 420, 360, 120, 1080, 720, 540 };

const s16 end_6_pos[6][2] = { { 256, 768 }, { 256, 768 }, { 256, 256 }, { 256, 0 }, { 768, 768 }, { 768, 512 } };

const s16 end_600_1000_tbl[8][2] = { { -4, 2 }, { 6, -6 }, { -2, 4 }, { -4, 3 },
                                     { 11, 0 }, { -4, 2 }, { 8, 0 },  { 2, -2 } };

const s16 end_600_2000_tbl[12][2] = { { 3, -2 }, { 1, 3 }, { 2, -1 }, { 3, -2 }, { 2, 3 }, { 2, -2 },
                                      { 3, -2 }, { 2, 3 }, { 2, -2 }, { 3, -2 }, { 1, 3 }, { 2, -1 } };

void end_06000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_6_tbl[end_w.r_no_2];
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

            end_w.timer = timer_6_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
        }

        end_600_move();
        end_601_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_600_move() {
    void (*end_600_jp[6])() = { end_600_0000, end_600_1000, end_600_2000, end_600_3000, end_600_4000, end_600_5000 };
    bgw_ptr = &bg_w.bgw[0];
    end_600_jp[end_w.r_no_2]();
}

void end_600_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(1);
        break;

    case 1:
        break;
    }
}

void end_600_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << (bgw_ptr->fam_no));
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        bgw_ptr->free = 0;
        bgw_ptr->old_pos_x = 0;
        Rewrite_End_Message(2);
        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0x8000;

        if (bgw_ptr->xy[1].disp.pos < 625) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x2700000;
        }

        /* fallthrough */

    case 2:
        bgw_ptr->old_pos_x++;

        if (bgw_ptr->old_pos_x > 2) {
            bgw_ptr->old_pos_x = 0;
            bgw_ptr->free++;
            bgw_ptr->free &= 7;
            bgw_ptr->xy[0].disp.pos = end_600_1000_tbl[bgw_ptr->free][0] + 0x100;
            bgw_ptr->xy[1].disp.pos += end_600_1000_tbl[bgw_ptr->free][1];
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_600_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][1] + 16;
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] + 16;
        bgw_ptr->abs_x = 0x210;
        bgw_ptr->abs_y = 16;
        bgw_ptr->free = 0;
        bgw_ptr->old_pos_x = 16;
        effect_E6_init(0x2B);
        Rewrite_End_Message(0);
        break;

    case 1:
        bgw_ptr->old_pos_x--;

        if (bgw_ptr->old_pos_x < 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->old_pos_x = end_600_2000_tbl[bgw_ptr->free][0];
            bgw_ptr->free++;
            bgw_ptr->free &= 7;
            bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0] + end_600_2000_tbl[bgw_ptr->free][1];

            // TODO: Check if this is correct. Original PS2 matching line is:
            // bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] + end_600_2000_tbl[bgw_ptr->free][2];
            // Original dev could have counted this as 1-indexed.
            // Makes much more sense that above would be 0 and below would be 1.
            bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] - end_600_2000_tbl[bgw_ptr->free + 1][0];

            bgw_ptr->abs_x = end_600_2000_tbl[bgw_ptr->free][0] + 0x200;
            bgw_ptr->abs_y = end_600_2000_tbl[bgw_ptr->free][1];
        }

        break;

    case 2:
        bgw_ptr->old_pos_x--;

        if (bgw_ptr->old_pos_x < 0) {
            bgw_ptr->old_pos_x = end_600_2000_tbl[bgw_ptr->free][0];
            bgw_ptr->free++;
            bgw_ptr->free &= 7;
            bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0] + end_600_2000_tbl[bgw_ptr->free][1];

            // TODO: Check if this is correct. Original PS2 matching line is:
            // bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] + end_600_2000_tbl[bgw_ptr->free][2];
            // Original dev could have counted this as 1-indexed.
            // Makes much more sense that above would be 0 and below would be 1.
            bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] - end_600_2000_tbl[bgw_ptr->free + 1][0];

            bgw_ptr->abs_x = end_600_2000_tbl[bgw_ptr->free][0] + 0x200;
            bgw_ptr->abs_y = end_600_2000_tbl[bgw_ptr->free][1];
        }

        break;
    }
}

void end_600_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(3);
        effect_E6_init(0x2D);
        effect_E6_init(0x2E);
        bgw_ptr->free = 0x1E;
        break;

    case 1:
    case 2:
    case 3:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 0x1E;
            effect_E6_init(0x31);
        }

        break;

    case 4:
        break;
    }
}

void end_600_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(4);
        break;

    case 1:
        break;
    }
}

void end_600_5000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(5);
        bgw_ptr->free = 0x1E;
        end_fade_flag = 1;
        end_fade_timer = timer_6_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 3;
            bgw_ptr->l_limit = 0;
        }

        break;

    case 2:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 3;
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 4) {
                bgw_ptr->l_limit = 0;
            }
        }

        break;
    }
}

void end_601_move() {
    void (*end_601_jp[6])() = { end_601_0000, end_601_1000, end_601_2000, end_601_3000, end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_601_jp[end_w.r_no_2]();
}

void end_601_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        break;
    }
}

void end_601_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0x2000;

        if (bg_w.bgw[0].r_no_1 >= 2) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_601_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_Off_W(1 << (bgw_ptr->fam_no));
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][1] - 16;
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] - 16;
        bgw_ptr->abs_x = 0x1F0;
        bgw_ptr->abs_y = -16;
        bgw_ptr->free = 0;
        bgw_ptr->old_pos_x = 16;

        if (Country == 1 || Country == 8) {
            effect_E6_init(0x2C);
            break;
        }

        effect_E6_init(0x98);
        break;

    case 1:
        bgw_ptr->old_pos_x--;

        if (bgw_ptr->old_pos_x < 0) {
            bgw_ptr->old_pos_x = end_600_2000_tbl[bgw_ptr->free][0];
            bgw_ptr->free++;
            bgw_ptr->free &= 7;
            bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0] - end_600_2000_tbl[bgw_ptr->free][1];

            // TODO: Check if this is correct. Original PS2 matching line is:
            // bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] - end_600_2000_tbl[bgw_ptr->free][2];
            // Original dev could have counted this as 1-indexed.
            // Makes much more sense that above would be 0 and below would be 1.
            bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1] - end_600_2000_tbl[bgw_ptr->free + 1][0];

            bgw_ptr->abs_x = 0x200 - end_600_2000_tbl[bgw_ptr->free][0];
            bgw_ptr->abs_y = -end_600_2000_tbl[bgw_ptr->free][1];
        }

        break;

    case 2:
        break;
    }
}

void end_601_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->xy[0].disp.pos = end_6_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_6_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        break;
    }
}
