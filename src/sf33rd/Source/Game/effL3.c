#include "sf33rd/Source/Game/effL3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effl3_0000(WORK_Other* ewk);
void effl3_0001(WORK_Other* ewk);
void effl3_0002(WORK_Other* ewk);
void effl3_tobi(WORK_Other* ewk);
void effl3_kie(WORK_Other* ewk);

const s16 effl3_wait_timer[8] = { 3, 0, 30, 50, 40, 8, 20, 1 };

const s16 effl3_data_tbl[112] = { -272, -8,  -1, 1, 18, -32, -8, 0, 272,  -8,  -1, 0, 18, 48,   -8, 0,
                                  272,  2,   1,  0, 17, 96,  2,  0, -272, -8,  -1, 1, 18, -128, -8, 0,
                                  112,  128, 1,  0, 17, 0,   2,  1, -272, 2,   1,  1, 17, -80,  2,  0,
                                  -272, -8,  -1, 1, 18, -64, -8, 0, -272, 2,   1,  1, 17, -112, 2,  0,
                                  272,  -8,  -1, 0, 18, 16,  -8, 0, 272,  -8,  -1, 0, 18, 80,   -8, 0,
                                  272,  2,   1,  0, 17, 192, 2,  0, 272,  128, 1,  0, 17, 160,  2,  1,
                                  -272, 2,   1,  0, 17, 16,  2,  0, 272,  128, 1,  0, 17, 32,   2,  1 };

void effect_L3_move(WORK_Other* ewk) {
    void (*effl3_jp[3])(WORK_Other*) = { effl3_0000, effl3_0001, effl3_0002 };

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        /* fallthrough */

    case 1:
        if (!EXE_flag && !Game_pause) {
            effl3_jp[ewk->wu.routine_no[1]](ewk);
        }

        pl_eff_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effl3_0000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 2);

        if (ewk->wu.rl_flag) {
            ewk->wu.mvxy.a[0].sp = 0x28000;
        } else {
            ewk->wu.mvxy.a[0].sp = -0x28000;
        }

        /* fallthrough */

    case 1:
        char_move(&ewk->wu);
        add_x_sub(ewk);

        if (ewk->wu.rl_flag) {
            if (ewk->wu.xyz[0].disp.pos > ewk->wu.old_rno[0]) {
                ewk->wu.routine_no[2]++;
                set_char_move_init(&ewk->wu, 0, 11);
                break;
            }
        } else {
            if (ewk->wu.xyz[0].disp.pos < ewk->wu.old_rno[0]) {
                ewk->wu.routine_no[2]++;
                set_char_move_init(&ewk->wu, 0, 11);
            }
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, 0);
        }

        break;
    case 3:
        if (Appear_Q) {
            ewk->wu.routine_no[1] = 2;
        }

        char_move(&ewk->wu);
        break;
    }
}

void effl3_0001(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 1, 49);
        ewk->wu.old_rno[2] = 60;
        cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.xyz[1].disp.pos - 32, 2, 2);
        break;

    case 1:
        char_move(&ewk->wu);
        ewk->wu.old_rno[2]--;

        if (ewk->wu.old_rno[2] < 0) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 1, 50);
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[1].sp = -0x6000;
            break;
        }

        add_x_sub(ewk);
        add_y_sub(ewk);
        break;

    case 2:
        char_move(&ewk->wu);
        add_y_sub(ewk);

        if (ewk->wu.xyz[1].disp.pos < ewk->wu.old_rno[1]) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, 0);
        }

        break;

    case 3:
        if (Appear_Q) {
            ewk->wu.routine_no[1] = 2;
        }

        char_move(&ewk->wu);
        break;
    }
}

void effl3_0002(WORK_Other* ewk) {
    s16 work;

    switch (ewk->wu.routine_no[3]) {
    case 0:
        ewk->wu.routine_no[3]++;
        ewk->wu.disp_flag = 1;
        work = random_16();
        ewk->wu.old_rno[4] = effl3_wait_timer[work];
        ewk->wu.old_rno[3] = ewk->wu.type & 1;
        break;

    case 1:
        if (ewk->wu.old_rno[3]) {
            effl3_tobi(ewk);
            break;
        }

        effl3_kie(ewk);
        break;
    }
}

void effl3_tobi(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[4]) {
    case 0:
        ewk->wu.old_rno[4]--;

        if (ewk->wu.old_rno[4] < 0) {
            ewk->wu.routine_no[4]++;
            set_char_move_init(&ewk->wu, 9, 39);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 1) {
            ewk->wu.routine_no[4]++;
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0x78000;
            ewk->wu.mvxy.d[1].sp = -0x6000;
        }

        break;

    case 2:
        add_y_sub(ewk);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 2) {
            break;
        }

        ewk->wu.routine_no[4]++;
        ewk->wu.mvxy.d[0].sp = 0;

        if (ewk->wu.rl_flag) {
            ewk->wu.mvxy.a[0].sp = 0x80000;
        } else {
            ewk->wu.mvxy.a[0].sp = -0x80000;
        }

        ewk->wu.mvxy.a[1].sp = -0x8000;
        ewk->wu.mvxy.d[1].sp = 0x4000;
        break;

        break;

    case 3:
        add_x_sub(ewk);
        add_y_sub(ewk);

        if (!range_x_check3(ewk, 208)) {
            ewk->wu.routine_no[0] = 99;
        }

        break;

    case 4:
        break;
    }
}

void effl3_kie(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[4]) {
    case 0:
        ewk->wu.old_rno[4]--;

        if (ewk->wu.old_rno[4] < 0) {
            ewk->wu.routine_no[4]++;
            set_char_move_init(&ewk->wu, 1, 94);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0] = 99;
        }

        break;
    }
}

s32 effect_L3_init(PLW* oya) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;
    s16 i;
    s16 id_w;
    const s16* data_ptr;

    if (!Perfect_Flag) {
        return 0;
    }

    data_ptr = effl3_data_tbl;
    id_w = oya->wu.id ^ 1;

    for (i = 0; i < 6; i++) {
        if ((ix = pull_effect_work(3)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 213;
        ewk->wu.work_id = 16;
        ewk->my_master = (u32*)oya;
        ewk->master_id = oya->wu.id;
        ewk->wu.cgromtype = 1;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = oya->wu.id ? 8 : 0;
        ewk->wu.type = i;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos;
        ewk->wu.xyz[0].disp.pos += *(s16*)data_ptr++;
        ewk->wu.xyz[1].disp.pos = plw[id_w].wu.xyz[1].disp.pos;
        ewk->wu.xyz[1].disp.pos += *(s16*)data_ptr++;
        ewk->wu.position_z = plw[id_w].wu.position_z;
        ewk->wu.position_z += *(s16*)data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z;
        ewk->wu.rl_flag = *data_ptr++;
        ewk->wu.kage_char = *data_ptr++;
        ewk->wu.old_rno[0] = plw[id_w].wu.xyz[0].disp.pos;
        ewk->wu.old_rno[0] += *(s16*)data_ptr++;
        ewk->wu.old_rno[1] = *data_ptr++;
        ewk->wu.routine_no[1] = *data_ptr++;
        ewk->wu.char_table[0] = oya->wu.char_table[0];
        ewk->wu.char_table[1] = _etc_char_table;
        ewk->wu.char_table[9] = oya->wu.char_table[9];
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 6;
        ewk->wu.kage_hy = 0;
        ewk->wu.kage_prio = ewk->wu.position_z + 5;
        ewk->wu.my_mts = oya->wu.my_mts;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
