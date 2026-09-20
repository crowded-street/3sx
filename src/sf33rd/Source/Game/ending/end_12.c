/**
 * @file end_12.c
 * Sean's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff2.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

void end_C00_move();
void end_C01_move();

void end_C00_0000();
void end_C00_1000();
void end_C00_2000();
void end_C00_3000();
void end_C00_4000();
void end_C00_5000();
void end_C00_6000();

void end_C01_0000();

const s16 timer_c_tbl[7] = { 600, 600, 360, 420, 360, 540, 420 };

const s16 end_c_pos[7][2] = { { 256, 32 },  { 768, 768 }, { 768, 768 }, { 768, 512 },
                              { 768, 768 }, { 768, 256 }, { 768, 256 } };

void end_12000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        end_w.timer = timer_c_tbl[end_w.r_no_2];
        BGM_Request(0x31);
        break;

    case 1:
        end_w.timer--;
        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 == 7) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_c_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_C00_move();
        end_C01_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_C00_move() {
    void (*end_C00_jp[7])() = { end_C00_0000, end_C00_1000, end_C00_2000, end_C00_3000,
                                end_C00_4000, end_C00_5000, end_C00_6000 };
    bgw_ptr = &bg_w.bgw[0];
    end_C00_jp[end_w.r_no_2]();
}

void end_C00_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_c_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_c_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        Rewrite_End_Message(1);
        break;

    case 1:
        if (bgw_ptr->xy[1].disp.pos > 304) {
            bgw_ptr->r_no_1++;
            effect_F2_init();
        }

        /* fallthrough */

    case 2:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bgw_ptr->xy[1].disp.pos > 384) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0x1800000;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 3:
        break;
    }
}

/* Open a scene: step the state and put the panel where this scene starts. */
static void end_12_open_scene() {
    bgw_ptr->r_no_1++;
    bgw_ptr->xy[0].disp.pos = end_c_pos[end_w.r_no_2][0];
    bgw_ptr->xy[1].disp.pos = end_c_pos[end_w.r_no_2][1];
}

/* What the three C00 message scenes do and do not do around their message: only
 * 1000 places the panel, each spawns its own effects, and 5000 leaves the colour
 * limits alone. The empty functions say the "does not" cases outright. */
static void place_end_C00_1000_panel(void) {
    bgw_ptr->abs_x = 512;
    bgw_ptr->abs_y = 0;
}

static void keep_end_C00_panel(void) {}

static void spawn_end_C00_1000_effects(void) {
    effect_E6_init(0x39);
}

static void spawn_end_C00_4000_effects(void) {
    effect_E6_init(0x3B);
}

static void spawn_end_C00_5000_effects(void) {
    effect_E6_init(0x3C);
    effect_E6_init(0x99);
}

static void reset_end_C00_colour_limits(void) {
    bgw_ptr->free = 0;
    bgw_ptr->l_limit = 3;
}

static void keep_end_C00_colour_limits(void) {}

typedef struct {
    void (*place_panel)(void);
    void (*spawn_effects)(void);
    u16 message;
    void (*set_colour_limits)(void);
} End12SceneOps;

/* The message scene end_C00_1000, end_C00_4000 and end_C00_5000 share. */
static void run_end_12_message_scene(const End12SceneOps* ops) {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_12_open_scene();
        ops->place_panel();
        ops->spawn_effects();
        Rewrite_End_Message(ops->message);
        ops->set_colour_limits();
        break;

    case 1:
        break;
    }
}

void end_C00_1000() {
    run_end_12_message_scene(&(End12SceneOps) {
        place_end_C00_1000_panel, spawn_end_C00_1000_effects, 2, reset_end_C00_colour_limits });
}

void end_C00_2000() {
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
            end_w.timer = 0x14;
            overwrite_panel(0xFFFFFFFF, 0x17);
        }

        break;

    default:
        overwrite_panel(0xFFFFFFFF, 0x17);
        break;
    }
}

void end_C00_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        overwrite_panel(0xFFFFFFFF, 0x17);
        bgw_ptr->xy[0].disp.pos = end_c_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_c_pos[end_w.r_no_2][1];
        Rewrite_End_Message(3);
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

        break;
    }
}

void end_C00_4000() {
    run_end_12_message_scene(&(End12SceneOps) {
        keep_end_C00_panel, spawn_end_C00_4000_effects, 4, reset_end_C00_colour_limits });
}

void end_C00_5000() {
    run_end_12_message_scene(&(End12SceneOps) {
        keep_end_C00_panel, spawn_end_C00_5000_effects, 5, keep_end_C00_colour_limits });
}

void end_C00_6000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_12_open_scene();
        bgw_ptr->free = 0x3C;
        end_fade_flag = 1;
        end_fade_timer = timer_c_tbl[end_w.r_no_2] - 120;
        Rewrite_End_Message(6);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->r_no_1++;
            effect_E6_init(0x3D);
        }

        break;

    case 2:
        break;
    }
}

void end_C01_move() {
    void (*end_c01_jp[7])() = { end_C01_0000, end_X_com01, end_X_com01, end_X_com01,
                                end_X_com01,  end_X_com01, end_X_com01 };
    bgw_ptr = &bg_w.bgw[1];
    end_c01_jp[end_w.r_no_2]();
}

void end_C01_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        bgw_ptr->xy[0].disp.pos = end_c_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_c_pos[end_w.r_no_2][1];
        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0x10000;

        if (bg_w.bgw[0].r_no_1 >= 3) {
            bgw_ptr->r_no_1++;
        }

        bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
        break;

    case 2:
        break;
    }
}
