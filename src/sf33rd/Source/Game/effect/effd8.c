/**
 * @file effd8.c
 * Selection circle in character select
 */

#include "sf33rd/Source/Game/effect/effd8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/screen/sel_pl.h"
#include "structs.h"

// Forward decls

void Setup_EffD8_Pos(WORK_Other* ewk, s16 /* unused */);
s32 Setup_Face_Offset_X(s16 x);

const u8 D8_Priority_Data[4] = { 55, 55, 52, 53 };

// Funcs

static s32 cursor_position_changed(const WORK_Other* ewk) {
    return (ewk->wu.vital_new != Cursor_X[ewk->master_id]) ||
           (ewk->wu.vital_old != Cursor_Y[ewk->master_id]);
}


/* In either training mode the challenger's face waits until the select screen
 * has moved on - the effect does nothing at all until then. */
static s32 d8_waiting_for_training_partner(const WORK_Other* ewk) {
    return ((Mode_Type == MODE_NORMAL_TRAINING) || (Mode_Type == MODE_PARRY_TRAINING)) &&
           (ewk->master_id == New_Challenger) && (S_No[3] < 2);
}

/* Hold until the portrait artwork is ready, then start the appear delay. */
static void d8_await_face_ready(WORK_Other* ewk) {
    if (Complete_Face <= 0) {
        ewk->wu.routine_no[0] += 1;
        ewk->wu.dir_timer = 10;
    }
}

/* Run the appear delay out, then show the face and start its animation. */
static void d8_await_appear_delay(WORK_Other* ewk) {
    if (--ewk->wu.dir_timer == 0) {
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    }
}

/* Copy the work position out and hand the face to the renderer. Faces on the
 * near layer flicker between two depths each frame, which is what hit_quake
 * counts here. */
static void d8_place_and_push(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;

    if (ewk->wu.direction == 0) {
        if (ewk->wu.hit_quake & 1) {
            ewk->wu.position_z = 56;
        } else {
            ewk->wu.position_z = 54;
        }
    }

    sort_push_request4(&ewk->wu);
}

/* Track the select cursor: when it moves, re-place the face and restart its
 * animation at the matching frame. Once this player has locked in, start the
 * confirm animation instead. */
static void d8_follow_cursor(WORK_Other* ewk) {
    s16 offset_x;

    if (cursor_position_changed(ewk)) {
        ewk->wu.vital_new = Cursor_X[ewk->master_id];
        ewk->wu.vital_old = Cursor_Y[ewk->master_id];

        if (Play_Type == 1) {
            offset_x = Setup_Face_Offset_X(99);
        } else {
            offset_x = Setup_Face_Offset_X(Play_Type_1st);
        }

        Setup_EffD8_Pos(ewk, offset_x);
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, (ewk->wu.cg_ix / ewk->wu.cgd_type) + 1, 0);
    }

    if (Sel_PL_Complete[ewk->master_id]) {
        ewk->wu.routine_no[0] += 1;
        ewk->wu.dir_timer = 20;
        ewk->wu.char_index += 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    } else {
        char_move(&ewk->wu);
    }
}

/* Run the confirm animation out, then lock the selection in and hand the select
 * timer over. */
static void d8_confirm_selection(WORK_Other* ewk) {
    if (--ewk->wu.dir_timer) {
        char_move(&ewk->wu);
    } else {
        ewk->wu.routine_no[0] += 1;
        Sel_PL_Complete[ewk->master_id] = -0x8000;

        if (Select_Start[ewk->master_id] == 0) {
            Select_Timer = 0x20;
        }

        Unit_Of_Timer = UNIT_OF_TIMER_MAX;
        ewk->wu.char_index += 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    }
}

void effect_D8_move(WORK_Other* ewk) {
    ewk->wu.hit_quake += 1;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (d8_waiting_for_training_partner(ewk)) {
            return;
        }

        d8_await_face_ready(ewk);
        break;

    case 1:
        d8_await_appear_delay(ewk);
        break;

    case 2:
        d8_follow_cursor(ewk);
        break;

    case 3:
        d8_confirm_selection(ewk);
        break;

    case 4:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 0;
        break;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    d8_place_and_push(ewk);
}

s32 effect_D8_init(s16 PL_id, s16 Type) {
    WORK_Other* ewk;
    s16 ix;
    s16 offset_x;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0x8A;
    ewk->wu.work_id = 0x10;
    ewk->wu.my_col_code = 0x90;
    ewk->wu.my_family = 2;
    ewk->wu.char_table[0] = _sel_pl_char_table;
    ewk->wu.char_index = (Type * 3) + 43;
    ewk->master_id = PL_id;
    ewk->wu.vital_new = Cursor_X[ewk->master_id];
    ewk->wu.vital_old = Cursor_Y[ewk->master_id];
    ewk->wu.position_z = D8_Priority_Data[Type];
    ewk->wu.direction = Type;
    ewk->wu.hit_quake = 0;
    ewk->wu.my_mts = 0xD;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    offset_x = Setup_Face_Offset_X(Play_Type_1st);
    Setup_EffD8_Pos(ewk, offset_x);
    return 0;
}

void Setup_EffD8_Pos(WORK_Other* ewk, s16 /* unused */) {
    s16 xx = ID_of_Face[Cursor_Y[ewk->master_id]][Cursor_X[ewk->master_id]];
    ewk->wu.xyz[0].disp.pos = Face_Pos_Data[xx][0] + 512;
    ewk->wu.xyz[1].disp.pos = Face_Pos_Data[xx][1] + 0;
}

s32 Setup_Face_Offset_X(s16 x) {
    switch (x) {
    case 0:
        return 0;

    case 1:
        return -184;

    default:
        return -92;
    }
}
