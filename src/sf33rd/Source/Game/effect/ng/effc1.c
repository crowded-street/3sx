/**
 * @file effc1.c
 * Yun or Yang watching the fight from Yun's stage in New Generation (effect C1 in the NG arcade ROM). When one player
 * is Yun or Yang and the other isn't either of them, the other twin stands in the background, turns to follow the
 * player, and cheers or sulks when the round is won. 3S's effect L2 is its counterpart on 3S Yun's stage.
 */

#include "sf33rd/Source/Game/effect/ng/effc1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define YANG_CHAR_INDEX 0
#define YUN_CHAR_INDEX 3

/// Player x from which the twin turns to each next pose, for each player
static const s16 pose_x[2][4] = { { 224, 384, 544, 672 }, { 429, 528, 624, 704 } };

static bool is_twin(s16 character) {
    return character == CHAR_YUN || character == CHAR_YANG;
}

static PLW* watched_player(WORK_Other* ewk) {
    return &plw[ewk->wu.old_rno[2]];
}

static s16 get_pose(WORK_Other* ewk) {
    const s16 x = watched_player(ewk)->wu.position_x;
    s16 pose = 0;

    while (pose < 4 && x >= pose_x[ewk->wu.old_rno[2]][pose]) {
        pose++;
    }

    return ewk->wu.rl_flag ? 4 - pose : pose;
}

static void set_pose(WORK_Other* ewk) {
    set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[3], ewk->wu.old_rno[0] + 1, 0);
    ewk->wu.old_rno[1] = ewk->wu.old_rno[0];
}

void effect_ngc1_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_pose(ewk);
        break;

    case 1:
        if (is_round_won()) {
            ewk->wu.routine_no[0]++;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[3] + (Winner_id == ewk->wu.old_rno[2] ? 1 : 2));
            break;
        }

        ewk->wu.old_rno[0] = get_pose(ewk);

        if (ewk->wu.old_rno[1] != ewk->wu.old_rno[0]) {
            set_pose(ewk);
        }

        break;

    case 2:
        // NG re-creates stage effects every round, while 3S keeps them. Start over once the next round begins.
        if (!is_round_over()) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.old_rno[0] = 0;
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    sort_push_request(&ewk->wu);
}

static s32 effect_ngc1_init_for_player(PLW* pl) {
    const s16 opponent = plw[pl->wu.id ^ 1].player_number;
    WORK_Other* ewk;
    s16 char_index;
    s16 ix;

    if (is_twin(opponent)) {
        return 0;
    }

    if (pl->player_number == CHAR_YANG) {
        char_index = YUN_CHAR_INDEX;
    } else if (pl->player_number == CHAR_YUN) {
        char_index = YANG_CHAR_INDEX;
    } else {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NGC1_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.charset_id = 0;
    ewk->wu.type = char_index;
    ewk->wu.old_rno[2] = pl->wu.id;
    ewk->wu.my_family = 2;
    ewk->wu.old_rno[0] = 0;
    ewk->wu.old_rno[3] = char_index;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 2;
    ewk->wu.kage_hy = 7;
    ewk->wu.kage_char = 13;
    ewk->wu.my_priority = ewk->wu.position_z = 70;
    ewk->wu.kage_prio = ewk->wu.my_priority + 1;
    ewk->wu.char_table[0] = _ng_effc1_char_table;
    ewk->wu.my_col_code = char_index == YANG_CHAR_INDEX ? 0x2149 : 0x2148; // NG 0x205D and 0x205C
    ewk->wu.xyz[1].cal = 0;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = 6;
    ewk->wu.xyz[0].cal = 0;

    if (pl->wu.id == 0) {
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = 368;
        ewk->wu.rl_flag = char_index != YANG_CHAR_INDEX;
    } else {
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = 656;
        ewk->wu.rl_flag = char_index == YANG_CHAR_INDEX;
    }

    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

void effect_ngc1_init_for_players() {
    effect_ngc1_init_for_player(&plw[0]);
    effect_ngc1_init_for_player(&plw[1]);
}
