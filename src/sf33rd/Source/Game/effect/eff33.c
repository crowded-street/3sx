/**
 * @file eff33.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff33.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 WinLoseID[2][2];

void effect_33_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 0;
        ewk->wu.kage_hy = -10;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 16;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);

        if (EXE_flag || Game_pause || pcon_rno[2] != 1 || Event_Judge_Gals != -1 || !Complete_Judgement) {
            break;
        }

        ewk->wu.routine_no[0]++;
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.routine_no[0]++;
            ewk->wu.char_index = WinLoseID[ewk->master_id][Winner_id] + 10;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0]++;
                break;
            }

            char_move(&ewk->wu);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    case 5:
    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_33_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 33;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 1;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = (u32*)wk;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos;
    ewk->wu.xyz[0].disp.pos += wk->rl_flag ? -48 : 48;
    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 12;
    ewk->wu.my_priority = wk->my_priority - 12;
    ewk->wu.position_z = ewk->wu.my_priority - 12;
    ewk->wu.char_table[0] = _etc3_char_table;
    ewk->wu.char_index = 7;
    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 0xE;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

const s16 WinLoseID[2][2] = { { 1, 0 }, { 0, 1 } };
