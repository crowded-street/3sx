#include "sf33rd/Source/Game/EffA9.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Eff59.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void Setup_A9(WORK_Other* ewk, s16 Char_Index, s16 Option, s16 Option2);

void effect_A9_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        if (!Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, ewk->wu.vital_new)) {
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[0]++;
        }

        break;

    case 2:
        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, ewk->wu.vital_new) || Suicide[3] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 99;
            break;
        }

        if (ewk->wu.char_index == 55) {
            if (E_07_Flag[LOSER]) {
                ewk->wu.routine_no[0]++;
                ewk->wu.dir_timer = 20;
                SsRequest(0x62);
            }

            break;
        }

        if (ewk->wu.char_index == 57) {
            char_move(&ewk->wu);
        }

        break;

    case 3:
        char_move(&ewk->wu);

        if (--ewk->wu.dir_timer == 0) {
            if (ewk->wu.cg_type == 1) {
                ewk->wu.dir_timer = 1;
            } else {
                ewk->wu.routine_no[0] = 4;
            }
        }

        break;

    case 4:
        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, ewk->wu.vital_new) || Suicide[3] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 99;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
    sort_push_request4(&ewk->wu);
}

s32 effect_A9_init(s16 Char_Index, s16 Option, s16 Pos_Index, s16 Option2) {
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
    ewk->wu.id = 109;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 144;
    ewk->wu.my_family = 4;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.char_index = Char_Index;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[0].disp.pos = Offset_BG_X[3] + bg_w.bgw[3].wxy[0].disp.pos + Position_Data_A9[Pos_Index][0];
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[3].wxy[1].disp.pos + Position_Data_A9[Pos_Index][1];
    ewk->wu.xyz[2].disp.pos = Position_Data_A9[Pos_Index][2];
    ewk->wu.vital_new = Position_Data_A9[Pos_Index][3];
    Setup_A9(ewk, Char_Index, Option, Option2);
    return 0;
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
    case 12:
    case 16:
    case 79:
    case 6:
    case 80:
    case 58:
    case 60:
    case 59:
        ewk->wu.dir_step = Option;
        break;
    }
}
