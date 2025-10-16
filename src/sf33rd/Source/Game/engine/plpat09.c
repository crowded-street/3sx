/**
 * @file plpat09.c
 * Oro Attacks
 */

#include "sf33rd/Source/Game/engine/plpat09.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff13.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void mvxy_table_reader(PLW* wk);

const u8 tenguiwa_stand_by[2][8] = { { 24, 25, 26, 27, 28, 29, 30, 30 }, { 31, 32, 33, 34, 35, 34, 33, 31 } };

const s16 tenguiwa_pos_hosei[4][6] = {
    { 8, 112, 2, 4, 64, 48 }, { 48, 104, -2, 40, 52, 96 }, { -48, 100, 2, -40, 56, 144 }, { -8, 96, -2, 0, 24, 192 }
};

const s16 tenguiwa_pos_hosei2[8][6] = { { 72, 100, 2, 68, 80, 48 },   { 32, 132, -2, 32, 56, 96 },
                                        { 8, 112, 2, 4, 76, 144 },    { -32, 130, -2, -32, 52, 192 },
                                        { -64, 100, 2, -64, 78, 48 }, { 16, 144, -2, 16, 96, 96 },
                                        { -8, 144, 2, -8, 96, 144 },  { 0, 160, -2, 0, 104, 192 } };

const s16 pl09_tk_table[14] = { 0, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700 };

const s16 homing_hos[2][20][2] = { { { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 },
                                     { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 },
                                     { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 } },
                                   { { 24, 86 }, { 28, 68 }, { 16, 62 }, { 16, 52 }, { 20, 72 }, { 20, 58 }, { 18, 82 },
                                     { 18, 52 }, { 28, 45 }, { 25, 42 }, { 16, 52 }, { 16, 62 }, { 16, 62 }, { 24, 86 },
                                     { 16, 62 }, { 16, 62 }, { 16, 62 }, { 24, 86 }, { 20, 58 }, { 20, 72 } } };

const s16 homing_kop[2][4] = { { 1, 14, 0, 2 }, { 0, 14, 0, 2 } };

void (*const pl09_exatt_table[18])(PLW*);

void pl09_extra_attack(PLW* wk) {
    pl09_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_SP_YAGYOUDAMA(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
        }

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 3;
            break;
        }

        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
        }

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 2;
        }

        break;
    }
}

s32 set_tenguiwa(PLW* wk, u8 data) {
    s16 i;
    s16 j;
    u16 num;
    const u8* tengu;
    WORK* tmw;

    if (!data) {
        tengu = tenguiwa_stand_by[0];

        for (i = 0; i < 3; i++) {
            effect_13_init(&wk->wu, tengu[random_16() & 7]);
        }

        for (j = 0, i = 0; i < 8; i++) {
            if (!get_my_shell_ix(&wk->wu, i, &tmw)) {
                continue;
            }

            num = tmw->type - 24;

            if (num < 36) {
                tmw->old_pos[0] = tenguiwa_pos_hosei[j][0];
                tmw->old_pos[1] = tenguiwa_pos_hosei[j][1];
                tmw->old_pos[2] = tenguiwa_pos_hosei[j][2];
                tmw->scr_mv_x = tenguiwa_pos_hosei[j][3];
                tmw->scr_mv_y = tenguiwa_pos_hosei[j][4];
                tmw->direction = tenguiwa_pos_hosei[j][5];

                j++;

                if (j > 2) {
                    break;
                }
            }
        }

        return 0;
    }

    tengu = tenguiwa_stand_by[1];

    for (i = 0; i < 5; i++) {
        effect_13_init(&wk->wu, tengu[random_16() & 7]);
    }

    for (j = 0, i = 0; i < 8; i++) {
        if (get_my_shell_ix(&wk->wu, i, &tmw)) {
            num = tmw->type - 24;

            if (num < 36) {
                tmw->old_pos[0] = tenguiwa_pos_hosei2[j][0];
                tmw->old_pos[1] = tenguiwa_pos_hosei2[j][1];
                tmw->old_pos[2] = tenguiwa_pos_hosei2[j][2];
                tmw->scr_mv_x = tenguiwa_pos_hosei2[j][3];
                tmw->scr_mv_y = tenguiwa_pos_hosei2[j][4];
                tmw->direction = tenguiwa_pos_hosei2[j][5];

                j++;

                if (j > 4) {
                    break;
                }
            }
        }
    }

    return 0;
}

void Att_PL09_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->tk_success = 0;
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

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;

            if (++wk->tk_success > 13) {
                wk->tk_success = 13;
            }
        }

        if (wk->wu.cg_type == 30) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            break;
        }

        wk->py->now.timer -= wk->py->recover * pl09_tk_table[wk->tk_success] / 100;

        if (wk->py->now.quantity.h <= 0) {
            wk->py->now.timer = 0;
        }

        break;

    default:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 64) {
            grade_add_personal_action(wk->wu.id);
        }

        break;
    }
}

void Att_JINNCHUUWATARI_EX(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->pl09_dat_index = wk->as->r_no;
        wk->wu.mvxy.index = wk->as->data_ix;
        break;

    case 1:
        char_move(&wk->wu);
        mvxy_table_reader(wk);
        break;

    case 2:
        mvxy_table_reader(wk);
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = (wk->wu.routine_no[1] == 0) ? 2 : 4;
            break;
        }

        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = (wk->wu.routine_no[1] == 0) ? 2 : 3;
        }

        break;
    }
}

void mvxy_table_reader(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    const s16* curr_kop = &homing_kop[wk->pl09_dat_index][0];
    s16 ex;
    s16 ey;

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;

        switch (curr_kop[0]) {
        case 0:
            if (wk->wu.xyz[0].disp.pos < twk->wu.xyz[0].disp.pos) {
                ex = twk->wu.xyz[0].disp.pos - homing_hos[wk->pl09_dat_index][twk->player_number][0];

                if (!wk->wu.rl_flag) {
                    ex = wk->wu.xyz[0].disp.pos - (ex - wk->wu.xyz[0].disp.pos);
                }
            } else {
                ex = twk->wu.xyz[0].disp.pos + homing_hos[wk->pl09_dat_index][twk->player_number][0];

                if (wk->wu.rl_flag) {
                    ex = wk->wu.xyz[0].disp.pos + (wk->wu.xyz[0].disp.pos - ex);
                }
            }

            ey = homing_hos[wk->pl09_dat_index][twk->player_number][1];
            wk->wu.mvxy.a[0].sp = 0;
            cal_initial_speed(&wk->wu, curr_kop[1], ex, ey);
            wk->pl09_dat_index++;
            break;

        case 1:
            ex = wk->wu.xyz[0].disp.pos;

            if (wk->wu.xyz[0].disp.pos < twk->wu.xyz[0].disp.pos) {
                ex += (twk->wu.xyz[0].disp.pos - wk->wu.xyz[0].disp.pos) / 2;
            } else {
                ex -= (wk->wu.xyz[0].disp.pos - twk->wu.xyz[0].disp.pos) / 2;
            }

            ey = homing_hos[wk->pl09_dat_index][twk->player_number][1];
            wk->wu.mvxy.a[0].sp = 0;
            cal_initial_speed(&wk->wu, curr_kop[1], ex, ey);
            wk->pl09_dat_index++;
            break;
        }

        if (wk->wu.rl_flag == 0) {
            wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
            wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
        }

        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }
}

void Att_PL09_EX_TENGUIWA(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->sa->dtm_mul = 2;
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL09_EX_KISHINRIKI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        wk->sa->dtm_mul = 16;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
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

void (*const pl09_exatt_table[18])(PLW*) = {
    Att_HADOUKEN,      Att_SHOURYUUKEN,      Att_KUUCHUUNICHIRINSHOU,   Att_HADOUKEN,       Att_HADOUKEN,
    Att_HADOUKEN,      Att_HADOUKEN,         Att_KUUCHUUJINNCHUUWATARI, Att_JINNCHUUWATARI, Att_JINNCHUUWATARI_EX,
    Att_SP_YAGYOUDAMA, Att_PL09_EX_TENGUIWA, Att_PL09_EX_KISHINRIKI,    Att_DUMMY,          Att_PL09_TOKUSHUKOUDOU,
    Att_DUMMY,         Att_METAMOR_WAIT,     Att_METAMOR_REBIRTH
};
