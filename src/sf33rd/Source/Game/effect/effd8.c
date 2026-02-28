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

// Data

const u8 D8_Priority_Data[4] = { 55, 55, 52, 53 };

// Funcs

void effect_D8_move(WORK_Other* ewk) {
    s16 offset_x;

    ewk->wu.hit_quake += 1;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (((Mode_Type == MODE_NORMAL_TRAINING) || (Mode_Type == MODE_PARRY_TRAINING)) &&
            (ewk->master_id == New_Challenger) && (S_No[3] < 2)) {
            return;
        }

        if (Complete_Face <= 0) {
            ewk->wu.routine_no[0] += 1;
            ewk->wu.dir_timer = 10;
        }

        break;

    case 1:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0] += 1;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        break;

    case 2:
        if ((ewk->wu.vital_new != Cursor_X[ewk->master_id]) || (ewk->wu.vital_old != Cursor_Y[ewk->master_id])) {
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

        break;

    case 3:
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

        break;

    case 4:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 0;
        break;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

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
