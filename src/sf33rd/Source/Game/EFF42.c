#include "sf33rd/Source/Game/EFF42.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void EFF42_SUDDENLY(WORK_Other* ewk);
void EFF42_SLIDE_IN(WORK_Other* ewk);
void EFF42_SLIDE_OUT(WORK_Other* ewk);
void EFF42_KILL(WORK_Other* ewk);
void EFF42_MOVE(WORK_Other* ewk);
void Setup_Char_Index(WORK_Other* ewk);

void (*const EFF42_Jmp_Tbl[5])();

void effect_42_move(WORK_Other* ewk) {
    EFF42_Jmp_Tbl[Order[ewk->wu.dir_old]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void EFF42_SUDDENLY(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] != 0) {
            break;
        }

        if (ewk->wu.my_family == 4) {
            ewk->wu.routine_no[6]++;
            ewk->wu.xyz[0].disp.pos = Target_BG_X[3] + Offset_BG_X[3] + Pos_Data_69[ewk->wu.dir_old][0];
            ewk->wu.xyz[1].disp.pos = bg_w.bgw[3].wxy[1].disp.pos + Pos_Data_69[ewk->wu.dir_old][1];
        } else {
            ewk->wu.disp_flag = 1;
            Order[ewk->wu.dir_old] = 3;
            ewk->wu.routine_no[6] = 0;
            ewk->wu.xyz[0].disp.pos = Pos_Data_69[ewk->wu.dir_old][0] + 512;
            ewk->wu.xyz[1].disp.pos = Pos_Data_69[ewk->wu.dir_old][1] + 0;
        }

        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        if (!Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 32)) {
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[6] = 0;
            Order[ewk->wu.dir_old] = 3;
        }

        break;
    }
}

void EFF42_SLIDE_IN(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    if (Order[ewk->wu.dir_old] != 1) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] != 0) {
            break;
        }

        ewk->wu.routine_no[6]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.hit_quake = Pos_Data_69[ewk->wu.dir_old][0] + 512;
        ewk->wu.xyz[1].disp.pos = Pos_Data_69[ewk->wu.dir_old][1] + 0;

        if (Order_Dir[ewk->wu.dir_old] == 4) {
            ewk->wu.xyz[0].disp.pos = 800;
            ewk->wu.mvxy.a[0].sp = -0x100000;
            ewk->wu.mvxy.d[0].sp = 0;
        } else {
            ewk->wu.xyz[0].disp.pos = 224;
            ewk->wu.mvxy.a[0].sp = 0x100000;
            ewk->wu.mvxy.d[0].sp = 0x8000;
        }

        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.hit_quake <= ewk->wu.xyz[0].disp.pos) {
                Order[ewk->wu.dir_old] = 3;
                ewk->wu.routine_no[6] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }
        } else if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            Order[ewk->wu.dir_old] = 3;
            ewk->wu.routine_no[6] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        break;
    }
}

void EFF42_SLIDE_OUT(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] != 0) {
            break;
        }

        ewk->wu.routine_no[6]++;

        if (Order_Dir[ewk->wu.dir_old] == 4) {
            ewk->wu.mvxy.a[0].sp = -0x100000;
            ewk->wu.mvxy.d[0].sp = -0x8000;
        } else {
            ewk->wu.mvxy.a[0].sp = 0x100000;
            ewk->wu.mvxy.d[0].sp = 0x8000;
        }

        break;

    case 1:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (Ck_Range_Out_S(ewk, 2, 16)) {
            ewk->wu.routine_no[6] += 1;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void EFF42_KILL(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] == 0) {
            ewk->wu.routine_no[1] += 1;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void EFF42_MOVE(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
            Time_Stop = 0;
        }

        break;

    case 1:
        if (ewk->wu.rl_waza != Select_Timer) {
            ewk->wu.rl_waza = Select_Timer;
            Setup_Char_Index(ewk);
            set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        }

        break;
    }
}

void Setup_Char_Index(WORK_Other* ewk) {
    s16 xx = Select_Timer & (s8)ewk->wu.routine_no[7];

    xx &= 0xFF;

    if (ewk->wu.routine_no[7] == 240) {
        xx >>= 4;
    }

    if (ewk->wu.dir_old >= 7) {
        ewk->wu.dir_step = xx + 10;
    } else {
        ewk->wu.dir_step = xx;
    }
}

s32 effect_42_init(s16 type) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if (Present_Mode == 4 || Present_Mode == 5) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 42;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = 3;
    ewk->wu.dir_timer = 10;
    ewk->wu.rl_waza = Select_Timer;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.dir_old = type;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (type & 1) {
        ewk->wu.direction = 4;
    } else {
        ewk->wu.direction = 8;
    }

    ewk->wu.char_index = 3;
    ewk->wu.position_z = 14;

    switch (type) {
    case 5:
        ewk->wu.routine_no[7] = 240;
        ix = Select_Timer & 0xF0;
        ix >>= 4;
        ewk->wu.dir_step = ix;
        break;

    case 6:
        ewk->wu.routine_no[7] = 15;
        ewk->wu.dir_step = 0;
        break;

    case 7:
        ewk->wu.routine_no[7] = 240;
        ix = Select_Timer & 0xF0;
        ix >>= 4;
        ewk->wu.dir_step = ix + 10;
        break;

    case 8:
        ewk->wu.routine_no[7] = 15;
        ewk->wu.dir_step = 10;
        break;

    case 9:
        ewk->wu.routine_no[7] = 240;
        ix = Select_Timer & 0xF0;
        ix >>= 4;
        ewk->wu.dir_step = ix + 10;
        ewk->wu.my_family = 4;
        break;

    case 10:
        ewk->wu.routine_no[7] = 15;
        ewk->wu.dir_step = 10;
        ewk->wu.my_family = 4;
        break;
    }

    return 0;
}

void (*const EFF42_Jmp_Tbl[5])() = { EFF42_SUDDENLY, EFF42_SLIDE_IN, EFF42_SLIDE_OUT, EFF42_MOVE, EFF42_KILL };
