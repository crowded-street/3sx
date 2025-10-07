#include "sf33rd/Source/Game/PLPAT01.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLPAT.h"
#include "sf33rd/Source/Game/PLPATUNI.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"

void (*const pl01_exatt_table[18])(PLW*);

const s16 pl01_ddt_dat[20][2] = { { 46, 11 }, { 32, 12 }, { 28, 13 }, { 16, 14 }, { 36, 15 }, { 16, 16 }, { 60, 17 },
                                  { 20, 18 }, { 24, 19 }, { 12, 20 }, { 16, 14 }, { 28, 13 }, { 28, 13 }, { 46, 11 },
                                  { 28, 13 }, { 24, 21 }, { 18, 22 }, { 52, 23 }, { 18, 16 }, { 38, 24 } };

void pl01_extra_attack(PLW* wk) {
    pl01_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL01_DDT(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    PLW* twk = (PLW*)wk->wu.target_adrs;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->data_ix;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            cal_initial_speed_y(&wk->wu, wk->as->r_no, pl01_ddt_dat[twk->player_number][0]);
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.routine_no[3] != 3 && wk->wu.cg_ja.caix) {
            wk->wu.cg_ja.caix = pl01_ddt_dat[twk->player_number][1];
            wk->wu.h_cat = wk->wu.cg_ja.caix + wk->wu.catch_adrs;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL01_TOKUSHUKOUDOU(PLW* wk) {
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

        if (wk->wu.cg_type == 40) {
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
        }

        switch (wk->wu.cg_type) {
        case 20:
            wk->wu.cg_type = 0;
            wk->tk_dageki += 3;
            wk->tk_nage += 2;
            break;

        case 30:
            wk->wu.cg_type = 0;
            wk->tk_dageki += 2;
            wk->tk_nage += 2;
            break;

        case 64:
            grade_add_personal_action(wk->wu.id);
            break;
        }

        if (wk->tk_dageki > 12) {
            wk->tk_dageki = 12;
        }

        if (wk->tk_nage > 16) {
            wk->tk_nage = 16;
        }

        break;
    }
}

void (*const pl01_exatt_table[18])(PLW*) = { Att_CHOUCHUURENGEKI, Att_SHOURYUUKEN,     Att_HADOUKEN,
                                             Att_HADOUKEN2,       Att_CHOUCHUURENGEKI, Att_SENPUUKYAKU2,
                                             Att_SENPUUKYAKU,     Att_NM_OKIAGARI,     Att_PL01_DDT,
                                             Att_HOMING_JUMP,     Att_SLIDE_and_JUMP,  Att_DUMMY,
                                             Att_DUMMY,           Att_DUMMY,           Att_PL01_TOKUSHUKOUDOU,
                                             Att_DUMMY,           Att_METAMOR_WAIT,    Att_METAMOR_REBIRTH };
