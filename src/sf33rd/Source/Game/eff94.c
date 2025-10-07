#include "sf33rd/Source/Game/eff94.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/eff05.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 eff94_data_tbl[5][10] = { { 2, 8492, 392, 64, 12, 22, 52, 0, 0, 1 },
                                    { 2, 8492, 408, 75, 11, 23, 53, 0, 1, 1 },
                                    { 2, 8492, 640, 16, 10, 9, 10, 0, 4, 1 },
                                    { 2, 8492, 352, 272, 10, 2, 2, 0, 2, 1 },
                                    { 2, 8492, 352, 272, 10, 2, 2, 0, 3, 0 } };

const s16 eff94_2000_tbl[8] = { 288, 336, 320, 288, 328, 296, 344, 304 };

const s8 eff94_2000_1_tbl[16] = { 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1 };

const s16 eff94_3000_tbl[4][3] = { { 328, 304, 0 }, { 288, 240, 1 }, { 280, 320, 0 }, { 352, 264, 1 } };

void effect_94_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    void (*const eff94_move_jp[5])(WORK_Other*) = {
        eff94_0000, eff94_1000, eff94_2000, eff94_3000, eff94_4000,
    };

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            eff94_move_jp[ewk->wu.routine_no[1]](ewk);
        }

        if (ewk->wu.old_rno[1]) {
            disp_pos_trans_entry_r(ewk);
            break;
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff94_0000(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (bg_w.quake_y_index > 3) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 1:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[2]);
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2] = 0;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
        }

        break;
    }
}

void eff94_1000(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (bg_w.quake_y_index > 3) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[2]);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            if (ewk->wu.xyz[0].disp.pos >= 452) {
                ewk->wu.routine_no[2]++;
                set_char_move_init(&ewk->wu, 0, 47);
                ewk->wu.mvxy.a[0].sp = 0x8000;
                ewk->wu.mvxy.a[1].sp = 0;
                ewk->wu.mvxy.d[0].sp = 0;
                ewk->wu.mvxy.d[1].sp = -0x6000;
                break;
            }

            ewk->wu.routine_no[2] = 0;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
            break;
        }

        break;

    case 2:
        char_move(&ewk->wu);
        add_x_sub(ewk);
        add_y_sub(ewk);

        if (ewk->wu.xyz[1].disp.pos < 11) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, 48);
            ewk->wu.xyz[0].disp.pos += 2;
            ewk->wu.old_rno[1] = 0;
            break;
        }

        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2]++;
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
        }

        break;
    }
}

void eff94_2000(WORK_Other* ewk) {
    void (*const eff94_2000_jp[5])(
        WORK_Other*) = { eff94_2000_0, eff94_2000_1, eff94_2000_2, eff94_2000_3, eff94_2000_4 };
    eff94_2000_jp[ewk->wu.routine_no[2]](ewk);
}

void eff94_2000_0(WORK_Other* ewk) {
    ewk->wu.disp_flag = 0;

    if (bg_w.quake_y_index > 3) {
        ewk->wu.routine_no[2]++;

        if (bg_w.quake_y_index >= 24) {
            ewk->wu.old_rno[3] = 1;
            return;
        }

        ewk->wu.old_rno[3] = 0;
    }
}

void eff94_2000_1(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;

    ewk->wu.routine_no[2]++;
    set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
    ewk->wu.disp_flag = 1;
    work = random_16();

    if (eff94_2000_1_tbl[work]) {
        ewk->wu.routine_no[2] = 4;
        ewk->wu.dir_timer = 8;
        return;
    }

    if (work == 7 && ewk->wu.old_rno[3]) {
        effect_94_init(4);
        effect_94_init(4);
        effect_94_init(4);
    }

    work &= 7;
    ewk->wu.xyz[0].disp.pos = eff94_2000_tbl[work];
    ewk->wu.xyz[1].cal = 0x1100000;
    ewk->wu.mvxy.a[1].sp = -0x68000;
    ewk->wu.mvxy.d[1].sp = -0x6000;
}

void eff94_2000_2(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    add_y_sub(ewk);

    if (ewk->wu.xyz[1].disp.pos < 17) {
        ewk->wu.routine_no[2]++;
        ewk->wu.xyz[1].disp.pos = 16;
        set_char_move_init(&ewk->wu, 0, 3);
    }
}

void eff94_2000_3(WORK_Other* ewk) {
    char_move(&ewk->wu);

    if ((ewk->wu.xyz[0].disp.pos <= bg_w.bgw[1].l_limit2 - bg_w.pos_offset) ||
        ewk->wu.xyz[0].disp.pos >= bg_w.bgw[1].r_limit2 + bg_w.pos_offset) {
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 0;
        ewk->wu.dir_timer = 4;
    }
}

void eff94_2000_4(WORK_Other* ewk) {
    ewk->wu.dir_timer--;

    if (ewk->wu.dir_timer <= 0) {
        ewk->wu.routine_no[2] = 0;
    }
}

void eff94_3000(WORK_Other* ewk) {
    void (*const eff94_2000_jp[4])(WORK_Other*) = { eff94_3000_0, eff94_2000_2, eff94_2000_3, eff94_3000_4 };
    eff94_2000_jp[ewk->wu.routine_no[2]](ewk);
}

void eff94_3000_0(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;

    ewk->wu.routine_no[2]++;
    set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
    ewk->wu.disp_flag = 1;
    work = random_16();
    work &= 3;
    ewk->wu.xyz[0].disp.pos = eff94_3000_tbl[work][0];
    ewk->wu.xyz[1].disp.pos = eff94_3000_tbl[work][1];
    ewk->wu.rl_flag = eff94_3000_tbl[work][2];
    ewk->wu.mvxy.a[1].sp = -0x68000;
    ewk->wu.mvxy.d[1].sp = -0x6000;
}

void eff94_3000_4(WORK_Other* ewk) {
    ewk->wu.routine_no[0] = 99;
}

void eff94_4000(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (bg_w.quake_y_index > 3) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 1:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[2]);
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            if (bg_w.quake_y_index > 3) {
                ewk->wu.routine_no[2] = 1;
                break;
            }
        } else if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2] = 0;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[0]);
        }

        break;
    }
}

s32 effect_94_init(u8 type94) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr = eff94_data_tbl[type94];

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 94;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.type = type94;
    ewk->wu.dead_f = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = char_add[bg_w.bg_index];
    ewk->wu.my_family = *data_ptr++;
    ewk->wu.my_col_code = *data_ptr++;
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.position_z = ewk->wu.my_priority = *data_ptr++;
    ewk->wu.old_rno[0] = ewk->wu.char_index = *data_ptr++;
    ewk->wu.old_rno[2] = *data_ptr++;
    ewk->wu.hit_stop = *data_ptr++;
    ewk->wu.routine_no[1] = *data_ptr++;
    ewk->wu.old_rno[1] = *data_ptr++;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
