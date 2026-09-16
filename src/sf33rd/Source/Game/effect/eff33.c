/**
 * @file eff33.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff33.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

const s16 WinLoseID[2][2];

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}


static s32 judgement_is_blocked(void) {
    return EXE_flag || Game_pause || pcon_rno[2] != 1 || Event_Judge_Gals != -1 || !Complete_Judgement;
}


static void eff33_spawn(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 0;
    ewk->wu.kage_hy = -10;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 16;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void eff33_wait_judgement(WORK_Other* ewk) {
    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);

    if (judgement_is_blocked()) {
        return;
    }

    ewk->wu.routine_no[0]++;
}

static void eff33_show_result(WORK_Other* ewk) {
if (game_is_active()) {
        ewk->wu.routine_no[0]++;
        ewk->wu.char_index = WinLoseID[ewk->master_id][Winner_id] + 10;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    }

    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

static void eff33_animate(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        char_move(&ewk->wu);
    }

    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

void effect_33_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        eff33_spawn(ewk);
        break;

    case 1:
        eff33_wait_judgement(ewk);
        break;

    case 2:
        eff33_show_result(ewk);
        break;

    case 3:
        eff33_animate(ewk);
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
    ewk->my_master = wk;
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
