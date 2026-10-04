/**
 * @file eff69.c
 * The car on the left of Alex's stage from New Generation (effects 69 and 70 in the NG arcade ROM)
 *
 * Effect 69 is the car. Attacks that hit it damage it, at most once every DAMAGE_WAIT frames. Effect 70 is one of three
 * pieces that show the damage of each state. The third hit breaks the car and starts a blinking effect 60.
 */

#include "sf33rd/Source/Game/effect/ng/eff69.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff60.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

/// Converted from NG's stage palette row 0x40 like in eff64.c
#define COLOR_CODE 8492

#define CAR_TYPE 6
#define CAR_CHAR_INDEX 0x10
#define BROKEN_CHAR_INDEX 0x13
#define BROKEN_STATE 3
#define DAMAGE_WAIT 0x60
#define PIECE_COUNT 3

/// Where attacks hit the car
static const s16 car_hit_box[4] = { -53, 39, 20, 25 };

typedef struct PieceData {
    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;
} PieceData;

static const PieceData piece_data_tbl[PIECE_COUNT] = {
    { 192, 48, 79, 20 },
    { 208, 48, 78, 24 },
    { 182, 50, 77, 28 },
};

/// Animation of each piece in each of the car's states
static const s16 piece_char_tbl[PIECE_COUNT][BROKEN_STATE + 1] = {
    { 20, 21, 22, 23 },
    { 24, 25, 26, 27 },
    { 28, 29, 30, 31 },
};

static bool is_paused() {
    return EXE_flag || Game_pause || EXE_obroll;
}

static void init_common(WORK_Other* ewk, s16 id, s8 type) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = id;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.type = type;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.dead_f = 1;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = char_add[bg_w.bg_index];
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
}

// MARK: - Car

/// Takes damage from attacks. old_rno[0] holds the wait until the car can take damage again.
static void eff69_wait(WORK_Other* ewk) {
    if (ewk->wu.old_rno[0] != 0) {
        ewk->wu.old_rno[0]--;
        return;
    }

    if (ewk->wu.routine_no[1] < eff_hit_check_box(ewk, 0, car_hit_box)) {
        ewk->wu.routine_no[1]++;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.old_rno[0] = DAMAGE_WAIT;
    }
}

static void eff69_broken(WORK_Other* ewk) {
    if (ewk->wu.routine_no[2] != 0) {
        return;
    }

    ewk->wu.routine_no[2]++;
    set_char_move_init(&ewk->wu, 0, BROKEN_CHAR_INDEX);
    effect_ng60_init(1);
}

void effect_ng69_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CAR_CHAR_INDEX);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (!is_paused()) {
            if (ewk->wu.routine_no[1] < BROKEN_STATE) {
                eff69_wait(ewk);
            } else {
                eff69_broken(ewk);
            }
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

// MARK: - Pieces

/// Shows the piece's animation for the car's state. The first piece is hidden until the car is damaged.
static void eff70_update(WORK_Other* ewk) {
    const s16 state = ewk->wu.routine_no[1];

    if (state == 0 || ewk->wu.routine_no[2] != 0) {
        return;
    }

    ewk->wu.routine_no[2]++;

    if (state == 1 && ewk->wu.type == 0) {
        ewk->wu.disp_flag = 1;
    } else {
        set_char_move_init(&ewk->wu, 0, piece_char_tbl[ewk->wu.type][state]);
    }
}

void effect_ng70_move(WORK_Other* ewk) {
    const WORK* car = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = ewk->wu.type != 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.routine_no[1] = car->routine_no[1];
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (!is_paused()) {
            if (car->routine_no[1] != ewk->wu.routine_no[1]) {
                ewk->wu.routine_no[2] = 0;
                ewk->wu.routine_no[1] = car->routine_no[1];
            }

            eff70_update(ewk);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

static s32 effect_ng70_init(WORK_Other* car) {
    for (s16 i = 0; i < PIECE_COUNT; i++) {
        const PieceData* data = &piece_data_tbl[i];
        WORK_Other* ewk;
        s16 ix;

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        init_common(ewk, EFFECT_NG70_ID, i);
        ewk->my_master = car;
        ewk->wu.xyz[0].disp.pos = data->x;
        ewk->wu.xyz[1].disp.pos = data->y;
        ewk->wu.my_priority = ewk->wu.position_z = data->priority;
        ewk->wu.char_index = data->char_index;
        suzi_offset_set(ewk);
    }

    return 0;
}

s32 effect_ng69_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    init_common(ewk, EFFECT_NG69_ID, CAR_TYPE);
    ewk->wu.old_rno[0] = 0;
    ewk->wu.xyz[0].disp.pos = 0xC0;
    ewk->wu.xyz[1].disp.pos = 0x30;
    ewk->wu.my_priority = ewk->wu.position_z = 0x50;
    suzi_offset_set(ewk);
    return effect_ng70_init(ewk);
}
