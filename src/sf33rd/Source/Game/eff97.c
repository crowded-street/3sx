#include "sf33rd/Source/Game/eff97.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_97_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 44);
        /* fallthrough */

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);

            if (plw[ewk->master_id].wu.routine_no[2] == 1 && plw[ewk->master_id].wu.routine_no[3] == 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.old_rno[0] = 16;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] > 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.rl_flag ^= 1;
                set_char_move_init(&ewk->wu, 0, 45);
                ewk->wu.old_rno[0] = 16;
                ewk->wu.mvxy.a[1].sp = 0xE8000;
                ewk->wu.mvxy.d[1].sp = -0x6000;

                if (plw[ewk->master_id].wu.id) {
                    ewk->wu.mvxy.a[0].sp = -0xA8000;
                    ewk->wu.mvxy.d[0].sp = -0x1000;
                } else {
                    ewk->wu.mvxy.a[0].sp = 0xA8000;
                    ewk->wu.mvxy.d[0].sp = 0x1000;
                }
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] > 0) {
                add_x_sub(ewk);
                add_y_sub(ewk);
            } else {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_97_init(PLW* oya) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 97;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->my_master = (u32*)oya;
    ewk->master_id = oya->wu.id;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = oya->wu.my_col_code;
    ewk->wu.my_family = 2;
    ewk->wu.my_priority = ewk->wu.position_z = oya->wu.my_priority;
    ewk->wu.sync_suzi = 0;
    ewk->wu.xyz[1].disp.pos = 40;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 0;
    ewk->wu.kage_hy = 40;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 20;
    *ewk->wu.char_table = _etc_char_table;

    if (oya->wu.id) {
        ewk->wu.rl_flag = 0;
        ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos - 108;
    } else {
        ewk->wu.rl_flag = 1;
        ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos + 108;
    }

    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
