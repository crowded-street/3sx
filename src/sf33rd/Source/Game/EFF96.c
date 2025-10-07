#include "sf33rd/Source/Game/EFF96.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_96_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.char_table[0] = _ef13_char_table;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[6] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
        } else if (EXE_flag == 0 && Game_pause == 0) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0]++;
                break;
            }
        }

        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_96_init(WORK* wk, u8 chix, s8 dspf, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 96;
    ewk->wu.work_id = 16;
    ewk->wu.blink_timing = wk->blink_timing;
    ewk->wu.char_index = chix;
    ewk->wu.disp_flag = dspf;
    ewk->wu.hit_stop = 0;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = wk->cgromtype;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code;
    ewk->wu.my_mts = 14;
    ewk->wu.position_x = wk->position_x;
    ewk->wu.position_y = wk->position_y;
    ewk->wu.position_z = 26;

    if (wk->work_id == 1) {
        ewk->master_player = ((PLW*)wk)->player_number;
        ewk->master_id = wk->id;
    } else {
        ewk->master_player = ((WORK_Other*)wk)->master_player;
        ewk->master_id = ((WORK_Other*)wk)->master_id;
    }

    return 0;
}
