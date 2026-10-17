/**
 * @file eff67.c
 * Falling leaf of Ibuki's stage from New Generation, dropped by effect 20
 */

#include "sf33rd/Source/Game/effect/ng/eff67.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff20.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define GROUP_COUNT 2
#define COLOR_CODE 0x212C // NG 0x2040
#define CHAR_BASE 2
#define LIE_TIME 50
#define MIN_X 0x80
#define MAX_X 0x3A0

/// Where a leaf starts
typedef struct LeafStart {
    s16 x;
    s16 y;
    s16 priority;
    s16 fall_chance; // Out of 32. Otherwise the leaf's vertical speed and acceleration are made positive.
    s16 ground_y;
    s16 char_index;
} LeafStart;

typedef struct LeafFall {
    s32 speed;
    s32 accel;
} LeafFall;

static const s16 leaf_families[GROUP_COUNT] = { 2, 1 };

static const LeafStart leaf_starts[GROUP_COUNT][8] = {
    { { 208, 240, 22, 29, 34, 0 },
      { 192, 256, 73, 29, 62, 1 },
      { 204, 224, 22, 30, 32, 0 },
      { 215, 288, 22, 30, 30, 0 },
      { 199, 233, 73, 30, 58, 1 },
      { 196, 264, 22, 29, 35, 1 },
      { 193, 247, 22, 29, 31, 0 },
      { 219, 384, 73, 30, 65, 1 } },
    { { 228, 242, 93, 28, 70, 2 },
      { 224, 240, 93, 29, 81, 3 },
      { 216, 228, 93, 29, 82, 2 },
      { 211, 264, 93, 28, 83, 2 },
      { 207, 217, 93, 28, 78, 3 },
      { 184, 192, 93, 29, 85, 3 },
      { 204, 196, 93, 29, 84, 2 },
      { 208, 200, 93, 29, 80, 3 } },
};

static const LeafFall leaf_falls[GROUP_COUNT][32] = {
    { { -0xE00, -0x40 },  { -0x800, -0xF8 },  { -0x480, -0x160 }, { -0x700, -0x100 }, { -0xC00, -0xE8 },
      { -0x510, -0x118 }, { -0xA00, -0xF8 },  { -0xCE0, -0xD8 },  { -0xD40, -0xC0 },  { -0x900, -0xF0 },
      { -0xA00, -0xF0 },  { -0x200, -0x198 }, { -0x400, -0x160 }, { -0x500, -0x120 }, { -0xB00, -0x120 },
      { -0x300, -0x180 }, { -0x500, -0x160 }, { -0xB00, -0xD8 },  { -0x200, -0x168 }, { -0xA00, -0xE0 },
      { -0xDC0, -0xC0 },  { -0x300, -0x1A0 }, { -0x100, -0x1F8 }, { -0x600, -0x128 }, { -0x300, -0x160 },
      { -0x500, -0x130 }, { -0x200, -0x1D8 }, { -0x200, -0x178 }, { -0x600, -0x110 }, { -0xA00, -0xF8 },
      { -0x700, -0x118 }, { -0x700, -0xD8 } },
    { { -0xB20, -0xC0 },  { -0x540, -0xE8 },  { -0xA00, -0xD8 },  { -0x3E0, -0xD0 },  { -0x900, -0xE8 },
      { -0xA80, -0xB0 },  { -0x700, -0xF8 },  { -0x930, -0xB8 },  { -0xC00, -0xA0 },  { -0x600, -0xD0 },
      { -0x700, -0xF0 },  { -0xF0, -0x180 },  { -0x100, -0x1C8 }, { -0x270, -0x100 }, { -0x800, -0x100 },
      { -0x160, -0x170 }, { -0x200, -0x168 }, { -0x790, -0xC8 },  { -0x80, -0x1D0 },  { -0x690, -0xD8 },
      { -0xC00, -0xA8 },  { -0x1C0, -0x158 }, { -0xC0, -0x1C8 },  { -0x320, -0x118 }, { -0x120, -0x160 },
      { -0x250, -0xF0 },  { -0x40, -0x1D8 },  { -0x68, -0x1A0 },  { -0x380, -0x120 }, { -0x188, -0x168 },
      { -0x440, -0x110 }, { -0x428, -0x108 } },
};

static const s32 leaf_speeds_x[64] = {
    0xC040, 0xC800, 0xD200,  0xD800, 0xC000, 0xB400, 0xCE00, 0xD000, 0xD200, 0xD600, 0xD800, 0xDA00, 0xDE00,
    0xE200, 0xF800, 0x10000, 0x6440, 0x6800, 0x6200, 0x6800, 0x6000, 0x5400, 0x6E00, 0x7000, 0x7200, 0x7600,
    0x7800, 0x7A00, 0x7E00,  0x8200, 0x8400, 0x8800, 0x380,  0x500,  0x6C0,  0x640,  0x600,  0x540,  0x580,
    0x5E0,  0x400,  0x440,   0x480,  0x5A0,  0x6E0,  0x800,  0x940,  0xC80,  0x80,   0x200,  0x3C0,  0x340,
    0x300,  0x240,  0x280,   0x2E0,  0x100,  0x140,  0x180,  0x3A0,  0x6E0,  0x800,  0x840,  0xA80,
};

#define LEAF_ACCELS_X (&leaf_speeds_x[32])

static const s32 leaf_brakes_x[GROUP_COUNT * 16] = {
    -0x220, -0x200, -0x1E0, -0x1C0, -0x1A0, -0x180, -0x160, -0x140, -0x120, -0x100, -0xE0,
    -0xC0,  -0xA0,  -0x80,  -0x60,  -0x40,  -0x28,  -0x30,  -0x40,  -0x50,  -0x60,  -0x70,
    -0x80,  -0xA0,  -0xC0,  -0x100, -0x130, -0x140, -0x150, -0x160, -0x170, -0x180,
};

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void set_fall(WORK_Other* ewk, const LeafFall* fall, s32 roll, bool set_speed) {
    if (roll < ewk->wu.vitality) {
        if (set_speed) {
            ewk->wu.mvxy.a[1].sp = fall->speed;
        }

        ewk->wu.mvxy.d[1].sp = fall->accel;
    } else {
        if (set_speed) {
            ewk->wu.mvxy.a[1].sp = SDL_abs(fall->speed);
        }

        ewk->wu.mvxy.d[1].sp = SDL_abs(fall->accel);
    }
}

static void set_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0x3FF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0x3FF;
}

static void flutter(WORK_Other* ewk) {
    const s16 group = ewk->wu.type;
    s32 roll;

    if (--ewk->wu.vital_new != 0) {
        return;
    }

    roll = random_32();
    ewk->wu.vital_new = (roll < 4) ? 16 : roll;

    roll = random_32();
    set_fall(ewk, &leaf_falls[group][roll], roll, false);

    if (random_16() < 7) {
        ewk->wu.mvxy.d[0].sp = leaf_brakes_x[group * 16 + random_16()];
    } else {
        ewk->wu.mvxy.d[0].sp = LEAF_ACCELS_X[group * 16 + random_16()];
    }
}

bool effect_ng67_is_hidden(WORK_Other* ewk) {
    return aku_flag || (akebono_flag == 1 && ewk->wu.position_z < 0x46);
}

void effect_ng67_move(WORK_Other* ewk) {
    WORK_Other* spawner = (WORK_Other*)ewk->my_master;

    if (ewk->wu.dead_f == 1) {
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (effect_ng67_is_hidden(ewk)) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        (*effect_ng20_leaf_count(spawner, ewk->wu.type))++;
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        return;

    case 1:
        if (!is_paused()) {
            char_move(&ewk->wu);
            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);

            if (ewk->wu.xyz[0].disp.pos < MIN_X || ewk->wu.xyz[0].disp.pos > MAX_X) {
                ewk->wu.routine_no[0] = 3;
                return;
            }

            if (ewk->wu.xyz[1].disp.pos < ewk->wu.vital_old) {
                // Leaves in front vanish on the ground. The others lie there for a while.
                if (ewk->wu.type != 0) {
                    ewk->wu.routine_no[0] = 3;
                    return;
                }

                ewk->wu.routine_no[0]++;
                set_position(ewk);
            } else {
                set_position(ewk);
                flutter(ewk);
            }
        }

        break;

    case 2:
        if (--ewk->wu.dm_vital == 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            return;
        }

        break;

    case 3:
        (*effect_ng20_leaf_count(spawner, ewk->wu.type))--;
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;

    default:
        return;
    }

    sort_push_request4(&ewk->wu);
}

s32 effect_ng67_init(WORK_Other* spawner, s16 group) {
    WORK_Other* ewk;
    const LeafStart* start;
    s16 ix;
    s32 roll;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG67_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = group;
    ewk->my_master = spawner;
    ewk->master_work_id = spawner->wu.work_id;
    ewk->master_id = spawner->wu.id;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.char_table[0] = _ng_eff67_char_table;
    ewk->wu.my_family = leaf_families[group];

    start = &leaf_starts[group][random_32() & 7];
    ewk->wu.xyz[0].disp.pos = start->x;
    ewk->wu.xyz[1].disp.pos = start->y;
    set_position(ewk);
    ewk->wu.my_priority = ewk->wu.position_z = start->priority;
    ewk->wu.vitality = start->fall_chance;
    ewk->wu.vital_old = start->ground_y;
    ewk->wu.char_index = CHAR_BASE + start->char_index;
    ewk->wu.dm_vital = LIE_TIME;

    ewk->wu.vital_new = random_32() * 2;

    if (ewk->wu.vital_new == 0) {
        ewk->wu.vital_new = 16;
    }

    roll = random_32();
    set_fall(ewk, &leaf_falls[group][roll], roll, true);
    ewk->wu.mvxy.a[0].sp = leaf_speeds_x[group * 16 + random_32()];
    ewk->wu.mvxy.d[0].sp = LEAF_ACCELS_X[group * 16 + random_16()];

    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
