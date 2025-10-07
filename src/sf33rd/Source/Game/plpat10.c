#include "sf33rd/Source/Game/plpat10.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLPAT.h"
#include "sf33rd/Source/Game/PLPATUNI.h"
#include "sf33rd/Source/Game/PLS02.h"

void (*const pl10_exatt_table[18])(PLW*);

void pl10_extra_attack(PLW* wk) {
    pl10_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL10_TOKUSHUKOUDOU(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void grade_add_personal_action(s32 ix);
#endif

    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 40:
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
            break;

        case 20:
            wk->wu.cg_type = 0;
            wk->tk_dageki += 10;
            wk->tk_nage += 2;
            break;

        case 64:
            grade_add_personal_action(wk->wu.id);
            break;
        }

        if (wk->tk_dageki > 10) {
            wk->tk_dageki = 10;
        }

        if (wk->tk_nage > 2) {
            wk->tk_nage = 2;
        }

        break;
    }
}

void Att_PL10_MACH_SLIDE2(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->rl_save = wk->wu.rl_flag;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.routine_no[3] != 1) {
            add_mvxy_speed(&wk->wu);
        }

        break;

    case 3:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->rl_save) {
            wk->wu.xyz[0].cal += wk->wu.mvxy.a[0].sp;
        } else {
            wk->wu.xyz[0].cal -= wk->wu.mvxy.a[0].sp;
        }

        wk->wu.xyz[1].cal += wk->wu.mvxy.a[1].sp;

        if (!wk->micchaku_flag) {
            break;
        }

        char_move_z(&wk->wu);

        if (wk->wu.cg_type == 21) {
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 1;
        }

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        break;
    }
}

void (*const pl10_exatt_table[18])(PLW*) = { Att_HADOUKEN,
                                             Att_TENSHINSENKYUUTAI,
                                             Att_SLIDE_and_JUMP,
                                             Att_HADOUKEN,
                                             Att_SLIDE_and_JUMP,
                                             Att_TENSHINSENKYUUTAI,
                                             Att_HADOUKEN,
                                             Att_PL10_MACH_SLIDE2,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL10_TOKUSHUKOUDOU,
                                             Att_DUMMY,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
