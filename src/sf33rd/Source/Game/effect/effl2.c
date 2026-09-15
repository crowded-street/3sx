/**
 * @file effl2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl2.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"

const s8 effl2_dir_tbl[2][16] = { { 0, 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4 },
                                  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3 } };

/* Non-zero once the round is decided: no battle running, the conclusion flag
 * up, and the scene counter past the fight itself. */
static s32 battle_is_over(void) {
    return Allow_a_battle_f == 0 && Conclusion_Flag == 1 && *C_No >= 2;
}

/* Non-zero for a perfect win.
 *
 * NOTE: the Conclusion_Flag test is redundant - the caller only reaches this
 * once battle_is_over() has already required it. Preserved as found; see
 * AGENTS.md on arcade-accurate oddities. */
static s32 is_complete_victory(void) {
    return !(Complete_Victory == 0) && Conclusion_Flag;
}

/* Start the effect: show it and face it the right way. */
static void l2_start(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    effl2_dir_check(ewk);
    set_char_move_init2(&ewk->wu, 0, 0, 1, 0);
}

/* Switch to the win animation - the winner's own, or the loser's. */
static void l2_show_result(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.old_rno[0] = 0;

    if (Winner_id != ewk->master_id) {
        set_char_move_init(&ewk->wu, 0, 2);
    } else {
        set_char_move_init(&ewk->wu, 0, 1);
    }
}

/* While the round runs, keep the effect facing the right way; once it is
 * decided, a perfect win switches to the result animation. */
static void l2_track_round(WORK_Other* ewk) {
    if (battle_is_over()) {
        if (is_complete_victory()) {
            l2_show_result(ewk);
        }
    } else if (!EXE_flag && !Game_pause) {
        effl2_dir_check(ewk);
    }
}

/* Hold the result until the wipe has come and gone, then restart the effect. */
static void l2_wait_for_wipe(WORK_Other* ewk) {
    if (Exec_Wipe) {
        ewk->wu.old_rno[0] = 1;
    }

    if (ewk->wu.old_rno[0] && !Exec_Wipe) {
        ewk->wu.routine_no[0] = 0;
    }
}

/* Place the effect at its work position and hand it to the renderer. */
static void l2_push_at_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
    sort_push_request(&ewk->wu);
}

void effect_L2_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        l2_start(ewk);
        break;

    case 1:
        l2_track_round(ewk);
        l2_push_at_position(ewk);
        break;

    case 2:
        l2_wait_for_wipe(ewk);
        l2_push_at_position(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effl2_dir_check(WORK_Other* ewk) {
    s16 work = (plw[ewk->master_id].wu.xyz[0].disp.pos);

    work >>= 6;
    work &= 15;

    if (ewk->wu.direction != effl2_dir_tbl[ewk->master_id][work]) {
        ewk->wu.direction = effl2_dir_tbl[ewk->master_id][work];
        set_char_move_init2(&ewk->wu, 0, 0, ewk->wu.direction + 1, 0);
    }
}

s32 effect_L2_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 oya_id;

    if (My_char[0] == 10 || My_char[1] == 10) {
        return -1;
    }

    if (My_char[0] == 3 && My_char[1] == 3) {
        return -1;
    }

    if (My_char[0] == 3) {
        oya_id = 0;
    } else if (My_char[1] == 3) {
        oya_id = 1;
    } else {
        return -1;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 212;
    ewk->wu.work_id = 16;
    ewk->master_id = oya_id;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.my_family = 2;
    ewk->my_master = &plw[oya_id];
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (oya_id) {
        ewk->wu.my_col_code = 0x2016;
    } else {
        ewk->wu.my_col_code = 0x2006;
    }

    ewk->wu.my_priority = ewk->wu.position_z = 71;

    if (oya_id) {
        ewk->wu.xyz[0].cal = 0x3000000;
    } else {
        ewk->wu.xyz[0].cal = 0xF00000;
    }

    ewk->wu.xyz[1].cal = 0xA0000;
    ewk->wu.char_table[0] = _direct_03_char_table;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 0;
    ewk->wu.kage_hy = 11;
    ewk->wu.kage_char = 10;
    ewk->wu.kage_prio = ewk->wu.position_z + 1;
    ewk->wu.dir_old = 0;
    ewk->wu.direction = 0;
    return 0;
}
