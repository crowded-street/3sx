/**
 * @file end_05.c
 * Necro's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/sc_sub.h"

#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_500_move();
void end_501_move();

void end_500_comm();
void end_500_0001();
void end_500_0006();
void end_500_0007();
void end_500_0008();
void end_500_0011();

void end_501_0007();
void end_501_0008();
void end_501_0009();
void end_501_0010();
void end_501_0011();
void end_501_0012();

void end_5_bg0_move_sub();
void end_5_bg1_move_sub();
void end_5_bg1_move_sub2();

s8 bdl_index;
s8 wr5_index;
s16 end_5_flag;

const s16 timer_5_tbl[13] = { 120, 540, 660, 420, 180, 360, 360, 360, 360, 360, 300, 360, 360 };

const s16 end_5_pos[11][2] = { { 256, 768 }, { 768, 768 }, { 256, 512 }, { 768, 512 }, { 256, 256 }, { 768, 256 },
                               { 256, 768 }, { 256, 768 }, { 256, 768 }, { 256, 768 }, { 256, 768 } };

const s16 necro_quake_tbl[32][2] = { { 2, -2 },  { 4, -4 }, { 6, -8 }, { 8, -4 }, { 6, 0 },  { 8, -4 },  { 4, 0 },
                                     { 0, 0 },   { -2, 0 }, { -4, 4 }, { -4, 8 }, { -6, 2 }, { 0, 6 },   { 4, 0 },
                                     { -5, -4 }, { -5, 2 }, { 0, 0 },  { 2, -4 }, { 4, -8 }, { 6, -10 }, { 2, 2 },
                                     { -2, 6 },  { 0, 4 },  { 3, 1 },  { 6, 0 },  { 4, -2 }, { -6, 4 },  { -2, 8 },
                                     { 0, 0 },   { 6, 6 },  { 0, -8 }, { 4, 4 } };

const s16 necro_quake_timer[32] = { 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 8,
                                    4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 8, 4, 8 };

const s16 end_500_quake_tbl[8] = { 2, 4, -2, -4, -2, -4, 2, 4 };

void end_05000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        end_w.timer = timer_5_tbl[end_w.r_no_2];
        common_end_init01();
        BGM_Request(0x32);
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 == 13) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_5_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_500_move();
        end_501_move();
        /* fallthough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_500_move() {
    void (*end_500_jp[13])() = { end_500_comm, end_500_0001, end_500_0001, end_500_comm, end_500_comm,
                                 end_500_comm, end_500_0006, end_500_0007, end_500_0008, end_X_com01,
                                 end_X_com01,  end_500_0011, end_X_com01 };
    bgw_ptr = &bg_w.bgw[0];
    end_500_jp[end_w.r_no_2]();
}

void end_500_comm() {
    bgw_ptr = &bg_w.bgw[0];

    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_5_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_5_pos[end_w.r_no_2][1];

        switch (end_w.r_no_2) {
        case 0:
            Bg_On_W(1);
            Rewrite_End_Message(0);
            break;

        case 3:
            bgw_ptr->abs_x = 512;
            bgw_ptr->abs_y = 0;
            Rewrite_End_Message(3);
            break;

        case 4:
            effect_E6_init(0xB);
            Rewrite_End_Message(4);
            break;

        case 5:
            effect_E6_init(0xC);
            effect_E6_init(0xD);
            Rewrite_End_Message(5);
            break;
        }

    case 1:
        break;
    }
}

void end_500_0001() {
    bgw_ptr = bg_w.bgw;

    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        bgw_ptr->xy[0].disp.pos = end_5_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_5_pos[end_w.r_no_2][1];
        bgw_ptr->frame_deff = 0;
        bgw_ptr->free = necro_quake_timer[bgw_ptr->frame_deff];

        switch (end_w.r_no_2) {
        case 1:
            effect_E6_init(9);
            Rewrite_End_Message(1);
            break;

        case 2:
            effect_E6_init(0xA);
            Rewrite_End_Message(2);
            break;
        }

        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->frame_deff++;
            bgw_ptr->frame_deff &= 0x1F;
            bgw_ptr->free = necro_quake_timer[bgw_ptr->frame_deff];
            bgw_ptr->xy[0].disp.pos = end_5_pos[end_w.r_no_2][0];
            bgw_ptr->xy[0].disp.pos += necro_quake_tbl[bgw_ptr->frame_deff][0];
            bgw_ptr->xy[1].disp.pos = end_5_pos[end_w.r_no_2][1];
            bgw_ptr->xy[1].disp.pos += necro_quake_tbl[bgw_ptr->frame_deff][1];
            bgw_ptr->abs_x = 512;
            bgw_ptr->abs_x += necro_quake_tbl[bgw_ptr->frame_deff][0];
            bgw_ptr->abs_y = 0;
            bgw_ptr->abs_y += necro_quake_tbl[bgw_ptr->frame_deff][1];
        }

        break;
    }
}

void end_500_0006() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        if (Request_Fade(1) != 0) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 1:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 30;
            overwrite_panel(0xFF000000, 0x17);
        }

        break;

    case 2:
        overwrite_panel(0xFF000000, 0x17);
        break;
    }
}

void end_500_quake_y_sub() {
    bgw_ptr->frame_deff--;

    if (bgw_ptr->frame_deff <= 0) {
        bgw_ptr->frame_deff = 4;
        bg_w.quake_y_index++;
        bg_w.quake_y_index &= 7;
        bgw_ptr->xy[1].disp.pos += end_500_quake_tbl[bg_w.quake_y_index];
        bgw_ptr->abs_y += end_500_quake_tbl[bg_w.quake_y_index];
    }
}

void end_500_0007() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFF000000, 0x17);
        bgw_ptr->r_no_1++;
        Bg_Off_W(1);
        bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 768;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        effect_E6_init(0xF);
        effect_E6_init(0x49);
        break;

    case 1:
        break;

    case 2:
        bgw_ptr->r_no_1++;
        bgw_ptr->free = 0x28;
        break;

    case 3:
        bgw_ptr->free--;

        if (bgw_ptr->free < 1) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 4:
        end_5_bg0_move_sub();

        if (bgw_ptr->xy[0].disp.pos < 193) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x11);
            bg_w.quake_y_index = 0;
            bgw_ptr->frame_deff = 4;
        }

        break;

    case 5:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -127) {
            bgw_ptr->r_no_1++;
            end_w.timer = 0;
        }

        break;
    }
}

void end_500_0008() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        /* fallthrough */

    case 1:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -240) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x12);
        }

        break;

    case 2:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -624) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x14);
        }

        break;

    case 3:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -1008) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x15);
        }

        break;

    case 4:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -1376) {
            bgw_ptr->r_no_1++;
            end_etc_flag = 0;
            effect_E6_init(0x13);
        }

        break;

    case 5:
        end_5_bg0_move_sub();
        end_500_quake_y_sub();

        if (bgw_ptr->xy[0].disp.pos < -1472) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 6:
        end_5_bg0_move_sub();

        if (bgw_ptr->xy[0].disp.pos < -1855) {
            bgw_ptr->r_no_1++;
            end_w.timer = 18;
        }

        break;
    }
}

void end_500_0011() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        Rewrite_End_Message(6);
        bgw_ptr->r_no_1++;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        break;

    case 1:
        end_X_com01();
        break;
    }
}

void end_5_bg0_move_sub() {
    if (end_w.r_no_2 == 7) {
        bgw_ptr->xy[0].cal += 0xFFF60000;
    } else {
        bgw_ptr->xy[0].cal += 0xFFFA0000;
    }

    bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
}

void end_5_bg1_move_sub() {
    bgw_ptr->xy[0].cal += 0xFFF00000;
    bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;

    if (bgw_ptr->xy[0].disp.pos < 193) {
        if (bgw_ptr->xy[1].disp.pos == 768) {
            bgw_ptr->xy[0].disp.pos = bgw_ptr->abs_x = 448;
            bgw_ptr->xy[1].disp.pos = 512;
            return;
        }

        bgw_ptr->xy[0].disp.pos = bgw_ptr->abs_x = 704;
        bgw_ptr->xy[1].disp.pos = 768;
    }
}

void end_5_bg1_move_sub2() {
    bgw_ptr->xy[0].cal += 0x180000;
    bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;

    if (bgw_ptr->xy[1].disp.pos == 768) {
        if (bgw_ptr->xy[0].disp.pos >= 704) {
            bgw_ptr->xy[0].disp.pos = bgw_ptr->abs_x = 192;
            bgw_ptr->xy[1].disp.pos = 512;
        }
    } else if (bgw_ptr->xy[0].disp.pos >= 448) {
        bgw_ptr->xy[0].disp.pos = bgw_ptr->abs_x = 192;
        bgw_ptr->xy[1].disp.pos = 768;
    }
}

void end_501_move() {
    void (*end_501_jp[13])() = { end_X_com01,  end_X_com01,  end_X_com01,  end_X_com01,  end_X_com01,
                                 end_X_com01,  end_X_com01,  end_501_0007, end_501_0008, end_501_0009,
                                 end_501_0010, end_501_0011, end_501_0012 };
    bgw_ptr = &bg_w.bgw[1];
    end_501_jp[end_w.r_no_2]();
}

void end_501_0007() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFF000000, 0x17);
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 768;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bdl_index = 0;
        wr5_index = 0;
        end_5_flag = 512;
        /* fallthrough */

    case 1:
        if (Request_Fade(0)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 2:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        /* fallthrough */

    case 3:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            bg_w.bgw[0].r_no_1 = 2;
        }

        /* fallthrough */

    case 4:
        end_5_bg1_move_sub();
        break;
    }
}

void end_501_0008() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        /* fallthrough */

    case 1:
        end_5_bg1_move_sub();
        break;
    }
}

void end_501_0009() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        wr5_index--;
        break;

    case 1:
        end_5_bg1_move_sub2();

        if (end_etc_flag) {
            bgw_ptr->r_no_1++;
            end_w.timer = 0;
        }

        break;
    }
}

void end_501_0010() {
    end_5_bg1_move_sub2();

    switch (bgw_ptr->r_no_1) {
    case 0:
        if (Request_Fade(3)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 1:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
            end_w.timer = 30;
            overwrite_panel(0xFFFFFFFF, 0x17);
        }

        break;

    case 2:
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_501_0011() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = 256;
        bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        effect_E6_init(0x10);
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

        /* fallthrough */

    case 3:
        bgw_ptr->xy[1].cal += 0x40000;

        if (bgw_ptr->xy[1].disp.pos >= 256) {
            bgw_ptr->xy[1].disp.pos = 0;
        }

        break;
    }
}

void end_501_0012() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        effect_E6_init(0xE);
        end_fade_flag = 1;
        end_fade_timer = timer_5_tbl[end_w.r_no_2] - 120;
        /* fallthrough */

    case 1:
        bgw_ptr->xy[1].cal += 0x40000;

        if (bgw_ptr->xy[1].disp.pos >= 256) {
            bgw_ptr->xy[1].disp.pos = 0;
        }

        break;
    }
}
