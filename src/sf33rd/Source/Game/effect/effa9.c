/**
 * @file effa9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa9.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff59.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void Setup_A9(WORK_Other* ewk, s16 Char_Index, s16 Option, s16 Option2);

/* Hide the effect once it has scrolled out of range, or when the global suicide
 * flag fires. Non-zero when it did - states 2 and 4 both end this way, and
 * state 2 stops the rest of its frame on it. */
static s32 a9_hide_if_gone(WORK_Other* ewk) {
    if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, ewk->wu.vital_new) || Suicide[3] != 0) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 99;
        return 1;
    }

    return 0;
}

static void effa9_wait_on_screen(WORK_Other* ewk) {
    if (!Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, ewk->wu.vital_new)) {
        ewk->wu.disp_flag = 1;
        ewk->wu.routine_no[0]++;
    }
}

static void effa9_watch_loser(WORK_Other* ewk) {
    if (a9_hide_if_gone(ewk)) {
        return;
    }

    if (ewk->wu.char_index == 55) {
        if (E_07_Flag[LOSER]) {
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = 20;
            SsRequest(0x62);
        }

        return;
    }

    if (ewk->wu.char_index == 57) {
        char_move(&ewk->wu);
    }
}

static void effa9_play_out(WORK_Other* ewk) {
    char_move(&ewk->wu);

    if (--ewk->wu.dir_timer == 0) {
        if (ewk->wu.cg_type == 1) {
            ewk->wu.dir_timer = 1;
        } else {
            ewk->wu.routine_no[0] = 4;
        }
    }
}

/* Where a slot in the position table puts the effect, and the position the
 * sorter is given once it has moved. */
static void a9_place_at_slot(WORK_Other* ewk, s16 Pos_Index) {
    ewk->wu.xyz[0].disp.pos = Offset_BG_X[3] + bg_w.bgw[3].wxy[0].disp.pos + Position_Data_A9[Pos_Index][0];
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[3].wxy[1].disp.pos + Position_Data_A9[Pos_Index][1];
    ewk->wu.xyz[2].disp.pos = Position_Data_A9[Pos_Index][2];
    ewk->wu.vital_new = Position_Data_A9[Pos_Index][3];
}

static void a9_publish_position(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
    sort_push_request4(&ewk->wu);
}

void effect_A9_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        effa9_wait_on_screen(ewk);
        break;

    case 2:
        effa9_watch_loser(ewk);
        break;

    case 3:
        effa9_play_out(ewk);
        break;

    case 4:
        a9_hide_if_gone(ewk);

        break;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    a9_publish_position(ewk);
}

s32 effect_A9_init(s16 Char_Index, s16 Option, s16 Pos_Index, s16 Option2) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 109;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 144;
    ewk->wu.my_family = 4;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.char_index = Char_Index;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    a9_place_at_slot(ewk, Pos_Index);
    Setup_A9(ewk, Char_Index, Option, Option2);
    return 0;
}

static void setup_a9_late(WORK_Other* ewk, s16 Char_Index, s16 Option) {
    switch (Char_Index) {
    case 80:
    case 58:
    case 60:
    case 59:
        ewk->wu.dir_step = Option;
        break;
    }
}

static void setup_a9_mid(WORK_Other* ewk, s16 Char_Index, s16 Option) {
    switch (Char_Index) {
    case 12:
    case 16:
    case 79:
    case 6:
        ewk->wu.dir_step = Option;
        break;

    default:
        setup_a9_late(ewk, Char_Index, Option);
        break;
    }
}

void Setup_A9(WORK_Other* ewk, s16 Char_Index, s16 Option, s16 Option2) {
    switch (Char_Index) {
    case 32:
        if (Option2) {
            effect_59_init(ewk, ewk->wu.my_family, 4, 1);
        }

        /* fallthrough */

    case 33:
    case 34:
    case 81:
        ewk->wu.dir_step = Option;
        break;

    default:
        setup_a9_mid(ewk, Char_Index, Option);
        break;
    }
}
