/**
 * @file eff34.c
 * Falling cherry blossom petals of Ryu's stage from New Generation (effects 34 and 35 in the NG arcade ROM). Effect 34
 * spawns the petals (effect 35) in two groups: near ones that land and lie on the ground for a while, and far ones
 * that vanish when they reach the ground.
 */

#include "sf33rd/Source/Game/effect/ng/eff34.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

#include <SDL3/SDL.h>

#define GROUP_COUNT 2
#define SPAWN_LIST_LENGTH 7
#define SPAWN_LIST_COUNT 8
#define LANDED_FRAMES 50
#define X_RANGE 0x180

static const s16 first_char_index[GROUP_COUNT] = { 2, 6 };

static const s16 spawn_lists[GROUP_COUNT][SPAWN_LIST_COUNT][SPAWN_LIST_LENGTH] = {
    { { 1, 6, 8, 3, 4, 0, 5 },
      { 2, 3, 6, 1, 0, 4, 5 },
      { 5, 10, 3, 0, 4, 2, 1 },
      { 9, 0, 2, 5, 3, 1, 6 },
      { 3, 2, 5, 1, 6, 4, 0 },
      { 6, 2, 10, 7, 1, 0, 5 },
      { 0, 1, 2, 3, 4, 5, 6 },
      { 6, 5, 4, 3, 2, 1, 0 } },
    { { 13, 14, 21, 19, 17, 12, 11 },
      { 15, 16, 13, 17, 19, 11, 18 },
      { 12, 20, 11, 16, 15, 14, 13 },
      { 14, 12, 15, 11, 21, 16, 17 },
      { 11, 18, 12, 19, 13, 17, 15 },
      { 19, 16, 13, 12, 14, 15, 18 },
      { 11, 12, 13, 14, 20, 16, 17 },
      { 20, 16, 15, 14, 13, 12, 11 } },
};

static const s16 spawn_delays[32] = { 10,  24,  28,  34,  40,  48,  60,  78,  88,  94,  108, 114, 118, 128, 134, 144,
                                      148, 154, 160, 174, 180, 188, 194, 208, 218, 228, 260, 328, 388, 400, 480, 600 };

typedef struct Eff35Data {
    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;   // Within the group's animations
    s16 right_chance; // Out of 16. The chance that a new velocity drifts right instead of left.
    s16 ground_y;
} Eff35Data;

/// Petals 0-10 are near, 11-21 far
static const Eff35Data eff35_data_tbl[22] = {
    { -288, 288, 22, 3, 15, 38 }, { -208, 304, 73, 2, 4, 65 },  { -184, 264, 22, 0, 12, 34 },
    { -120, 264, 22, 1, 23, 36 }, { -128, 408, 73, 3, 20, 62 }, { 0, 320, 22, 0, 22, 37 },
    { 32, 312, 22, 2, 23, 36 },   { -256, 272, 22, 3, 10, 38 }, { -232, 274, 73, 2, 7, 65 },
    { -169, 312, 22, 0, 16, 34 }, { -92, 296, 22, 1, 19, 36 },  { -304, 384, 90, 0, 12, 72 },
    { -264, 368, 90, 1, 16, 72 }, { -160, 240, 90, 2, 17, 72 }, { -80, 304, 90, 3, 27, 72 },
    { 0, 336, 90, 1, 19, 72 },    { 80, 344, 90, 0, 21, 72 },   { -48, 400, 90, 2, 18, 72 },
    { -288, 361, 90, 0, 15, 72 }, { -244, 377, 90, 1, 20, 72 }, { -98, 276, 90, 3, 22, 72 },
    { 36, 323, 90, 1, 16, 72 },
};

#define FIRST_FAR_PETAL 11

typedef struct Velocity {
    s32 speed;
    s32 accel;
} Velocity;

static const Velocity y_velocities[GROUP_COUNT][32] = {
    { { -0x1400, -0x480 }, { -0x1800, -0x380 }, { -0x2C00, -0x330 }, { -0x2E00, -0x310 }, { -0x3000, -0x300 },
      { -0x3200, -0x2E0 }, { -0x3600, -0x280 }, { -0x3800, -0x260 }, { -0x3A00, -0x240 }, { -0x3C00, -0x230 },
      { -0x3D00, -0x220 }, { -0x3F00, -0x200 }, { -0x4000, -0x180 }, { -0x4100, -0x160 }, { -0x4200, -0x140 },
      { -0x4E00, -0x100 }, { -0x1400, -0x100 }, { -0x1800, -0x140 }, { -0x2C00, -0x130 }, { -0x2E00, -0x110 },
      { -0x3000, -0x200 }, { -0x3200, -0x1E0 }, { -0x3600, -0x120 }, { -0x3800, -0x2E0 }, { -0x3A00, -0x180 },
      { -0x3C00, -0x130 }, { -0x3D00, -0x280 }, { -0x3F00, -0x240 }, { -0x4000, -0x1E0 }, { -0x4100, -0x200 },
      { -0x4200, -0x280 }, { -0x4E00, -0x330 } },
    { { -0x400, -0x380 },  { -0x800, -0x280 },  { -0xC00, -0x230 },  { -0xE00, -0x210 },  { -0x1000, -0x200 },
      { -0x1200, -0x1E0 }, { -0x1600, -0x180 }, { -0x1800, -0x160 }, { -0x1A00, -0x140 }, { -0x1C00, -0x130 },
      { -0x1D00, -0x120 }, { -0x1F00, -0x100 }, { -0x2000, -0x80 },  { -0x2100, -0x60 },  { -0x2200, -0x40 },
      { -0x2E00, -0x20 },  { -0x400, -0x60 },   { -0x800, -0x40 },   { -0xC00, -0x30 },   { -0xE00, -0x10 },
      { -0x1000, -0x100 }, { -0x1200, -0xE0 },  { -0x1600, -0x20 },  { -0x1800, -0x1E0 }, { -0x1A00, -0x80 },
      { -0x1C00, -0x30 },  { -0x1D00, -0x180 }, { -0x1F00, -0x140 }, { -0x2000, -0xE0 },  { -0x2100, -0x100 },
      { -0x2200, -0x180 }, { -0x2E00, -0x230 } },
};

static const s32 x_speeds[16] = { -0x40,   -0x800,  -0x1200, -0x1800, -0x2000, -0x2400, -0x2E00, -0x3000,
                                  -0x3200, -0x3600, -0x3800, -0x3A00, -0x3E00, -0x4200, -0x5800, -0x6000 };

static const s32 x_accels[2][16] = {
    { 0,
      -0x100,
      -0x2C0,
      -0x240,
      -0x300,
      -0x340,
      -0x380,
      -0x3E0,
      -0x400,
      -0x440,
      -0x480,
      -0x4A0,
      -0x4E0,
      -0x500,
      -0x540,
      -0x600 },
    { 0,
      -0x80,
      -0xC0,
      -0xD0,
      -0xE0,
      -0x120,
      -0x160,
      -0x180,
      -0x1C0,
      -0x1E0,
      -0x220,
      -0x260,
      -0x280,
      -0x300,
      -0x340,
      -0x380 },
};

static s16* group_count(WORK_Other* master, s16 group) {
    return (group == 0) ? &master->wu.direction : &master->wu.dir_old;
}

static void eff35_set_velocity(WORK_Other* ewk) {
    const Velocity* y = &y_velocities[ewk->wu.dmcal_m][random_32()];
    const s16 x = random_16();

    ewk->wu.mvxy.a[1].sp = y->speed;
    ewk->wu.mvxy.d[1].sp = y->accel;
    bool right = x < ewk->wu.vitality;
    ewk->wu.mvxy.a[0].sp = right ? SDL_abs(x_speeds[x]) : x_speeds[x];

    const s32* accels = x_accels[random_16() & 1];
    const s32 accel = accels[random_16()];
    ewk->wu.mvxy.d[0].sp = right ? SDL_abs(accel) : accel;
}

/// Like add_x_sub and add_y_sub, but accelerates before moving, as NG does
static void eff35_add_xy(WORK_Other* ewk) {
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;
}

/// Draws without the 1 pixel nudge or range check of disp_pos_trans_entry, like NG
static void eff35_disp(WORK_Other* ewk) {
    sort_push_request4(&ewk->wu);
}

static void eff35_set_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
}

void effect_ng35_move(WORK_Other* ewk) {
    WORK_Other* master = (WORK_Other*)ewk->my_master;

    if (ewk->wu.dead_f == 1) {
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (aku_flag || (akebono_flag && ewk->wu.my_priority < 70)) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        (*group_count(master, ewk->wu.dmcal_m))++;
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        return;

    case 1:
        if (EXE_flag || Game_pause) {
            break;
        }

        char_move(&ewk->wu);
        eff35_add_xy(ewk);

        if (ewk->wu.xyz[0].disp.pos < -X_RANGE || ewk->wu.xyz[0].disp.pos > X_RANGE) {
            ewk->wu.routine_no[0] += 2;
            return;
        }

        if (ewk->wu.xyz[1].disp.pos < ewk->wu.vital_old) {
            // Far petals vanish on the ground, near ones lie there for a while
            if (ewk->wu.dmcal_m != 0) {
                ewk->wu.routine_no[0] += 2;
                return;
            }

            ewk->wu.routine_no[0]++;
            eff35_set_position(ewk);
            break;
        }

        eff35_set_position(ewk);

        if (--ewk->wu.vital_new < 1) {
            ewk->wu.vital_new = random_32();

            if (ewk->wu.vital_new < 5) {
                ewk->wu.vital_new = 38;
            }

            eff35_set_velocity(ewk);
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
        (*group_count(master, ewk->wu.dmcal_m))--;
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;

    default:
        return;
    }

    eff35_disp(ewk);
}

static s32 eff35_init(WORK_Other* master, s16 type) {
    const Eff35Data* data = &eff35_data_tbl[type];
    WORK_Other* ewk;
    s16 ix;
    s16 delay;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG35_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = type;
    ewk->my_master = master;
    ewk->master_work_id = master->wu.work_id;
    ewk->master_id = master->wu.id;
    ewk->wu.my_family = 2;

    // Converted from NG's palette row 0x4E to the PS2 BG palette row 0x13A
    ewk->wu.my_col_code = 0x213A;

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = data->x;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = data->y;
    ewk->wu.my_priority = ewk->wu.position_z = data->priority;
    ewk->wu.vitality = data->right_chance;
    ewk->wu.vital_old = data->ground_y;
    ewk->wu.dm_vital = LANDED_FRAMES;
    ewk->wu.dmcal_m = (type < FIRST_FAR_PETAL) ? 0 : 1;
    ewk->wu.char_index = first_char_index[ewk->wu.dmcal_m] + data->char_index;
    ewk->wu.char_table[0] = _ng_j10_a_char_table;

    delay = random_32() & 0x1C;
    ewk->wu.vital_new = (delay == 0) ? 32 : delay;

    eff35_set_velocity(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

static void spawn_burst(WORK_Other* ewk, s16 group, s16 count) {
    const s16* list = spawn_lists[group][random_32() & 7];

    for (s16 i = 0; i < count; i++) {
        eff35_init(ewk, list[i]);
    }
}

static void spawn_over_time(WORK_Other* ewk, s16 group, s16 max_count) {
    s16* state = &ewk->wu.routine_no[2 + group];
    s16* delay = (group == 0) ? &ewk->wu.vitality : &ewk->wu.vital_new;
    s16 type;

    switch (*state) {
    case 0:
        if (*delay == 0) {
            (*state)++;
        } else {
            (*delay)--;
        }

        break;

    case 1:
        if (*group_count(ewk, group) >= max_count) {
            break;
        }

        type = random_32() & 0xF;

        if (type > 10) {
            type -= 10;
        }

        eff35_init(ewk, type + ((group == 0) ? 0 : FIRST_FAR_PETAL));
        *delay = spawn_delays[random_32()];
        (*state)--;
        break;
    }
}

void effect_ng34_move(WORK_Other* ewk) {
    s16 count;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.routine_no[3] = 0;
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.dead_f == 1) {
        ewk->wu.routine_no[0]++;
        return;
    }

    if (EXE_flag || Game_pause || aku_flag || (akebono_flag && ewk->wu.my_priority < 70)) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        count = random_32() & 7;
        spawn_burst(ewk, 0, (count == 0) ? 4 : count);
        ewk->wu.vitality = spawn_delays[random_32()];

        count = random_32() & 7;
        spawn_burst(ewk, 1, (count == 0) ? 3 : count);
        ewk->wu.vital_new = spawn_delays[random_32()];

        ewk->wu.routine_no[1]++;
        break;

    case 1:
        // More petals fall once the round is decided
        if (Conclusion_Flag == 0) {
            ewk->wu.dir_step = 16;
            ewk->wu.dir_timer = 8;
        } else {
            ewk->wu.dir_step = 30;
            ewk->wu.dir_timer = 15;
        }

        spawn_over_time(ewk, 0, ewk->wu.dir_step);
        spawn_over_time(ewk, 1, ewk->wu.dir_timer);
        break;
    }
}

s32 effect_ng34_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG34_ID;
    ewk->wu.work_id = 16;
    ewk->wu.direction = 0;
    ewk->wu.dir_step = 0;
    ewk->wu.dir_old = 0;
    ewk->wu.dir_timer = 0;
    return 0;
}
