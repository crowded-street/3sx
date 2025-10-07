#include "sf33rd/Source/Game/EFF74.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFF57.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 EFF74_Pos_Data[3][2][2] = { { { 0, 148 }, { 0, 116 } },
                                      { { -48, 104 }, { 48, 104 } },
                                      { { -48, 80 }, { 48, 80 } } };

void (*const EFF74_Jmp_Tbl[5])();

void effect_74_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (Order[ewk->wu.dir_old] == 4) {
        ewk->wu.routine_no[0] = 4;
    }

    EFF74_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag == 0) {
        return;
    }

    if (Menu_Cursor_Y[0] == ewk->master_id) {
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, 2, 0);
    } else {
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, 1, 0);
    }

    sort_push_request4(&ewk->wu);
}

void EFF74_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF74_SUDDENLY(WORK_Other* ewk) {
    s16 pos_y;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;

        if (ewk->master_id) {
            pos_y = 104;
        } else {
            pos_y = 136;
        }

        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 0;
        ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + pos_y + 12;
        ewk->wu.position_z = 64;

        if (ewk->master_id) {
            pos_y = 104;
        } else {
            pos_y = 136;
        }

        ewk->wu.position_x =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + EFF74_Pos_Data[ewk->wu.type][ewk->master_id][0];
        ewk->wu.position_y =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + EFF74_Pos_Data[ewk->wu.type][ewk->master_id][1];
        ewk->wu.position_z = 64;
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

void (*const EFF74_Jmp_Tbl[5])() = { EFF74_WAIT, EFF74_SUDDENLY, EFF74_SUDDENLY, EFF74_SUDDENLY, EFF57_KILL };
