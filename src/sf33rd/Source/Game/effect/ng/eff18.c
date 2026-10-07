/**
 * @file eff18.c
 * Stage objects from New Generation that react to the fight (effect 18 in the NG arcade ROM). Depending on its data,
 * an object either swings for a while when a player does a special move, or plays an animation once a round is won.
 */

#include "sf33rd/Source/Game/effect/ng/eff18.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

/// How long a swing lasts after a special move. Each loop of the animation uses up as many frames as it took.
#define SWING_BUDGET 5

typedef enum Eff18Behavior {
    EFF18_SWING_ON_SPECIAL_MOVE,
    EFF18_PLAY_ON_VICTORY,
} Eff18Behavior;

typedef struct Eff18Data {
    s16 dead_f;
    s16 family;
    s16 col_code;
    s16 x;
    s16 y;
    s16 priority;
    s16 idle_char_index;
    s16 reaction_char_index;
    s16 animate; // Animate every frame
    s16 sync_suzi;
    s16 behavior;
    s16 check_range; // Draw only near the screen
} Eff18Data;

typedef struct Eff18Variant {
    const Eff18Data* objects;
    s16 object_count;
} Eff18Variant;

/// First area of Ryu's stage. Colour codes are converted from palette row 0x63 to the PS2 BG palette row 0x14F.
static const Eff18Data eff18_ryu_a_data[1] = {
    { 1, 2, 8527, -224, 64, 74, 18, 19, 1, 0, EFF18_SWING_ON_SPECIAL_MOVE, 1 },
};

/// Second area of Ryu's stage. Colour codes are converted from palette row 0x5A to the PS2 BG palette row 0x146, with
/// the 0x2000 flag and without it. NG syncs these objects with the line scroll of layer 1 (sync_suzi 1), which we don't
/// have.
// FIXME: Set sync_suzi back to 1 once line scrolling is implemented.
static const Eff18Data eff18_ryu_b_data[3] = {
    { 1, 2, 326, 336, 80, 75, 20, 21, 1, 0, EFF18_SWING_ON_SPECIAL_MOVE, 1 },
    { 1, 2, 326, 48, 48, 73, 28, 29, 1, 0, EFF18_PLAY_ON_VICTORY, 0 },
    { 1, 2, 8518, -112, 57, 76, 25, 26, 1, 0, EFF18_SWING_ON_SPECIAL_MOVE, 1 },
};

/// NG picks each variant's data by `compel_flag` as well, but the data of variants 0 and 1 is the same for all of them.
/// The other variants belong to stages that aren't ported yet.
static const Eff18Variant eff18_variants[] = {
    { eff18_ryu_a_data, SDL_arraysize(eff18_ryu_a_data) },
    { eff18_ryu_b_data, SDL_arraysize(eff18_ryu_b_data) },
};

static bool is_special_move_active() {
    for (int i = 0; i < 2; i++) {
        if (plw[i].wu.routine_no[1] == 4 && plw[i].wu.routine_no[2] > 15) {
            return true;
        }
    }

    return false;
}

static bool is_round_over() {
    return !Allow_a_battle_f && Conclusion_Flag == 1 && C_No[0] > 1;
}

static bool is_round_won() {
    return is_round_over() && Complete_Victory;
}

static void animate(WORK_Other* ewk) {
    if (ewk->wu.hit_stop) {
        char_move(&ewk->wu);
    }
}

static void swing_on_special_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.dir_old);

        /* fallthrough */

    case 1:
        if (is_special_move_active()) {
            ewk->wu.routine_no[2]++;
            ewk->wu.dir_timer = SWING_BUDGET;
            ewk->wu.direction = 0;
        }

        animate(ewk);
        break;

    case 2:
        ewk->wu.direction++;

        if (ewk->wu.cg_type) {
            ewk->wu.cg_type = 0;
            ewk->wu.dir_timer -= ewk->wu.direction;

            if (ewk->wu.dir_timer < 0) {
                ewk->wu.routine_no[2] = 4;
                set_char_move_init(&ewk->wu, 0, ewk->wu.dir_step);
            } else {
                ewk->wu.routine_no[2]++;
            }
        }

        animate(ewk);
        break;

    case 3:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer < 0) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, ewk->wu.dir_step);
        }

        animate(ewk);
        break;

    case 4:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.cg_type = 0;
            ewk->wu.routine_no[2] = 0;
        }

        break;
    }
}

static void play_on_victory(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.dir_old);

        /* fallthrough */

    case 1:
        if (is_round_won()) {
            ewk->wu.routine_no[2]++;
        }

        animate(ewk);
        break;

    case 2:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, ewk->wu.dir_step);
        break;

    case 3:
        // NG re-creates stage effects every round, while 3S keeps them. Start over once the next round begins.
        if (!is_round_over()) {
            ewk->wu.routine_no[2] = 0;
            break;
        }

        char_move(&ewk->wu);
        break;
    }
}

/// Like disp_pos_trans_entry_s and disp_pos_trans_entry_rs, but without the 1 pixel nudge and with NG's range
static void eff18_trans_entry(WORK_Other* ewk) {
    if (obr_no_disp_check() || (ewk->wu.old_rno[0] && !range_x_check_ng(ewk))) {
        return;
    }

    suzi_sync_pos_set(ewk);
    sort_push_request4(&ewk->wu);
}

void effect_ng18_move(WORK_Other* ewk) {
    void (*const behaviors[2])(WORK_Other*) = { swing_on_special_move, play_on_victory };

    if (akebono_flag) {
        return;
    }

    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (!EXE_flag && !Game_pause) {
        behaviors[ewk->wu.routine_no[1]](ewk);
    }

    eff18_trans_entry(ewk);
}

s32 effect_ng18_init(s16 variant) {
    const Eff18Variant* data = &eff18_variants[variant];
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < data->object_count; i++) {
        const Eff18Data* object = &data->objects[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG18_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.dead_f = object->dead_f;
        ewk->wu.my_family = object->family;
        ewk->wu.my_col_code = object->col_code;
        ewk->wu.xyz[0].disp.pos = object->x;
        ewk->wu.xyz[1].disp.pos = object->y;
        ewk->wu.my_priority = ewk->wu.position_z = object->priority;
        ewk->wu.dir_old = object->idle_char_index;
        ewk->wu.dir_step = object->reaction_char_index;
        ewk->wu.hit_stop = object->animate;
        ewk->wu.sync_suzi = object->sync_suzi;
        ewk->wu.routine_no[1] = object->behavior;
        ewk->wu.old_rno[0] = object->check_range;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
