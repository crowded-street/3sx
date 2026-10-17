/**
 * @file bg340.c
 * Ibuki's stage from New Generation (BG110 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg340.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff20.h"
#include "sf33rd/Source/Game/effect/ng/eff82.h"
#include "sf33rd/Source/Game/effect/ng/eff83.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define INTRO_ZOOM 80
#define INTRO_ZOOM_X 192
#define INTRO_ZOOM_Y 224

typedef struct LayerStep {
    s8 frame;
    s8 time;
} LayerStep;

typedef struct LayerAnim {
    const LayerStep* steps;
    s8 step_count;
} LayerAnim;

enum {
    ANIM_SWAY,       // NG effect 48
    ANIM_LANTERN_10, // NG effect 14, variant 10. Shown together with ANIM_SWAY, whose chips it overlaps.
    ANIM_WATERFALL,  // NG effect 14, variants 4-8
    ANIM_LANTERN_9,  // NG effect 14, variant 9
    ANIM_COUNT,
};

static const LayerStep sway_steps[] = { { 0, 11 }, { 1, 11 }, { 2, 10 }, { 1, 9 },  { 0, 10 }, { 1, 11 }, { 0, 11 },
                                        { 1, 10 }, { 2, 9 },  { 3, 9 },  { 2, 9 },  { 3, 9 },  { 2, 9 },  { 1, 10 },
                                        { 0, 11 }, { 1, 11 }, { 0, 11 }, { 1, 11 }, { 0, 11 }, { 1, 11 } };
static const LayerStep waterfall_steps[] = { { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 }, { 5, 4 } };
static const LayerStep lantern_9_steps[] = { { 0, 4 }, { 1, 4 }, { 0, 4 }, { 1, 4 }, { 0, 2 }, { 1, 2 }, { 0, 4 },
                                             { 1, 4 }, { 0, 5 }, { 1, 3 }, { 0, 4 }, { 1, 4 }, { 0, 4 }, { 1, 4 },
                                             { 0, 6 }, { 1, 5 }, { 0, 4 }, { 1, 4 }, { 0, 4 }, { 1, 4 }, { 0, 5 },
                                             { 1, 4 }, { 0, 4 }, { 1, 4 }, { 0, 6 }, { 1, 6 } };
static const LayerStep lantern_10_steps[] = { { 0, 4 }, { 1, 4 }, { 0, 4 }, { 1, 4 }, { 0, 3 }, { 1, 3 }, { 0, 2 },
                                              { 1, 3 }, { 0, 4 }, { 1, 5 }, { 0, 4 }, { 1, 4 }, { 0, 4 }, { 1, 4 },
                                              { 0, 2 }, { 1, 3 }, { 0, 4 }, { 1, 5 }, { 0, 4 }, { 1, 4 }, { 0, 4 },
                                              { 1, 4 }, { 0, 5 }, { 1, 6 }, { 0, 5 }, { 1, 4 } };

static const LayerAnim anims[ANIM_COUNT] = {
    [ANIM_SWAY] = { sway_steps, SDL_arraysize(sway_steps) },
    [ANIM_LANTERN_10] = { lantern_10_steps, SDL_arraysize(lantern_10_steps) },
    [ANIM_WATERFALL] = { waterfall_steps, SDL_arraysize(waterfall_steps) },
    [ANIM_LANTERN_9] = { lantern_9_steps, SDL_arraysize(lantern_9_steps) },
};

static void bg3400();
static void bg3400_init00();
static void bg3400_move00();
static void bg3401();
static void bg3401_init00();
static void bg3401_move00();
static void bg3401_move01();
static void bg3402();
static void bg3402_init00();
static void bg3404();
static void bg3404_init00();
static void bg3404_move00();
static void bg340_init_layer();
static void bg340_move_layer();
static void bg340_anims_init();
static void bg340_anims_update();
static bool is_cpu_ibuki_present();

void BG340() {
    bgw_ptr = &bg_w.bgw[1];
    bg3401();
    bgw_ptr = &bg_w.bgw[0];
    bg3400();
    bgw_ptr = &bg_w.bgw[2];
    bg3402();
    bgw_ptr = &bg_w.bgw[4];
    bg3404();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3400() {
    void (*bg3400_jmp[2])() = { bg3400_init00, bg3400_move00 };
    bg3400_jmp[bgw_ptr->r_no_0]();
}

static void bg3400_init00() {
    bg340_init_layer();
}

static void bg3400_move00() {
    bg340_move_layer();

    bg340_anims_update();
}

static void bg3401() {
    void (*bg3401_jmp[3])() = { bg3401_init00, bg3401_move00, bg3401_move01 };
    bg3401_jmp[bgw_ptr->r_no_0]();
}

static void bg3401_init00() {
    bg340_init_layer();
    effect_05_init();
    effect_06_init();

    bg340_anims_init(); // NG effects 48 and 14

    switch (bg_w.area) {
    case 0:
        effect_ng82_init(); // Spectators

        if (is_cpu_ibuki_present()) {
            bg_w.bg_f_x = bg_w.bg_f_y = INTRO_ZOOM;
            Frame_Down(INTRO_ZOOM_X, INTRO_ZOOM_Y, INTRO_ZOOM - 64);
        } else {
            bgw_ptr->r_no_0++;
        }

        break;

    case 1:
        bgw_ptr->r_no_0++;
        effect_ng82_init(); // Spectators
        effect_ng83_init(); // Clouds
        effect_ng94_init_for_players(0);
        break;

    case 2:
        bgw_ptr->r_no_0++;
        effect_ng94_init_for_players(1);
        effect_ng83_init(); // Clouds
        effect_ng20_init(); // Falling leaves
        break;
    }
}

static void bg3401_move00() {
    if (C_No[1] <= 3 && !Allow_a_battle_f) {
        return;
    }

    bg_w.bg_f_x--;
    bg_w.bg_f_y--;

    if (bg_w.bg_f_x > 64) {
        Frame_Up(INTRO_ZOOM_X, INTRO_ZOOM_Y, 1);
        return;
    }

    bgw_ptr->r_no_0++;
    Zoomf_Init();
    bg_w.bg_f_x = bg_w.bg_f_y = 64;
}

static void bg3401_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3402() {
    void (*bg3402_jmp[2])() = { bg3402_init00, bg340_move_layer };
    bg3402_jmp[bgw_ptr->r_no_0]();
}

static void bg3402_init00() {
    bg340_init_layer();
}

static void bg3404() {
    void (*bg3404_jmp[2])() = { bg3404_init00, bg3404_move00 };
    bg3404_jmp[bgw_ptr->r_no_0]();
}

static void bg3404_init00() {
    bg340_init_layer();
    bgw_ptr->fam_no = 4;
    bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xA0;
    bgw_ptr->pos_y_work = 0;
    bgw_ptr->xy[1].disp.pos = 0;
    bgw_ptr->speed_x = 0xE000;
    bgw_ptr->speed_y = 0x10000;
    sync_fam_set3(4);
}

static void bg3404_move00() {
    bg_x_move_check();
    bg_y_move_check();
    sync_fam_set3(4);
}

static void bg340_init_layer() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg340_move_layer() {
    bg_x_move_check();
    bg_y_move_check();
}

static bool is_cpu_ibuki_present() {
    for (int i = 0; i < 2; i++) {
        if (plw[i].player_number == CHAR_IBUKI && !plw[i].wu.operator) {
            return true;
        }
    }

    return false;
}

static s8 anim_time(int anim) {
    const s8 speed_up[3] = { 0, 2, 3 };
    const s8 time = anims[anim].steps[bg_w.layer_frame_step[anim]].time;

    // NG's effect 48 runs faster in later areas
    return (anim == ANIM_SWAY) ? time - speed_up[bg_w.area] : time;
}

static s8 anim_frame(int anim) {
    return anims[anim].steps[bg_w.layer_frame_step[anim]].frame;
}

static void anim_update(int anim) {
    if (--bg_w.layer_frame_timer[anim] != 0) {
        return;
    }

    bg_w.layer_frame_step[anim] = (bg_w.layer_frame_step[anim] + 1) % anims[anim].step_count;
    bg_w.layer_frame_timer[anim] = anim_time(anim);
}

static void set_layer_frames() {
    bg_w.layer_frame[ANIM_SWAY] = anim_frame(ANIM_SWAY);
    bg_w.layer_frame[ANIM_LANTERN_10] = anim_frame(ANIM_SWAY) + 4 * anim_frame(ANIM_LANTERN_10);
    bg_w.layer_frame[ANIM_WATERFALL] = anim_frame(ANIM_WATERFALL);
    bg_w.layer_frame[ANIM_LANTERN_9] = anim_frame(ANIM_LANTERN_9);
}

static void bg340_anims_init() {
    for (int i = 0; i < ANIM_COUNT; i++) {
        bg_w.layer_frame_step[i] = 0;
        bg_w.layer_frame_timer[i] = anim_time(i);
    }

    set_layer_frames();
}

static void bg340_anims_update() {
    if (EXE_flag || Game_pause) {
        return;
    }

    anim_update(ANIM_SWAY);

    // NG's effect 14 also stops while these backgrounds are up
    if (!aku_flag && akebono_flag != 1) {
        anim_update(ANIM_LANTERN_10);
        anim_update(ANIM_WATERFALL);
        anim_update(ANIM_LANTERN_9);
    }

    set_layer_frames();
}
