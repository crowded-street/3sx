#include "sf33rd/Source/Game/EFF69.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFF57.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Eff59.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void Setup_Clear_OBJ(WORK_Other* ewk);

void (*const EFF69_Jmp_Tbl[5])();

void effect_69_move(WORK_Other* ewk) {
    EFF69_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void EFF69_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF69_SLIDE_IN(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    if (Order[ewk->wu.dir_old] != 1) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        Setup_Clear_OBJ(ewk);

        if (Order_Dir[ewk->wu.dir_old] == 4) {
            ewk->wu.mvxy.a[0].sp = -0x100000;
            ewk->wu.mvxy.d[0].sp = 0;
        } else {
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
                if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                    Order[ewk->wu.dir_old] = 0;
                }

                ewk->wu.routine_no[0] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }
        } else if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                Order[ewk->wu.dir_old] = 0;
            }

            ewk->wu.routine_no[0] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        break;
    }
}

void EFF69_SLIDE_OUT(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.disp_flag == 0) {
            ewk->wu.routine_no[1] = 99;
        } else {
            if (--Order_Timer[ewk->wu.dir_old]) {
                break;
            }
            ewk->wu.routine_no[1]++;
        }

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

        if (Ck_Range_Out_S(ewk, 2, 128)) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void EFF69_SUDDENLY(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos = Pos_Data_69[ewk->wu.dir_old][0] + 512;
        ewk->wu.xyz[1].disp.pos = Pos_Data_69[ewk->wu.dir_old][1] + 0;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

s32 effect_69_init(s16 dir_old) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((dir_old == 3 || dir_old == 4) && (Present_Mode == 4 || Present_Mode == 5)) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 69;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = 3;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.char_index = 16;
    ewk->wu.dir_step = dir_old + 2;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[0].disp.pos = 800;
    ewk->wu.mvxy.a[0].sp = -0xF0000;
    ewk->wu.hit_quake = Pos_Data_69[dir_old][0] + 512;
    ewk->wu.xyz[1].disp.pos = Pos_Data_69[dir_old][1] + 0;
    ewk->wu.position_z = 73 - dir_old;
    return 0;
}

void Setup_Clear_OBJ(WORK_Other* ewk) {
    if (ewk->wu.dir_old < 3) {
        effect_59_init(ewk, ewk->wu.my_family, ewk->wu.dir_old, 1);
    }
}

void (*const EFF69_Jmp_Tbl[5])() = { EFF69_WAIT, EFF69_SLIDE_IN, EFF69_SLIDE_OUT, EFF69_SUDDENLY, EFF57_KILL };
