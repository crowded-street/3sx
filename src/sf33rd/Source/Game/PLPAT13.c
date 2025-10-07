#include "sf33rd/Source/Game/PLPAT13.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLPAT.h"
#include "sf33rd/Source/Game/PLPATUNI.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"

void (*const pl13_exatt_table[18])(PLW*);

const s16 mnd_em_tall2[21][2] = { { 28, 56 }, { 24, 44 }, { 24, 40 }, { 20, 32 }, { 24, 48 }, { 24, 40 }, { 28, 60 },
                                  { 16, 44 }, { 32, 32 }, { 28, 24 }, { 20, 32 }, { 24, 40 }, { 24, 40 }, { 28, 56 },
                                  { 24, 40 }, { 24, 40 }, { 24, 40 }, { 24, 40 }, { 24, 40 }, { 24, 40 }, { 24, 40 } };

void pl13_extra_attack(PLW* wk) {
    pl13_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_MOONSALT_KNEE_DROP2(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    PLW* twk;
    s16 ex;
    s16 ey;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        twk = (PLW*)wk->wu.target_adrs;

        if (wk->wu.rl_flag) {
            ex = twk->wu.position_x - mnd_em_tall2[twk->player_number][0];
        } else {
            ex = twk->wu.position_x + mnd_em_tall2[twk->player_number][0];
        }

        ey = mnd_em_tall2[twk->player_number][1];
        wk->wu.mvxy.a[0].sp = 0;
        cal_delta_speed(&wk->wu, wk->as->r_no, ex, ey, 2, 2);

        if (wk->wu.rl_flag == 0) {
            wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
            wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
        }

        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL13_TOKUSHUKOUDOU(PLW* wk) {
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

            if (wk->tk_dageki > 10) {
                wk->tk_dageki = 10;
            }

            break;

        case 64:
            grade_add_personal_action(wk->wu.id);
            break;
        }

        break;
    }
}

void (*const pl13_exatt_table[18])(PLW*) = { Att_HADOUKEN,
                                             Att_MOONSALT_KNEE_DROP2,
                                             Att_ABISEGERI,
                                             Att_SENPUUKYAKU,
                                             Att_CHOUCHUURENGEKI,
                                             Att_CHOUCHUURENGEKI,
                                             Att_HADOUKEN,
                                             Att_CHOUCHUURENGEKI,
                                             Att_SLIDE_and_JUMP,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL13_TOKUSHUKOUDOU,
                                             Att_DUMMY,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
