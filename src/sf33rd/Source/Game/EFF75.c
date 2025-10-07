#include "sf33rd/Source/Game/EFF75.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFF57.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const EFF75_Jmp_Tbl[5])();

void effect_75_move(WORK_Other* ewk) {
    EFF75_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void EFF75_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF75_SLIDE_IN(WORK_Other* /* unused */) {}

void EFF75_CHAR_CHANGE(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    if (--Order_Timer[ewk->wu.dir_old] != 0) {
        return;
    }

    ewk->wu.routine_no[0] = 0;
    Order[ewk->wu.dir_old] = 0;
    ewk->wu.dir_step = Order_Dir[ewk->wu.dir_old];
    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
}

void EFF75_SUDDENLY(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 1;
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 1;
        ewk->wu.position_z = 76;
        ewk->wu.dir_step = Order_Dir[ewk->wu.dir_old];
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

s32 effect_75_init(s16 dir_old, s16 ID, s16 Target_BG) {
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
    ewk->wu.id = 75;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x90;
    ewk->wu.my_family = Target_BG + 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.char_index = 19;
    ewk->wu.dir_step = ID;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

void (*const EFF75_Jmp_Tbl[5])() = { EFF75_WAIT, EFF75_SLIDE_IN, EFF75_CHAR_CHANGE, EFF75_SUDDENLY, EFF57_KILL };
