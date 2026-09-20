/**
 * @file end_16.c
 * Chun-Li's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

void end_1600_move();

void end_1600_0000();
void end_1600_1000();
void end_1600_2000();
void end_1600_3000();
void end_1600_3100();
void end_1600_5000();

void end_16_col_change();

const s16 timer_16_tbl[6] = { 660, 360, 360, 660, 660, 240 };

const s16 end_16_pos[7][2] = { { 256, 768 }, { 256, 768 }, { 768, 256 }, { 256, 256 },
                               { 256, 0 },   { 256, 0 },   { 768, 0 } };

void end_16000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        c_kakikae = 0;
        c_number = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x31);
        end_w.timer = timer_16_tbl[end_w.r_no_2];
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

            end_w.timer = timer_16_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
        }

        end_1600_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1600_move() {
    void (*end_1600_move_jp[6])() = { end_1600_0000, end_1600_1000, end_1600_2000,
                                      end_1600_3000, end_1600_3100, end_1600_5000 };
    bgw_ptr = &bg_w.bgw[0];
    end_1600_move_jp[end_w.r_no_2]();
}

void end_1600_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        Rewrite_End_Message(1);
        break;

    case 1:
        bgw_ptr->xy[0].cal += 0x8000;

        if (bgw_ptr->xy[0].disp.pos >= 480) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1E00000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 2:
        break;
    }
}

/* Hold on the blacked-out panel, then end the scene. */
static void end_1600_1000_hold_blank() {
    overwrite_panel(0xFF000000, 0x17);
    bgw_ptr->free--;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->r_no_1++;
        end_w.timer = 0;
    }
}

void end_1600_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        /* fallthrough */

    case 1:
        if (Request_Fade(1)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            bgw_ptr->free = 10;
            overwrite_panel(0xFF000000, 0x17);
        }

        break;

    case 3:
        end_1600_1000_hold_blank();

        break;

    default:
        overwrite_panel(0xFF000000, 0x17);
        break;
    }
}

/* Open a scene: step the state and put the panel where this scene starts. */
static void end_16_open_scene() {
    bgw_ptr->r_no_1++;
    bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
    bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
}

void end_1600_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        overwrite_panel(0xFF000000, 0x17);
        end_16_open_scene();
        bgw_ptr->abs_x = 512;
        /* fallthrough */

    case 1:
        overwrite_panel(0xFF000000, 0x17);

        if (Request_Fade(0)) {
            bgw_ptr->r_no_1++;
            end_no_cut = 1;
            Rewrite_End_Message(2);
        }

        break;

    case 2:
        if (end_fade_complete()) {
            bgw_ptr->r_no_1++;
            end_no_cut = 0;
        }

        break;

    case 3:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bgw_ptr->xy[1].disp.pos >= 384) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1800000;
        }

        break;
    }
}

/* 3000 resets the colour limits as it opens; 3100 leaves them where they are,
 * which the empty function says. */
static void set_end_1600_3000_limits(void) {
    bgw_ptr->l_limit2 = 2;
    bgw_ptr->l_limit = 0;
}

static void keep_end_1600_limits(void) {}

/* The message scene end_1600_3000 and end_1600_3100 share: open, spawn the
 * scene's effect, put up its message, and start the colour change. */
static void run_end_16_message_scene(u8 effect_id, u16 message, void (*set_limits)(void), u8 kakikae) {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_16_open_scene();
        effect_E6_init(effect_id);
        Rewrite_End_Message(message);
        set_limits();
        c_kakikae = kakikae;
        break;

    case 1:
        end_16_col_change();
        break;
    }
}

void end_1600_3000() {
    run_end_16_message_scene(0x56, 3, set_end_1600_3000_limits, 1);
}

void end_1600_3100() {
    run_end_16_message_scene(0x57, 4, keep_end_1600_limits, 2);
}

void end_1600_5000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        c_kakikae = 0;
        bgw_ptr->xy[0].disp.pos = end_16_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_16_pos[end_w.r_no_2][1];
        effect_E6_init(0x58);
        Rewrite_End_Message(5);
        bgw_ptr->free = 2;
        bgw_ptr->rewrite_flag = 0;
        bgw_ptr->l_limit2 = 2;
        bgw_ptr->l_limit = 0;
        end_fade_flag = 1;
        end_fade_timer = timer_16_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->free = 2;
            bgw_ptr->rewrite_flag ^= 1;

            if (bgw_ptr->rewrite_flag) {
                bgw_ptr->xy[0].disp.pos = 768;
                break;
            }

            bgw_ptr->xy[0].disp.pos = 256;
        }

        break;
    }
}

void end_16_col_change() {
    bgw_ptr->l_limit2--;

    if (bgw_ptr->l_limit2 <= 0) {
        bgw_ptr->l_limit2 = 1;
        bgw_ptr->l_limit++;
        bgw_ptr->l_limit &= 1;
        c_number = bgw_ptr->l_limit;
    }
}
