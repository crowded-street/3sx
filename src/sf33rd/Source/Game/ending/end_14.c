/**
 * @file end_14.c
 * Akuma/Gouki's Ending
 */

#include "sf33rd/Source/Game/ending/end_14.h"
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

void end_e00_move();
void end_e01_move();
void end_e02_move();

void end_e00_0000();
void end_e00_1000();
void end_e00_2000();
void end_e00_3000();
void end_e00_4000();
void end_e00_5000();
void end_e00_6000();
void end_e00_7000();

void end_e01_0000();
void end_e01_7000();

void end_e02_0000();
void end_e02_1000();
void end_e02_2000();
void end_e02_3000();
void end_e02_4000();
void end_e02_7000();

s16 end_e00_0000_col_sub();
s16 end_e00_0000_col_sub2();
void end_e00_1000_col_sub();

struct {
    XY xy[2];
} gxy;

const s16 timer_e_tbl[9] = { 1320, 240, 900, 1200, 360, 360, 300, 420, 600 };

const s16 end_e_pos[10][2] = { { 256, 768 }, { 768, 0 }, { 768, 768 }, { 256, 0 },   { 768, 768 },
                               { 768, 768 }, { 256, 0 }, { 256, 768 }, { 256, 256 }, { 768, 256 } };

void end_14000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        nosekae = 0;
        g_kakikae[0] = 0;
        g_kakikae[1] = 0;
        g_number[0] = 0;
        g_number[1] = 0;
        gxy.xy[1].disp.pos = 0;
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_e_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x32);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 == 8) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_e_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_e00_move();
        end_e01_move();
        end_e02_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_e00_move() {
    void (*end_e00_jp[8])() = { end_e00_0000, end_e00_1000, end_e00_2000, end_e00_3000,
                                end_e00_4000, end_e00_5000, end_e00_6000, end_e00_7000 };
    bgw_ptr = &bg_w.bgw[0];
    end_e00_jp[end_w.r_no_2]();
}

void end_e00_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[0].disp.pos += 64;
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->free = 0x3C;
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= 0x18000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (bgw_ptr->xy[1].disp.pos < 273) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x19);
            gxy.xy[1].disp.pos = bgw_ptr->xy[1].disp.pos;
            nosekae = 1;
            *scr_bcm = ending_map_tbl[20][0];
        }

        break;

    case 3:
        bgw_ptr->xy[1].cal -= 0x18000;
        gxy.xy[1].cal = gxy.xy[1].cal - 0x18000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].disp.pos = 0;
        }

        if (gxy.xy[1].disp.pos < -223) {
            bgw_ptr->xy[1].disp.pos = 752;
        }

        break;

    case 4:
        bgw_ptr->r_no_1++;
        bgw_ptr->free = 7;
        bgw_ptr->l_limit = 0;
        g_kakikae[0] = 1;
        /* fallthrough */

    case 5:
        if (end_e00_0000_col_sub()) {
            bgw_ptr->r_no_1++;
        }

        /* fallthrough */

    case 6:
        bgw_ptr->xy[1].cal -= 0x4000;

        if (bgw_ptr->xy[1].disp.pos < 713) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x2C80000;
            end_w.timer = 20;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 7:
        break;
    }
}

const u8 end_e00_0000_col_tbl[12] = { 0, 1, 2, 3, 4, 5, 6, 5, 6, 5, 6, 6 };

s16 end_e00_0000_col_sub() {
    bgw_ptr->free--;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->free = 7;
        bgw_ptr->l_limit++;

        if (bgw_ptr->l_limit >= 12) {
            return 1;
        }

        *g_number = end_e00_0000_col_tbl[bgw_ptr->l_limit];
    }

    return 0;
}

s16 end_e00_0000_col_sub2() {
    bgw_ptr->free--;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->free = 7;
        bgw_ptr->l_limit++;

        if (bgw_ptr->l_limit >= 12) {
            return 1;
        }
    }

    return 0;
}

const u8 end_e00_1000_col_tbl[8] = { 0, 0, 1, 2, 3, 4, 5, 6 };

void end_e00_1000_col_sub() {
    if (bgw_ptr->l_limit >= 8) {
        return;
    }

    bgw_ptr->free--;

    if (bgw_ptr->free > 0) {
        return;
    }

    bgw_ptr->l_limit++;

    if (bgw_ptr->l_limit < 7) {
        bgw_ptr->free = 8;
        g_number[1] = end_e00_1000_col_tbl[bgw_ptr->l_limit];
    }
}

void end_e00_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 768;
        bgw_ptr->xy[1].disp.pos = 128;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 128;
        bgw_ptr->free = 8;
        bgw_ptr->l_limit = 0;
        g_kakikae[1] = 1;
        g_number[1] = 0;
        Rewrite_End_Message(2);
        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0x8000;

        if (bgw_ptr->xy[1].disp.pos < 36) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x240000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        /* fallthrough */

    case 2:
        end_e00_1000_col_sub();
        break;
    }
}

const u8 end_e00_2000_col_tbl[8] = { 6, 5, 4, 3, 2, 1, 0, 0 };

void end_e00_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bgw_ptr->xy[1].disp.pos > 80) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x500000;
            bgw_ptr->free = 0x4E;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 8;
            bgw_ptr->l_limit = 0;
        }

        break;

    case 3:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->l_limit++;

            if (bgw_ptr->l_limit >= 8) {
                bgw_ptr->r_no_1++;
                end_w.timer = 120;
                break;
            }

            bgw_ptr->free = 8;
            g_number[1] = end_e00_2000_col_tbl[bgw_ptr->l_limit];
        }

        break;

    case 4:
        g_kakikae[1] = 0;
        g_number[1] = 0;
        break;
    }
}

void end_e00_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_Off_W(1);
        bgw_ptr->xy[0].disp.pos = 256;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x1A);
        bgw_ptr->free = 0x30;
        Rewrite_End_Message(0);
        break;

    case 1:
        break;

    case 2:
        bgw_ptr->free--;

        if (bgw_ptr->free < 1) {
            bgw_ptr->r_no_1++;
        } else {
            Frame_Up(0xC0, 0x30, 1);
        }

        break;

    case 3:
        if (Request_Fade(3)) {
            end_no_cut = 1;
            bgw_ptr->r_no_1++;
        }

        break;

    case 4:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 10;
            overwrite_panel(0xFFFFFFFF, 0x17);
            Frame_Down(0xC0, 0x30, 0x10);
        }

        break;

    case 5:
        Frame_Down(0xC0, 0x30, 3);
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_e00_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->r_no_1++;
        Zoomf_Init();
        bgw_ptr->xy[0].disp.pos = 256;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x1D);
        effect_E6_init(0x1E);
        break;

    case 1:
        overwrite_panel(0xFFFFFFFF, 0x17);

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
        bgw_ptr->xy[1].cal += 0x4000;

        if (bgw_ptr->xy[1].disp.pos >= 64) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x400000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 4:
        break;
    }
}

void end_e00_5000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Bg_On_W(1);
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        Rewrite_End_Message(3);
        break;

    case 1:
        break;
    }
}

void end_e00_6000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Bg_Off_W(1);
        effect_E6_init(0x1E);
        end_etc_flag = 0;
        effect_E6_init(0x1F);
        Rewrite_End_Message(4);
        break;

    case 1:
        if (end_etc_flag) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 10;
        }

        break;

    case 2:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 3:
        if (Request_Fade(3)) {
            end_no_cut = 1;
            bgw_ptr->r_no_1++;
        }

        break;

    case 4:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 10;
            overwrite_panel(0xFFFFFFFF, 0x17);
        }

        /* fallthrough */

    case 5:
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_e00_7000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->r_no_1++;
        nosekae = 2;
        *scr_bcm = ending_map_tbl[20][1];
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->xy[1].disp.pos += 48;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Bg_On_W(1);
        end_fade_flag = 1;
        end_fade_timer = timer_e_tbl[end_w.r_no_2] - 120;
        Rewrite_End_Message(5);
        /* fallthrough */

    case 1:
        overwrite_panel(0xFFFFFFFF, 0x17);

        if (Request_Fade(2)) {
            end_no_cut = 1;
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
        }

        /* fallthrough */

    case 3:
        bgw_ptr->xy[1].cal -= 0x3000;

        if (bgw_ptr->xy[1].disp.pos < 697) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x2B80000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 4:
        break;
    }
}

void end_e01_move() {
    void (*end_101_jp[8])() = { end_e01_0000, end_X_com01, end_X_com01, end_X_com01,
                                end_X_com01,  end_X_com01, end_X_com01, end_e01_7000 };
    bgw_ptr = &bg_w.bgw[1];
    end_101_jp[end_w.r_no_2]();
}

void end_e01_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        bgw_ptr->free = 0x3C;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[1].cal -= 0x18000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (bgw_ptr->xy[1].disp.pos < 273) {
            bgw_ptr->r_no_1++;
            Bg_Off_W(2);
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        bgw_ptr->xy[1].cal -= 0x18000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 4:
        bgw_ptr->r_no_1++;
        bgw_ptr->free = 7;
        bgw_ptr->l_limit = 0;
        /* fallthrough */

    case 5:
        if (end_e00_0000_col_sub2()) {
            bgw_ptr->r_no_1++;
        }

        /* fallthrough */

    case 6:
        bgw_ptr->xy[1].cal -= 0x4000;

        if (bgw_ptr->xy[1].disp.pos < -311) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0xFEC80000;
            end_w.timer = 20;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 7:
        break;
    }
}

void end_e01_7000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x23);
        break;

    case 1:
        if (bg_w.bgw[0].r_no_1 >= 4) {
            bgw_ptr->r_no_1++;
        } else {
            bgw_ptr->xy[1].cal -= 0x7000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_e02_move() {
    void (*end_102_jp[8])() = { end_e02_0000, end_e02_1000, end_e02_2000, end_e02_3000,
                                end_e02_4000, end_X_com01,  end_X_com01,  end_e02_7000 };
    bgw_ptr = &bg_w.bgw[2];
    end_102_jp[end_w.r_no_2]();
}

void end_e02_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x16);
        effect_E6_init(0x17);
        effect_E6_init(0x18);
        effect_E6_init(0x1B);
        effect_E6_init(0x1C);
        bgw_ptr->free = 0x3C;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        bgw_ptr->xy[0].cal -= 0x8000;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->xy[1].cal += 0xFFFDE000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;
    }
}

void end_e02_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(4);
        bgw_ptr->xy[0].disp.pos = 768;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        break;
    }
}

void end_e02_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 768;
        bgw_ptr->xy[1].disp.pos = 256;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0xF000;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;

        if (bgw_ptr->xy[1].disp.pos >= 432) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 2:
        break;
    }
}

void end_e02_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 768;
        bgw_ptr->xy[1].disp.pos = 408;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 1:
        bgw_ptr->xy[1].cal += -0x18000;

        if (bgw_ptr->xy[1].disp.pos < 353) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1600000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}

void end_e02_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 768;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        bgw_ptr->r_no_1++;
        Bg_Off_W(4);
        break;

    case 2:
        break;
    }
}

void end_e02_7000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_e_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_e_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        effect_E6_init(0x20);
        effect_E6_init(0x21);
        effect_E6_init(0x22);
        break;

    case 1:
        if (bg_w.bgw[0].r_no_1 >= 4) {
            bgw_ptr->r_no_1++;
        } else {
            bgw_ptr->xy[1].cal -= 0x6000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}
