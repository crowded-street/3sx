#include "sf33rd/Source/Game/EFF15.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void eff15_kemuri(WORK_Other* ewk);
void eff15_koishi(WORK_Other* ewk);

void effect_15_move(WORK_Other* ewk) {
    if (ewk->wu.type) {
        eff15_koishi(ewk);
    } else {
        eff15_kemuri(ewk);
    }
}

void eff15_kemuri(WORK_Other* ewk) {
    WORK* oya_ptr;

    (void)ewk;
    (void)oya_ptr;
}

void eff15_koishi(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    WORK* oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);

        if (ewk->wu.type == 1) {
            ewk->wu.mvxy.a[0].sp = 0x8000;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = -0x28000;
            ewk->wu.mvxy.d[1].sp = -0x6000;
            ewk->wu.xyz[0].disp.pos = 640;
            ewk->wu.old_rno[2] = 64;
            break;
        }

        ewk->wu.mvxy.a[0].sp = -0x8000;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.a[1].sp = 0;
        ewk->wu.mvxy.d[1].sp = -0x4000;
        ewk->wu.xyz[0].disp.pos = 576;
        ewk->wu.old_rno[2] = 72;
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);

            if (ewk->wu.xyz[1].disp.pos < ewk->wu.old_rno[2]) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, 41);
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_15_init(WORK* wk, u8 data) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0xF;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.type = data;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2020;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = (u32*)wk;
    ewk->wu.rl_flag = 0;
    ewk->wu.xyz[0].disp.pos = 629;
    ewk->wu.xyz[1].disp.pos = 340;
    ewk->wu.my_priority = 76;
    ewk->wu.position_z = 76;
    ewk->wu.char_table[0] = _etc2_char_table;

    if (data) {
        ewk->wu.char_index = 40;
    } else {
        ewk->wu.char_index = 39;
    }

    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
