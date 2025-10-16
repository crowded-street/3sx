/**
 * @file effe0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe0.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void Setup_Char_E0(WORK_Other* ewk);

void effect_E0_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 64)) {
            return;
        }

        ewk->wu.disp_flag = 1;

        if (ewk->wu.dm_vital == 1) {
            ewk->wu.routine_no[0] = 4;
        } else {
            ewk->wu.routine_no[0]++;
        }

        break;

    case 1:
        if (Sel_EM_Complete[Player_id]) {
            if (VS_Index[Player_id] >= 8) {
                ewk->wu.routine_no[0] = 4;
            } else {
                ewk->wu.routine_no[0] = 2;
            }

            ewk->wu.dir_timer = 1;
        } else if (Moving_Plate[Player_id] != 0 && ewk->wu.dm_vital == 0) {
            if (--Moving_Plate_Counter[Player_id] == 0) {
                Moving_Plate[Player_id] = 0;
            }

            Setup_Char_E0(ewk);
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        break;

    case 2:
        if (--ewk->wu.dir_timer != 0) {
            break;
        }

        ewk->wu.routine_no[0]++;
        ewk->wu.dir_timer = 20;

        if (Temporary_EM[Player_id] == ewk->wu.direction) {
            ewk->wu.char_index = ((ewk->wu.direction - 1) * 4) + 38;
        } else {
            ewk->wu.char_index = ((ewk->wu.direction - 1) * 4) + 37;
        }

        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 3:
        if (Exec_Wipe == 0) {
            char_move(&ewk->wu);
        }

        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
            Sel_EM_Complete[Player_id] |= ~0x7F;
            ewk->wu.char_index = ((ewk->wu.direction - 1) * 4) + 35;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        break;

    case 4:
        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 64)) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.dm_vital == 0 && Exec_Wipe == 0) {
        char_move(&ewk->wu);
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

void Setup_Char_E0(WORK_Other* ewk) {
    ewk->wu.char_index = ((ewk->wu.direction - 1) * 4) + 35;
    ewk->wu.dir_step = 0;

    if (ewk->wu.direction == Temporary_EM[Player_id]) {
        ewk->wu.char_index++;
    }
}

s32 effect_E0_init(s16 Direction, s16 dm_vital, s16 Pos_Type) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 140;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = 4;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.dm_vital = dm_vital;
    ewk->wu.direction = Direction;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[0].disp.pos = Offset_BG_X[3] + bg_w.bgw[3].wxy[0].disp.pos + Plate_Pos_Data_E0[Pos_Type][0];
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[3].wxy[1].disp.pos + Plate_Pos_Data_E0[Pos_Type][1];
    ewk->wu.position_z = 17;

    if (Direction == 2) {
        ewk->wu.xyz[1].disp.pos -= 104;
        ewk->wu.position_z++;
    }

    if (dm_vital == 1) {
        ewk->wu.xyz[0].disp.pos += 3;
        ewk->wu.xyz[1].disp.pos--;
        ewk->wu.char_index = 17;
        ewk->wu.dir_step = Direction - 1;
    } else {
        Setup_Char_E0(ewk);
    }

    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
    return 0;
}
