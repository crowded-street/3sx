/**
 * @file eff23.c
 * Bird seller's stall of Yun's stage from New Generation (effects 21-24 in the NG arcade ROM). Effect 23 is a stack of
 * bird cages that breaks when a knocked down player hits it. It spawns the birds in the cages (effect 21), a bird that
 * sits on top of them (effect 22) and the seller sitting on a cage (effect 24). Once the cages break, the birds fly
 * away and the seller jumps up. The stall stays broken for the rest of the match.
 */

#include "sf33rd/Source/Game/effect/ng/eff23.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define HIT_TYPE 8
#define CAGE_BIRD_CHAR_BASE 36
#define TOP_BIRD_CHAR_BASE 42
#define CAGES_COLOR_CODE 0x212C   // NG 0x2040
#define OBJECTS_COLOR_CODE 0x2141 // NG 0x2055

static const s16 cages_hit_box[4] = { 12, 61, 16, 28 };

typedef struct CageBird {
    s16 char_index;
    s16 x;
    s16 y;
    s16 priority;
    s16 rl_flag;
    s32 speed_x;
    s32 speed_y;
    s32 accel_x;
    s32 accel_y;
} CageBird;

static const CageBird cage_birds[3] = {
    { 0, 696, 64, 74, 0, 0, 0, -0x1000, 0x480 },
    { 2, 752, 84, 74, 0, 0, 0, -0x1800, 0x400 },
    { 4, 752, 63, 74, 1, 0x10000, 0, 0x2400, 0x480 },
};

static bool is_broken() {
    return eff_hit_flag[HIT_TYPE] != 0;
}

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void set_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0x3FF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0x3FF;
}

/// Like add_x_sub and add_y_sub, but accelerates before moving, as NG does
static void add_xy(WORK_Other* ewk) {
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;
}

/// Draws without the 1 pixel nudge of disp_pos_trans_entry and with NG's range
static void trans_entry(WORK_Other* ewk) {
    if (obr_no_disp_check() || !range_x_check_ng(ewk)) {
        return;
    }

    set_position(ewk);
    sort_push_request4(&ewk->wu);
}

static void init_common(WORK_Other* ewk, s16 id, WORK_Other* master) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = id;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = OBJECTS_COLOR_CODE;
    ewk->wu.char_table[0] = _ng_hkg_a_char_table;
    ewk->my_master = master;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
}

static void set_xyz(WORK_Other* ewk, s16 x, s16 y, s16 priority) {
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = x;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = y;
    ewk->wu.my_priority = ewk->wu.position_z = priority;
}

// MARK: - Cage birds (effect 21)

void effect_ng21_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;

        if (is_broken()) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!is_paused()) {
            if (is_broken()) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, ewk->wu.char_index + 1);
            } else {
                char_move(&ewk->wu);
            }
        }

        sort_push_request4(&ewk->wu);
        break;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);
            add_xy(ewk);

            if (ewk->wu.xyz[0].disp.pos < 0x80 || ewk->wu.xyz[0].disp.pos > 0x380 || ewk->wu.xyz[1].disp.pos > 0x200) {
                ewk->wu.routine_no[0]++;
            }

            set_position(ewk);
        }

        sort_push_request4(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

static s32 effect_ng21_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < SDL_arraysize(cage_birds); i++) {
        const CageBird* bird = &cage_birds[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        init_common(ewk, EFFECT_NG21_ID, master);
        ewk->wu.type = i;
        ewk->wu.char_index = CAGE_BIRD_CHAR_BASE + bird->char_index;
        set_xyz(ewk, bird->x, bird->y, bird->priority);
        ewk->wu.rl_flag = bird->rl_flag;
        ewk->wu.mvxy.a[0].sp = bird->speed_x;
        ewk->wu.mvxy.a[1].sp = bird->speed_y;
        ewk->wu.mvxy.d[0].sp = bird->accel_x;
        ewk->wu.mvxy.d[1].sp = bird->accel_y;
    }

    return 0;
}

// MARK: - Top bird (effect 22)

void effect_ng22_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;

        if (is_broken()) {
            set_char_move_init(&ewk->wu, 0, TOP_BIRD_CHAR_BASE + 2);
            ewk->wu.routine_no[0] = 3;
        } else {
            set_char_move_init(&ewk->wu, 0, TOP_BIRD_CHAR_BASE);
        }

        return;

    case 1:
        if (!is_paused()) {
            if (is_broken()) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, TOP_BIRD_CHAR_BASE + 1);
            } else {
                char_move(&ewk->wu);
            }
        }

        break;

    case 2:
        if (!is_paused()) {
            if (ewk->wu.cg_type == 99) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, TOP_BIRD_CHAR_BASE + 2);
            } else {
                ewk->wu.direction = ewk->wu.cg_type;
                char_move(&ewk->wu);
                add_pos_dir_032(&ewk->wu, 0x300);
            }

            set_position(ewk);
        }

        break;

    case 3:
        ewk->wu.position_x = 732;
        ewk->wu.position_y = 54;
        ewk->wu.routine_no[0]++;

        /* fallthrough */

    case 4:
        if (!is_paused()) {
            char_move(&ewk->wu);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    sort_push_request4(&ewk->wu);
}

static s32 effect_ng22_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    init_common(ewk, EFFECT_NG22_ID, master);
    set_xyz(ewk, 712, 56, 74);
    return 0;
}

// MARK: - Cages (effect 23)

void effect_ng23_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (is_broken()) {
            set_char_move_init(&ewk->wu, 0, 34);
            ewk->wu.routine_no[0] = 3;
        } else {
            set_char_move_init(&ewk->wu, 0, 7);
        }

        return;

    case 1:
        if (!is_paused()) {
            if (eff_hit_check_box(ewk, 0, cages_hit_box)) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, 12);
            } else {
                char_move(&ewk->wu);
            }
        }

        break;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0]++;
            }
        }

        break;

    case 3:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    trans_entry(ewk);
}

// MARK: - Seller (effect 24)

void effect_ng24_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (is_broken()) {
            set_char_move_init(&ewk->wu, 0, 24);
            ewk->wu.routine_no[0] = 2;
        } else {
            set_char_move_init(&ewk->wu, 0, 23);
        }

        return;

    case 1:
        if (!is_paused()) {
            if (is_broken()) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, 24);
            } else {
                char_move(&ewk->wu);
            }
        }

        break;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    trans_entry(ewk);
}

static s32 effect_ng24_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    init_common(ewk, EFFECT_NG24_ID, master);
    set_xyz(ewk, 728, 48, 91);
    return 0;
}

s32 effect_ng23_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    init_common(ewk, EFFECT_NG23_ID, NULL);
    ewk->wu.old_rno[0] = 0;
    ewk->wu.type = HIT_TYPE;
    ewk->wu.my_col_code = CAGES_COLOR_CODE;
    set_xyz(ewk, 672, 48, 90);
    effect_ng21_init(ewk);
    effect_ng22_init(ewk);
    effect_ng24_init(ewk);
    return 0;
}
