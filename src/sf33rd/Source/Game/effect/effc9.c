/**
 * @file effc9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc9.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff03.h"
#include "sf33rd/Source/Game/effect/eff37.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sound/se_data.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/workuser.h"

// forward declarations

const s32 efy_data[6];
const u8 sel_ejg_ix_table[2][2][4];
const u16 ag_cc_table[8];
const s16 app_pos_hosei[3][3];
const s16 judge_gals_kage_tbl[8][4];
const u8 ag_sel_table[22][4][4];
u32* ag_char_table[];

void effect_C9_move(WORK_Other* ewk) {
    s16 scrc;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 1;
        scrc = get_center_position();
        ewk->wu.xyz[0].disp.pos = app_pos_hosei[ewk->wu.type][0] + scrc;
        ewk->wu.xyz[1].disp.pos = app_pos_hosei[ewk->wu.type][1];
        ewk->wu.xyz[2].disp.pos = app_pos_hosei[ewk->wu.type][2] + 32;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = judge_gals_kage_tbl[ewk->wu.charset_id][0];
        ewk->wu.kage_hy = judge_gals_kage_tbl[ewk->wu.charset_id][1];
        ewk->wu.kage_prio = judge_gals_kage_tbl[ewk->wu.charset_id][2];
        ewk->wu.kage_char = judge_gals_kage_tbl[ewk->wu.charset_id][3];

        if (ewk->wu.type == 1) {
            ewk->wu.kage_hy -= 2;
        }

        set_char_move_init(&ewk->wu, 0, 0);

        if (ewk->wu.charset_id == 7) {
            ewk->wu.next_x = efy_data[0];
            ewk->wu.xyz[0].disp.pos += efy_data[1];
            ewk->wu.next_y = ewk->wu.xyz[1].disp.pos;
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
        ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
        sort_push_request(&ewk->wu);
        break;

    case 1:
        if ((ewk->wu.dead_f == 1) || (Suicide[0] != 0)) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if ((EXE_flag == 0) && (Game_pause == 0)) {
            char_move(&ewk->wu);

            switch (ewk->wu.routine_no[1]) {
            case 0:
                if (ewk->wu.charset_id == 7) {
                    switch (ewk->wu.routine_no[2]) {
                    case 0:
                        ewk->wu.next_x -= 1;

                        if (ewk->wu.next_x > 0) {
                            break;
                        }

                        ewk->wu.routine_no[2] += 1;
                        /* fallthrough */

                    case 1:
                        ewk->wu.mvxy.a[0].sp = efy_data[2];
                        ewk->wu.mvxy.d[0].sp = efy_data[3];
                        ewk->wu.mvxy.a[1].sp = efy_data[4];
                        ewk->wu.mvxy.d[1].sp = efy_data[5];
                        ewk->wu.mvxy.kop[0] = 2;
                        ewk->wu.routine_no[2] += 1;
                        /* fallthrough */

                    case 2:
                        add_mvxy_speed(&ewk->wu);
                        cal_mvxy_speed(&ewk->wu);

                        if (ewk->wu.xyz[1].disp.pos <= ewk->wu.next_y) {
                            ewk->wu.routine_no[2] += 1;
                            ewk->wu.xyz[1].disp.pos = ewk->wu.next_y;
                            ewk->wu.mvxy.d[1].sp = 0;
                            ewk->wu.mvxy.a[1].sp = 0;
                            ewk->wu.mvxy.kop[0] = 1;
                            effect_03_init(&ewk->wu, 110);
                            sound_effect_request[309](ewk, 309);
                            char_move_z(&ewk->wu);
                        }

                        break;

                    default:
                        add_mvxy_speed(&ewk->wu);
                        cal_mvxy_speed(&ewk->wu);
                        break;
                    }
                }

                if (Event_Judge_Gals) {
                    ewk->wu.routine_no[1] += 1;
                    set_char_move_init(&ewk->wu, 0, 1);
                }

                break;

            case 1:
                if (ewk->wu.cg_type == 0xFF) {
                    ewk->wu.routine_no[1] += 1;
                    set_char_move_init(&ewk->wu, 0, 2);
                    effect_37_init(&ewk->wu, ewk->wu.charset_id, EJG_index[ewk->wu.type]);
                }

                break;

            case 2:
                if (ewk->wu.cg_type == 0xFF) {
                    ewk->wu.routine_no[1] += 1;
                    Event_Judge_Gals -= 1;
                }

                break;
            }
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_C9_init(PLW* arg0, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 129;
    ewk->wu.work_id = 16;
    ewk->wu.type = data;
    ewk->wu.charset_id = ag_sel_table[bg_w.stage][Round_num & 3][data];
    ewk->wu.cgromtype = 1;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = ag_cc_table[ewk->wu.charset_id];
    ewk->wu.my_mts = 14;
    ewk->wu.char_table[0] = ag_char_table[ewk->wu.charset_id];
    return 0;
}

void setup_EJG_index() {
    s16 i;
    s16 gra;

    if (judge_gals[0].grade < judge_gals[1].grade) {
        gra = judge_gals[1].grade - judge_gals[0].grade;
    } else {
        gra = judge_gals[0].grade - judge_gals[1].grade;
    }

    if (gra > 5) {
        if (Winner_id) {
            EJG_index[0] = 1;
            EJG_index[1] = 1;
            EJG_index[2] = 1;
            EJG_index[3] = 0xFF;
        } else {
            EJG_index[0] = 0;
            EJG_index[1] = 0;
            EJG_index[2] = 0;
            EJG_index[3] = 0xFF;
        }
    } else {
        for (i = 0; i < 4; i++) {
            EJG_index[i] = sel_ejg_ix_table[Winner_id][Game_timer & 1][i];
        }
    }
}

u32* ag_char_table[] = { _ag_00_char_table, _ag_01_char_table, _ag_02_char_table, _ag_03_char_table,
                         _ag_04_char_table, _ag_05_char_table, _ag_06_char_table, _ag_07_char_table };

const s32 efy_data[6] = { 40, -368, -851968, 32768, 229376, -24576 };

const u8 sel_ejg_ix_table[2][2][4] = { { { 0, 0, 1, 255 }, { 1, 0, 0, 255 } }, { { 1, 1, 0, 255 }, { 0, 1, 1, 255 } } };

const u16 ag_cc_table[8] = { 8257, 8258, 8259, 8260, 8261, 8262, 8263, 8264 };

const s16 app_pos_hosei[3][3] = { { -64, -6, -2 }, { 0, -8, -4 }, { 64, -6, -2 } };

const s16 judge_gals_kage_tbl[8][4] = { { 0, -6, 32, 12 }, { 0, -6, 32, 14 }, { 0, -6, 32, 8 },  { 0, -6, 32, 8 },
                                        { 0, -6, 32, 8 },  { 0, -6, 32, 10 }, { 0, -6, 32, 12 }, { 0, -6, 32, 16 } };

const u8 ag_sel_table[22][4][4] = { { { 6, 7, 3, 0 }, { 6, 5, 1, 0 }, { 6, 0, 5, 0 }, { 4, 7, 1, 0 } },
                                    { { 6, 5, 1, 0 }, { 2, 7, 6, 0 }, { 3, 0, 1, 0 }, { 5, 4, 0, 0 } },
                                    { { 5, 0, 1, 0 }, { 2, 5, 1, 0 }, { 3, 5, 4, 0 }, { 2, 4, 5, 0 } },
                                    { { 2, 5, 4, 0 }, { 6, 5, 4, 0 }, { 0, 3, 1, 0 }, { 2, 3, 4, 0 } },
                                    { { 3, 0, 2, 0 }, { 4, 5, 0, 0 }, { 0, 2, 1, 0 }, { 0, 4, 1, 0 } },
                                    { { 5, 7, 1, 0 }, { 6, 5, 1, 0 }, { 4, 7, 3, 0 }, { 2, 7, 6, 0 } },
                                    { { 2, 0, 5, 0 }, { 5, 7, 1, 0 }, { 3, 0, 1, 0 }, { 0, 7, 1, 0 } },
                                    { { 0, 7, 6, 0 }, { 2, 5, 1, 0 }, { 5, 0, 1, 0 }, { 4, 5, 0, 0 } },
                                    { { 6, 4, 1, 0 }, { 6, 7, 3, 0 }, { 0, 5, 2, 0 }, { 6, 5, 1, 0 } },
                                    { { 4, 5, 0, 0 }, { 6, 7, 1, 0 }, { 3, 0, 1, 0 }, { 6, 5, 0, 0 } },
                                    { { 4, 5, 0, 0 }, { 3, 0, 1, 0 }, { 5, 0, 2, 0 }, { 4, 3, 2, 0 } },
                                    { { 2, 0, 1, 0 }, { 6, 5, 3, 0 }, { 4, 0, 1, 0 }, { 3, 2, 5, 0 } },
                                    { { 5, 4, 1, 0 }, { 6, 0, 1, 0 }, { 0, 3, 2, 0 }, { 6, 0, 1, 0 } },
                                    { { 0, 7, 5, 0 }, { 6, 3, 0, 0 }, { 6, 3, 1, 0 }, { 3, 7, 2, 0 } },
                                    { { 0, 2, 1, 0 }, { 3, 7, 6, 0 }, { 5, 2, 0, 0 }, { 0, 4, 1, 0 } },
                                    { { 4, 5, 0, 0 }, { 3, 0, 1, 0 }, { 5, 0, 2, 0 }, { 4, 3, 2, 0 } },
                                    { { 0, 7, 6, 0 }, { 2, 5, 1, 0 }, { 5, 0, 1, 0 }, { 4, 5, 0, 0 } },
                                    { { 5, 0, 1, 0 }, { 6, 5, 1, 0 }, { 4, 2, 3, 0 }, { 7, 7, 7, 0 } },
                                    { { 5, 7, 1, 0 }, { 6, 5, 1, 0 }, { 4, 7, 3, 0 }, { 2, 7, 6, 0 } },
                                    { { 3, 0, 2, 0 }, { 4, 5, 0, 0 }, { 0, 2, 1, 0 }, { 0, 4, 1, 0 } },
                                    { { 3, 0, 2, 0 }, { 4, 5, 0, 0 }, { 0, 2, 1, 0 }, { 0, 4, 1, 0 } },
                                    { { 3, 0, 2, 0 }, { 4, 5, 0, 0 }, { 0, 2, 1, 0 }, { 0, 4, 1, 0 } } };
