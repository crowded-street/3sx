#include "sf33rd/Source/Game/plpat19.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLPAT.h"
#include "sf33rd/Source/Game/PLPATUNI.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/cmd_data.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/effect/effk7.h"
#include "sf33rd/Source/Game/effect/effl0.h"
#include "sf33rd/Source/Game/workuser.h"

s32 kabe_check3(PLW* wk);
u8 get_lever_dir(PLW* wk);

void (*const pl19_exatt_table[18])(PLW*);

void pl19_extra_attack(PLW* wk) {
    pl19_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_METAMORPHOSE(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        reset_mvxy_data(&wk->wu);
        wk->metamorphose = 0;
        wk->metamor_over = 0;

        if ((Bonus_Game_Flag != 20) && (effect_K7_init(wk) != -1)) {
            set_char_move_init(&wk->wu, 5, wk->as->char_ix);
            break;
        }

        set_char_move_init(&wk->wu, 5, wk->as->char_ix + 2);
        wk->wu.routine_no[3] = 9;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[2] = 32;
            wk->wu.routine_no[3] = 1;
        }

        break;

    case 9:
        char_move(&wk->wu);
        break;
    }
}

const s16 dra_em_tall[20][2] = { { 24, 16 }, { 28, 16 }, { 16, 16 }, { 16, 16 }, { 20, 16 }, { 20, 16 }, { 18, 16 },
                                 { 18, 16 }, { 28, 16 }, { 25, 16 }, { 16, 16 }, { 16, 16 }, { 16, 16 }, { 24, 16 },
                                 { 16, 16 }, { 16, 16 }, { 16, 16 }, { 24, 16 }, { 20, 16 }, { 20, 16 } };

void Att_SA__D_R_A(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    PLW* emwk;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if ((wk->wu.routine_no[3] != 3) && (wk->wu.cg_type == 20)) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.routine_no[3] == 3) {
            if (wk->wu.mvxy.kop[0] == 2) {
                wk->wu.mvxy.kop[0] = 1;
            }

            wk->wu.mvxy.d[1].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 21) {
            reset_mvxy_data(&wk->wu);
            wk->wu.routine_no[3] = 5;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 30) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            emwk = (PLW*)wk->wu.target_adrs;
            wk->wu.xyz[0].disp.pos = emwk->wu.xyz[0].disp.pos;
            wk->wu.xyz[1].disp.pos = emwk->wu.xyz[1].disp.pos + -224;

            if (wk->wu.xyz[1].disp.pos < 0) {
                wk->wu.xyz[1].disp.pos = 0;
            }
        }

        if ((wk->wu.cg_type == 64) || (wk->wu.cg_type == 0xFF)) {
            wk->wu.routine_no[3] = 5;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.a[0].sp = 0;
        }

        break;

    case 4:
        jumping_union_process(&wk->wu, 5);

        if ((wk->wu.routine_no[3] != 5) && (wk->wu.cg_type == 20)) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.routine_no[3] == 5) {
            if (wk->wu.mvxy.kop[0] == 2) {
                wk->wu.mvxy.kop[0] = 1;
            }

            wk->wu.mvxy.d[1].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 5:
        char_move(&wk->wu);
        break;

    default:
        char_move(&wk->wu);
    }
}

void Att_EX__D_R_A(PLW* wk) {
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
        setup_mvxy_data(&wk->wu, wk->as->r_no);
        twk = (PLW*)wk->wu.target_adrs;

        if (wk->wu.rl_flag) {
            ex = twk->wu.position_x - dra_em_tall[twk->player_number][0];
        } else {
            ex = twk->wu.position_x + dra_em_tall[twk->player_number][0];
        }

        ey = dra_em_tall[twk->player_number][1];
        wk->wu.mvxy.a[0].sp = 0;
        cal_delta_speed(&wk->wu, 8, ex, ey, 2, 2);

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
            wk->wu.mvxy.kop[1] = 2;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if ((wk->wu.routine_no[3] != 3) && (wk->wu.cg_type == 20)) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.routine_no[3] == 3) {
            if (wk->wu.mvxy.kop[0] == 2) {
                wk->wu.mvxy.kop[0] = 1;
            }

            wk->wu.mvxy.d[1].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 64 || wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.a[0].sp = 0;
        }

        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

void Att_KUUCHUUHISSATU(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->r_no);
        /* fallthrough */

    case 1:
        jumping_union_process(&wk->wu, 2);

        if ((wk->wu.routine_no[3] != 2) && (wk->wu.cg_type == 20)) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.routine_no[3] == 2) {
            if (wk->wu.mvxy.kop[0] == 2) {
                wk->wu.mvxy.kop[0] = 1;
            }

            wk->wu.mvxy.d[1].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 2:
        char_move(&wk->wu);
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 21) {
            reset_mvxy_data(&wk->wu);
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 64 || wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.a[0].sp = 0;
        }

        break;
    default:
        char_move(&wk->wu);
        break;
    }
}

void Att_AIRDASH(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
    void add_to_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        if (kabe_check3(wk) != 0) {
            wk->wu.rl_flag = (wk->wu.rl_flag + 1) & 1;
            wk->wu.xyz[0].disp.pos = wk->wu.rl_flag ? bg_w.bgw[1].l_limit2 - 192 : bg_w.bgw[1].r_limit2 + 192;
            set_char_move_init(&wk->wu, 5, 65);
            wk->wu.routine_no[3] = 5;
            wk->wu.cg_type = 0;
            effect_I3_init(&wk->wu, 4);
            break;
        }

        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 25:
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 30:
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
            break;
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);

        if (kabe_check3(wk)) {
            wk->wu.rl_flag = wk->wu.rl_flag + 1 & 1;
            wk->wu.xyz[0].disp.pos = wk->wu.rl_flag ? bg_w.bgw[1].l_limit2 - 192 : bg_w.bgw[1].r_limit2 + 192;
            set_char_move_init(&wk->wu, 5, 65);
            wk->wu.routine_no[3] = 5;
            wk->wu.cg_type = 0;
            effect_I3_init(&wk->wu, 4);
        }

        break;

    case 4:
        char_move(&wk->wu);
        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
        }

        break;

    case 6:
        wk->wu.routine_no[3] = 1;
        char_move_cmj4(&wk->wu);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;
    }
}

s32 kabe_check3(PLW* wk) {
    if (get_lever_dir(wk) != 1) {
        return 0;
    }

    if (wk->wu.xyz[1].disp.pos < 33) {
        return 0;
    }

    return (wk->wu.rl_flag + wk->micchaku_flag == 2);
}

void Att_pl19_TOKUSHUKOUDOU(PLW* wk) {
#if defined(TARGET_PS2)
    void grade_add_personal_action(s32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);

        if (wk->sa->ok == -1) {
            wk->wu.routine_no[3] = 2;
            set_char_move_init(&wk->wu, 5, 62);
            break;
        }

        if (wk->wu.disp_flag != 1 || wk->wu.my_col_mode != 0x4200) {
            wk->wu.routine_no[3] = 2;
            set_char_move_init(&wk->wu, 5, 64);
            break;
        }

        set_char_move_init(&wk->wu, 5, 63);
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 40:
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
            break;

        case 0xFF:
            grade_add_personal_action(wk->wu.id);
            effect_L0_init(&wk->wu, 180);
            break;
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

void Att_AIR_A_X_E(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
    void add_to_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;

        if (wk->wu.mvxy.d[1].sp >= 0) {
            wk->wu.mvxy.d[1].sp = wk->wu.move_xy_table[64];
            wk->wu.mvxy.d[1].sp <<= 8;
        }

        wk->wu.mvxy.kop[1] = 0;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.routine_no[3]++;
        }

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.routine_no[3] != 3) {
            if (wk->wu.cg_type == 20) {
                add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
                wk->wu.cg_type = 0;
                wk->wu.mvxy.index++;
            }

            if (wk->wu.cg_type == 30) {
                setup_mvxy_data(&wk->wu, wk->as->data_ix);
                wk->wu.cg_type = 0;
            }
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

u8 get_lever_dir(PLW* wk) {
    u8 num;

    if (wk->wu.work_id == 1) {
        if (wk->py->flag == 0) {
            num = wcp[wk->wu.id].lever_dir;
        } else {
            num = 0;
        }
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].lever_dir;
    }

    return num;
}

void (*const pl19_exatt_table[18])(PLW*) = {
    Att_HADOUKEN,     Att_AIRDASH,   Att_KUUCHUUHISSATU,     Att_HADOUKEN,       Att_HADOUKEN,       Att_EX__D_R_A,
    Att_METAMORPHOSE, Att_AIR_A_X_E, Att_HADOUKEN,           Att_JINNCHUUWATARI, Att_SLIDE_and_JUMP, Att_SA__D_R_A,
    Att_DUMMY,        Att_DUMMY,     Att_pl19_TOKUSHUKOUDOU, Att_DUMMY,          Att_METAMOR_WAIT,   Att_METAMOR_REBIRTH
};
