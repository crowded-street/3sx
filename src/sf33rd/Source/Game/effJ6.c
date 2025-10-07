#include "sf33rd/Source/Game/effJ6.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFF27.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_j6_hit_sub(WORK_Other* ewk);

void effect_J6_move(WORK_Other* ewk) {
    WORK_Other* oya_ptr;

    if (obr_no_disp_check()) {
        return;
    }

    oya_ptr = (WORK_Other*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.routine_no[0] = 4;
            set_char_move_init(&ewk->wu, 0, 3);
        } else {
            set_char_move_init(&ewk->wu, 0, 4);
        }

        break;

    case 1:
        if (oya_ptr->wu.routine_no[0] >= 2) {
            ewk->wu.routine_no[0]++;
        }

        disp_pos_trans_entry_r(ewk);
        break;

    case 2:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            effect_j6_hit_sub(ewk);
        }

        disp_pos_trans_entry_r(ewk);
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        set_char_move_init(&ewk->wu, 0, 3);
        /* fallthrough */

    case 4:
        disp_pos_trans_entry_r(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effect_j6_hit_sub(WORK_Other* ewk) {
    if (eff_hit_check(ewk, 0)) {
        ewk->wu.routine_no[0]++;
        effect_27_init(ewk, 1);
    }
}

s32 effect_J6_init(WORK_Other* oya) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->my_master = (u32*)oya;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 196;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = 3;
    ewk->wu.dead_f = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_col_code = 0x212C;
    *ewk->wu.char_table = _chn_char_table;
    ewk->wu.xyz[0].disp.pos = 904;
    ewk->wu.xyz[1].disp.pos = 16;
    ewk->wu.my_priority = ewk->wu.position_z = 10;
    ewk->wu.char_index = 4;
    ewk->wu.routine_no[1] = 0;
    return 0;
}
