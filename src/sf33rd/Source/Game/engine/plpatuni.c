/**
 * @file plpatuni.c
 * Various Attacks
 */

#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void att_ahj_table_reader(PLW* wk);

void Att_DUMMY(PLW* /* unused */) {}

void Att_METAMOR_WAIT(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 30) {
            wk->wu.routine_no[3] = 2;
        }

        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type != 30) {
            wk->wu.routine_no[3] = 3;
            wk->wu.mvxy.a[0].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.d[1].sp = -0x8000;
            wk->wu.mvxy.kop[0] = wk->wu.mvxy.kop[1] = 0;
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

const s16 metareb_pos[20][2] = { { 1, 9 },  { 14, 19 }, { 5, 31 }, { 8, 24 },  { 9, 28 },  { 4, 22 }, { 6, -10 },
                                 { 6, 35 }, { 15, 24 }, { 6, 26 }, { 8, 24 },  { 5, 31 },  { 4, 32 }, { 1, 9 },
                                 { 5, 30 }, { 1, 23 },  { 6, 22 }, { 13, 25 }, { -4, 22 }, { 0, 17 } };

void Att_METAMOR_REBIRTH(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] = 1;
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);

        if (wk->wu.xyz[1].disp.pos < 3) {
            wk->wu.xyz[1].disp.pos = -8;
        } else {
            wk->wu.xyz[1].disp.pos -= metareb_pos[wk->player_number][1];

            if (wk->wu.rl_flag) {
                wk->wu.xyz[0].disp.pos += metareb_pos[wk->player_number][0];
            } else {
                wk->wu.xyz[0].disp.pos -= metareb_pos[wk->player_number][0];
            }
        }

        set_char_move_init(&wk->wu, 5, 1);
        wk->metamor_over = 0;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 31) {
            wk->wu.cg_type = 0;
            wk->caution_flag = 0;
            wk->wu.cg_ja = wk->wu.hit_ix_table[wk->wu.cg_hit_ix];
            set_jugde_area(&wk->wu);
            break;
        }

        if (wk->wu.cg_type == 40) {
            wk->wu.routine_no[3] = 2;
            wk->wu.mvxy.a[0].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.d[1].sp = -0x8000;
            wk->wu.mvxy.kop[0] = wk->wu.mvxy.kop[1] = 0;
            wk->scr_pos_set_flag = 1;
        }

        break;

    case 2:
        wk->scr_pos_set_flag = 1;
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        wk->scr_pos_set_flag = 1;
        char_move(&wk->wu);
        break;
    }
}

void Att_HADOUKEN(PLW* wk) {
    wk->scr_pos_set_flag = 0;
    Att_HADOUKEN2(wk);
}

void Att_HADOUKEN2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_NM_OKIAGARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_SHOURYUUKEN(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);
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
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Att_SENPUUKYAKU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->data_ix;
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

        if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 20) {
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_SENPUUKYAKU2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        cal_initial_speed_y(&wk->wu, wk->as->r_no, 0);
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

void Att_ABISEGERI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->r_no);
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

        if (wk->wu.routine_no[3] == 3) {
            if (wk->wu.mvxy.kop[0] == 2) {
                wk->wu.mvxy.kop[0] = 1;
            }

            wk->wu.mvxy.d[1].sp = 0;
            wk->wu.mvxy.a[1].sp = 0;
        }

        break;

    case 3:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        char_move(&wk->wu);

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

void Att_SHOURYUUREPPA(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 40 && (wk->cp->sw_new & 0x770) == 0x70) {
            wk->wu.routine_no[1] = 0;
            wk->wu.routine_no[2] = 6;
            wk->wu.routine_no[3] = 0;
            wk->wu.cg_type = 0;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Att_SHINSHOURYUUKEN(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
        }

        break;
    }
}

void Att_KUUCHUUNICHIRINSHOU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        /* fallthrough */

    case 1:
        jumping_union_process(&wk->wu, 2);

        if (wk->wu.routine_no[3] != 2) {
            if (wk->wu.cg_type == 20) {
                add_to_mvxy_data(&wk->wu, wk->as->data_ix);
                wk->wu.cg_type = 0;
            }

            if (wk->wu.cg_type == 30) {
                add_to_mvxy_data(&wk->wu, wk->as->r_no);
                wk->wu.cg_type = 0;
                return;
            }
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

void Att_KUUCHUUJINNCHUUWATARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
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

        if (wk->wu.routine_no[3] == 3) {
            break;
        }

        if (wk->wu.cg_type == 20) {
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
        }

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_TENSHINSENKYUUTAI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 40:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;

        case 50:
            wk->wu.routine_no[3] = 4;
            wk->wu.cg_type = 0;
            break;

        case 30:
            wk->wu.mvxy.index = wk->as->data_ix;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->wu.routine_no[3] != 4 && wk->hos_fi_flag | wk->hos_em_flag) {
            char_move_cmj4(&wk->wu);
            wk->wu.routine_no[3] = 4;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;

    case 4:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 40:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;

        case 30:
            wk->wu.mvxy.index = wk->as->data_ix;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        break;
    }
}

void Att_CHOUCHUURENGEKI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    default:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 21:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;
        }
    }
}

void Att_SLIDE_and_JUMP(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

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

    case 2:
        jumping_union_process(&wk->wu, 1);

        if ((wk->wu.routine_no[3] != 1) && (wk->wu.cg_type == 20)) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

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

void Att_JINNCHUUWATARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->data_ix;
        break;

    case 1:
        char_move(&wk->wu);
        att_ahj_table_reader(wk);
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

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 2;
        }

        break;
    }
}

void Att_HOMING_JUMP(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->data_ix;
        break;

    case 1:
        char_move(&wk->wu);
        att_ahj_table_reader(wk);
        break;

    case 3:
        wk->wu.routine_no[3] = 2;
        /* fallthrough */

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.routine_no[3] != 1) {
            att_ahj_table_reader(wk);
        }

        break;
    }
}

const s16 ahj_empos_hos[3][20][2] = {
    { { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 },
      { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 } },
    { { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 } },
    { { 56, 56 }, { 48, 48 }, { 40, 40 }, { 40, 28 }, { 64, 40 }, { 52, 40 }, { 64, 56 },
      { 64, 32 }, { 56, 40 }, { 56, 32 }, { 40, 28 }, { 40, 40 }, { 40, 40 }, { 56, 56 },
      { 40, 40 }, { 56, 32 }, { 48, 32 }, { 52, 44 }, { 48, 36 }, { 48, 36 } }
};

const s16 ahj_kop[3][4] = { { 0, 24, 0, 2 }, { 0, 22, 0, 2 }, { 0, 23, 0, 2 } };

void att_ahj_table_reader(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    const s16* curr_kop = ahj_kop[wk->as->r_no];
    s16 ex;
    s16 ey;

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        switch (curr_kop[0]) {
        case 0:
            if (wk->wu.rl_flag) {
                ex = twk->wu.position_x - *ahj_empos_hos[wk->as->r_no][twk->player_number];
            } else {
                ex = twk->wu.position_x + *ahj_empos_hos[wk->as->r_no][twk->player_number];
            }

            ey = ahj_empos_hos[wk->as->r_no][twk->player_number][1];
            wk->wu.mvxy.a[0].sp = 0;
            cal_delta_speed(&wk->wu, curr_kop[1], ex, ey, curr_kop[2], curr_kop[3]);

        default:
            if (wk->wu.rl_flag == 0) {
                wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
                wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
            }

            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
        }
    }

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }
}
