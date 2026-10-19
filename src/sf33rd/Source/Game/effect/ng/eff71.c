/**
 * @file eff71.c
 * Dog in Oro's cave from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff71.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2144 // NG 0x2058
#define PRIORITY 74

#define CHAR_LOOK 28 // Head turns. Its ip is the direction.
#define CHAR_SPECIAL_REACTION 29
#define CHAR_CLOSE_REACTION 30
#define CHAR_TYPE1_IDLE 40
#define CHAR_TYPE1_REACTION 41

#define LOOK_LEFT 1
#define LOOK_FRONT 3
#define LOOK_RIGHT 5

#define CLOSE_RANGE 32

static s16 get_interest(s16 player_number) {
    switch (player_number) {
    case CHAR_IBUKI:
    case CHAR_ELENA:
        return 1;

    case CHAR_ORO:
        return 2;

    default:
        return 0;
    }
}

static s16 eff71_look_direction(WORK_Other* ewk) {
    s16 x;

    if (ewk->wu.old_rno[0] == ewk->wu.old_rno[1]) {
        x = bg_w.bgw[1].xy[0].disp.pos;

        if (x < 0x220) {
            return LOOK_LEFT;
        }

        return x < 0x270 ? LOOK_FRONT : LOOK_RIGHT;
    }

    x = ewk->wu.old_rno[0] <= ewk->wu.old_rno[1] ? plw[1].wu.position_x : plw[0].wu.position_x;

    if (x < 0x1C0) {
        return LOOK_LEFT;
    }

    return x < 0x270 ? LOOK_FRONT : LOOK_RIGHT;
}

static bool is_close(WORK_Other* ewk, s16 player_x) {
    if (ewk->wu.dir_old == LOOK_FRONT && is_near(ewk->wu.position_x, player_x, CLOSE_RANGE)) {
        ewk->wu.routine_no[0] = 2;
        ewk->wu.routine_no[1] = 0;
        return true;
    }

    return false;
}

static void eff71_check_reaction(WORK_Other* ewk) {
    if (is_special_move_active()) {
        ewk->wu.routine_no[0] = 1;
        ewk->wu.routine_no[1] = 0;
        return;
    }

    if (ewk->wu.routine_no[0] == 0) {
        if (ewk->wu.old_rno[0] != 0 && is_close(ewk, plw[0].wu.position_x)) {
            return;
        }

        if (ewk->wu.old_rno[1] != 0) {
            is_close(ewk, plw[1].wu.position_x);
        }
    }
}

static void eff71_look(WORK_Other* ewk) {
    s16 direction;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        direction = eff71_look_direction(ewk);
        ewk->wu.dir_old = direction;
        set_char_move_init2(&ewk->wu, 0, CHAR_LOOK, direction, 0);
        break;

    case 1:
        direction = eff71_look_direction(ewk);

        if (ewk->wu.dir_old == direction) {
            eff71_check_reaction(ewk);
            break;
        }

        set_char_move_init2(&ewk->wu, 0, CHAR_LOOK, direction, 0);
        ewk->wu.dir_old = direction;
        ewk->wu.routine_no[1]++;
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[1] = 1;
        }

        break;
    }
}

static void eff71_react_to_special(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_SPECIAL_REACTION);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

static void eff71_react_to_closeness(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_CLOSE_REACTION);
        break;

    case 1:
        if (is_special_move_active()) {
            ewk->wu.routine_no[0] = 1;
            ewk->wu.routine_no[1] = 0;
            break;
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

static void eff71_type0(WORK_Other* ewk) {
    void (*const states[3])(WORK_Other*) = { eff71_look, eff71_react_to_special, eff71_react_to_closeness };
    states[ewk->wu.routine_no[0]](ewk);
}

static void eff71_type1(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_TYPE1_IDLE);
        break;

    case 1:
        if (is_special_move_active()) {
            ewk->wu.routine_no[0] = 2;
            set_char_move_init(&ewk->wu, 0, CHAR_TYPE1_REACTION);
            break;
        }

        char_move(&ewk->wu);
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0] = 0;
        }

        break;
    }
}

void effect_ng71_move(WORK_Other* ewk) {
    void (*const types[2])(WORK_Other*) = { eff71_type0, eff71_type1 };

    if (!EXE_flag && !Game_pause) {
        types[ewk->wu.type](ewk);
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

s32 effect_ng71_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 p1 = plw[0].player_number;
    s16 p2 = plw[1].player_number;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG71_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.my_priority = ewk->wu.position_z = PRIORITY;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_brz_char_table;
    ewk->wu.dir_old = LOOK_LEFT;
    ewk->wu.old_rno[0] = get_interest(p1);
    ewk->wu.old_rno[1] = get_interest(p2);

    if (get_interest(p1) != 0 || get_interest(p2) != 0) {
        ewk->wu.type = 0;
        ewk->wu.xyz[0].disp.pos = 608;
        ewk->wu.xyz[1].disp.pos = 53;
    } else {
        ewk->wu.type = 1;
        ewk->wu.xyz[0].disp.pos = 700;
        ewk->wu.xyz[1].disp.pos = 56;
    }

    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
