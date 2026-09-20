/**
 * @file end_04.c
 * Dudley's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/effect/effh1.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

void end_400_move();
void end_401_move();
void end_402_move();

void end_400_0000();
void end_400_1000();

void end_401_0000();
void end_401_1000();
void end_401_2000();
void end_401_3000();
void end_401_4000();

void end_402_0000();
void end_402_1000();

const s16 timer_4_tbl[5] = { 960, 1140, 1800, 600, 780 };

const s16 end_4_pos[5][2] = { { 576, 768 }, { 512, 256 }, { 512, 768 }, { 512, 512 }, { 512, 256 } };

void end_04000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        end_w.timer = timer_4_tbl[end_w.r_no_2];
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

            if (end_w.r_no_2 == 2) {
                end_no_cut = 1;
            }

            end_w.timer = timer_4_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_400_move();
        end_401_move();
        end_402_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void (*end_400_jp[5])() = { end_400_0000, end_400_1000, end_X_com01, end_X_com01, end_X_com01 };

void end_400_move() {
    bgw_ptr = &bg_w.bgw[0];
    end_400_jp[end_w.r_no_2]();
}

void end_400_0000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Rewrite_End_Message(1);
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = 720;
        bgw_ptr->xy[1].disp.pos = end_4_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        bgw_ptr->speed_x = 0x8000;
        break;

    case 1:
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

        if (bgw_ptr->xy[0].disp.pos < 416) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[0].cal = 0x1A00000;
        }

        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;
    }
}

/* Follow a move with the absolute position the scroller reads. */
static void end_04_commit_position() {
    bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
    bgw_ptr->abs_y = bgw_ptr->xy[1].disp.pos;
}

/* Open a scene: step the state and put the panel where this scene starts. */
static void end_04_open_scene() {
    bgw_ptr->r_no_1++;
    bgw_ptr->xy[0].disp.pos = end_4_pos[end_w.r_no_2][0];
    bgw_ptr->xy[1].disp.pos = end_4_pos[end_w.r_no_2][1];
}

/* The hold before the drift: when it runs out, set the drift speeds and the drift's
 * own timer. Reports whether it fired, for the one caller that wrote a break there.
 * The three scenes differ only in the two speeds. */
static s32 end_04_launch_drift(s32 speed_x, s32 speed_y) {
    bgw_ptr->free--;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->r_no_1++;
        bgw_ptr->speed_x = speed_x;
        bgw_ptr->speed_y = speed_y;
        bgw_ptr->free = 0xF0;
        return 1;
    }

    return 0;
}

/* Drift the panel along its set speeds until the drift's timer runs out. */
static void end_04_drift_until_done() {
    bgw_ptr->free--;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->r_no_1++;
    } else {
        bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;
        bgw_ptr->xy[1].cal += bgw_ptr->speed_y;
    }

    end_04_commit_position();
}

void end_400_1000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Rewrite_End_Message(2);
        bgw_ptr->free = 0x12C;
        bgw_ptr->xy[0].disp.pos = end_4_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_4_pos[end_w.r_no_2][1];
        end_04_commit_position();
        break;

    case 1:
        end_04_launch_drift(0x5000, 0x3000);

        break;

    case 2:
        end_04_drift_until_done();
        break;

    case 3:
        break;
    }
}

void (*end_401_jp[5])() = { end_401_0000, end_401_1000, end_401_2000, end_401_3000, end_401_4000 };

void end_401_move() {
    bgw_ptr = &bg_w.bgw[1];
    end_401_jp[end_w.r_no_2]();
}

/* Slide the panel left until it reaches its mark. */
static void end_04_slide_to_mark() {
    bgw_ptr->xy[0].cal -= bgw_ptr->speed_x;

    if (bgw_ptr->xy[0].disp.pos < 161) {
        bgw_ptr->r_no_1++;
    }

    bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
}

/* The effects each slide scene spawns as it opens. The 402 scene spawns none,
 * which the empty function says outright. */
static void spawn_end_401_0000_effects(void) {
    effect_E6_init(0x5D);
    effect_H1_init();
}

static void spawn_end_402_0000_effects(void) {}

/* The slide scene end_401_0000 and end_402_0000 share: open, spawn whatever the
 * scene spawns, set the slide speed, and slide to the mark. */
static void run_end_04_slide_scene(void (*spawn_effects)(void), s32 speed_x) {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_04_open_scene();
        spawn_effects();
        bgw_ptr->speed_x = speed_x;
        bgw_ptr->abs_x = bgw_ptr->xy[0].disp.pos;
        break;

    case 1:
        end_04_slide_to_mark();
        break;

    case 2:
        break;
    }
}

void end_401_0000() {
    run_end_04_slide_scene(spawn_end_401_0000_effects, 0xC000);
}

/* How each drift scene launches its drift. They differ in the vertical speed,
 * and 402 still tests the launch's result although both paths leave the arm at
 * once; each keeps its own line rather than being rewritten into the other. */
static void launch_end_401_1000_drift(void) {
    end_04_launch_drift(0xA000, 0x6000);
}

static void launch_end_402_1000_drift(void) {
    if (end_04_launch_drift(0xA000, 0x4000)) {
        return;
    }
}

/* The drift scene end_401_1000 and end_402_1000 share: open and spawn the
 * scene's effect, launch the drift, then let it run out. */
static void run_end_04_drift_scene(u8 effect_id, void (*launch_drift)(void)) {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_04_open_scene();
        effect_E6_init(effect_id);
        bgw_ptr->free = 0x12C;
        end_04_commit_position();
        break;

    case 1:
        launch_drift();
        break;

    case 2:
        end_04_drift_until_done();
        break;

    case 3:
        break;
    }
}

void end_401_1000() {
    run_end_04_drift_scene(0x5F, launch_end_401_1000_drift);
}

/* Step the frame zoom in one notch every `interval` frames, and end the scene once it
 * has run all the way. The three scenes that do this differed only in the interval
 * and in the timer they leave behind. */
static void end_04_step_frame_zoom(s16 interval, s16 end_timer) {
    bg_w.frame_vol--;

    if (bg_w.frame_vol <= 0) {
        bg_w.frame_vol = interval;
        bg_w.frame_deff--;

        if (bg_w.frame_deff < 0) {
            bgw_ptr->r_no_1++;
            end_w.timer = end_timer;
            return;
        }

        Frame_Down(bg_w.center_x, bg_w.center_y, 1);
    }
}

/* Start the frame zoom at a centre, with the number of notches it has to run. The
 * three scenes that do this differed only in those three values. */
static void end_04_begin_frame_zoom(s16 center_x, s16 center_y, s16 notches) {
    bg_w.center_x = center_x;
    bg_w.center_y = center_y;
    Frame_Up(bg_w.center_x, bg_w.center_y, 0x1E);
    bg_w.frame_deff = notches;
    bg_w.frame_vol = 1;
}

void end_401_2000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(2);
        Rewrite_End_Message(0);
        bgw_ptr->xy[0].disp.pos = end_4_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_4_pos[end_w.r_no_2][1];
        bgw_ptr->abs_x = 512;
        bgw_ptr->abs_y = 0;
        end_04_begin_frame_zoom(0xC0, 0x70, 0x33);
        break;

    case 1:
        bgw_ptr->r_no_1++;
        end_no_cut = 0;
        /* fallthrough */

    case 2:
        end_04_step_frame_zoom(1, 0);

        break;

    case 3:
        break;
    }
}

void end_401_3000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_04_open_scene();
        break;

    case 1:
        bgw_ptr->r_no_1++;
        Zoomf_Init();
        end_04_begin_frame_zoom(176, 128, 48);
        /* fallthrough */

    case 2:
        end_04_step_frame_zoom(1, 0);

        break;

    case 3:
        break;
    }
}

void end_401_4000() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        end_04_open_scene();
        end_fade_flag = 1;
        end_fade_timer = timer_4_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->r_no_1++;
        Zoomf_Init();
        end_04_begin_frame_zoom(160, 128, 0x2E);
        /* fallthrough */

    case 2:
        end_04_step_frame_zoom(2, 60);

        break;

    case 3:
        break;
    }
}

void (*end_402_jp[5])() = { end_402_0000, end_402_1000, end_X_com01, end_X_com01, end_X_com01 };

void end_402_move() {
    bgw_ptr = &bg_w.bgw[2];
    end_402_jp[end_w.r_no_2]();
}

void end_402_0000() {
    run_end_04_slide_scene(spawn_end_402_0000_effects, 0x8000);
}

void end_402_1000() {
    run_end_04_drift_scene(0x5E, launch_end_402_1000_drift);
}
