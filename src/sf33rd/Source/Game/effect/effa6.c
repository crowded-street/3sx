/**
 * @file effa6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa6.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effb6.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/work_sys.h"

s16 effa6_pos_x_1p;
s16 effa6_pos_y_1p;
s16 effa6_pos_z_1p;
s16 effa6_pos_x_2p;
s16 effa6_pos_y_2p;
s16 mmes_already;

const s16 effA6_pl2_data_tbl[20][64] = {
    { 1, 210, 2, 210, 3, 210, 0, 630, 7, 210, 8, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 630, 4, 210, 5, 210, 6, 210, 0, 420, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 630, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 420, 3, 210, 4, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 210, 2, 420, 4, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 420, 3, 420, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 840, 6, 210, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 420, 3, 210, 4, 210, 5, 420, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 630, 4, 210, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 210, 2, 210, 3, 630, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 420, 4, 210, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 420, 3, 630, 0, -1,  0, 0,   0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 210, 3, 630, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 630, 4, 210, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 420, 3, 210,  4, 210, 5, 210, 6, 420, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 210, 2, 1050, 7, 210, 0, -1,  0, 0,   0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 420, 3, 210, 4, 420, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 210, 2, 630, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 630, 5, 210, 6, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 420, 3, 210, 4, 630, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 420, 3, 210, 4, 630, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 210, 2, 630, 5, 210, 6, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 630, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 420, 3, 210, 4, 420, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 840, 5, 210, 6, 210, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 210, 2, 210, 3, 210, 4, 630, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 630, 4, 210, 5, 210, 6, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 210, 2, 210, 3, 840, 0, -1,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 210, 3, 1050, 0, -1,  0, 0,   0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 630, 4, 210, 5, 210,  6, 210, 7, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 210, 2, 210, 3, 630, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 630, 4, 210, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 420, 3, 210, 4, 420, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      1, 210, 2, 630, 5, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 420, 3, 210, 4, 210, 5, 630, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 210, 2, 840, 6, 210, 7, 210, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void wwwk_set(WORK_Other_CONN* ewk);

s32 check2_A6_shortcut() {
    u16 sw_w;

    if (Player_id) {
        sw_w = p2sw_0;
    } else {
        sw_w = p1sw_0;
    }

    if (sw_w & 0x4000) {
        return 1;
    }

    return 0;
}

void effect_A6_move(WORK_Other_CONN* ewk) {
    WORK_Other* mwk;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (check2_A6_shortcut() != 0) {
            Next_Step |= ~0x7F;
        }

        if (Auto_Cut_Sub() != 0) {
            if (ewk->wu.routine_no[6] < 211) {
                ewk->wu.routine_no[6] = 1;
            } else if (ewk->wu.routine_no[6] < 421) {
                ewk->wu.routine_no[6] = 210;
            } else if (ewk->wu.routine_no[6] < 631) {
                ewk->wu.routine_no[6] = 420;
            } else if (ewk->wu.routine_no[6] < 841) {
                ewk->wu.routine_no[6] = 630;
            } else {
                ewk->wu.routine_no[6] = 840;
            }
        }

        ewk->wu.routine_no[6] = ewk->wu.routine_no[6] - 1;

        if (ewk->wu.routine_no[6] <= 0) {
            ewk->wu.routine_no[6] = effA6_pl2_data_tbl[ewk->master_player][ewk->wu.routine_no[5] + 1];

            if (ewk->wu.routine_no[6] < 0) {
                ewk->wu.routine_no[6] = -1;
                Next_Step |= ~0x7F;
            } else {
                if (!(mmes_already = effA6_pl2_data_tbl[ewk->master_player][ewk->wu.routine_no[5]])) {
                    ewk->wu.disp_flag = 0;
                    ewk->wu.routine_no[5] = ewk->wu.routine_no[5] + 2;
                } else {
                    ewk->wu.routine_no[5] = ewk->wu.routine_no[5] + 2;
                    get_message_conn_data(ewk, 1, ewk->master_player, mmes_already);
                    ewk->wu.disp_flag = 1;
                    ewk->wu.vitality = 0xF0;
                    ewk->wu.routine_no[1] = 0;
                }
            }
        }

        if (Suicide[3]) {
            ewk->wu.routine_no[0] = 1;
            break;
        }

        if (!ewk->wu.disp_flag) {
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;

            switch (ewk->wu.dir_old) {
            case 0x43:
                ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 384;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = bg_w.bgw[0].xy[1].disp.pos + 186;
                break;

            default:
                ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 72;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = bg_w.bgw[0].xy[1].disp.pos + 26;
                break;
            }

            /* fallthrough */

        case 1:
            switch (ewk->wu.dir_old) {
            case 0x43:
                ewk->wu.position_x += 10;

                if (ewk->wu.position_x >= bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 192) {
                    ewk->wu.routine_no[1]++;
                    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos =
                        bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 192;
                }

                break;

            default:
                ewk->wu.position_x -= 10;

                if (ewk->wu.position_x <= bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 120) {
                    ewk->wu.routine_no[1]++;
                    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos =
                        bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 120;
                }

                break;
            }

            break;

        case 2:
            break;
        }

        mwk = (WORK_Other*)ewk->my_master;

        switch (ewk->wu.dir_old) {
        case 0x43:
            ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = mwk->wu.position_z - 1;
            effa6_pos_x_1p = mwk->wu.position_x;
            effa6_pos_y_1p = mwk->wu.position_y;
            effa6_pos_z_1p = mwk->wu.position_z;
            break;

        default:
            ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = mwk->wu.position_z - 1;
            effa6_pos_x_2p = mwk->wu.position_x;
            effa6_pos_y_2p = mwk->wu.position_y;
            effa6_pos_y_2p = mwk->wu.position_y;
            break;
        }

        sort_push_request3(&ewk->wu);
        break;

    case 1:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_A6_init(WORK_Other* mwk) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    wwwk_set(ewk);
    ewk->wu.dir_old = mwk->wu.dir_old;
    ewk->wu.routine_no[6] = 60;
    ewk->master_player = My_char[Player_id];
    ewk->my_master = (u32*)mwk;

    switch (ewk->wu.dir_old) {
    case 0x43:
        if (!Player_id) {
            ewk->wu.routine_no[5] = 0;
        } else {
            ewk->wu.routine_no[5] = 32;
        }

        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 64;
        break;

    default:

        if (Player_id) {
            ewk->wu.routine_no[5] = 0;
        } else {
            ewk->wu.routine_no[5] = 32;
        }

        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 64;
        break;
    }

    return 0;
}

void wwwk_set(WORK_Other_CONN* ewk) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = 106;
    ewk->wu.work_id = 16;
    ewk->wu.rl_flag = 0;
    ewk->wu.cgromtype = 1;
    ewk->wu.sync_suzi = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_mts = 12;
    ewk->wu.my_col_code = 55;
    ewk->wu.my_family = 1;
    ewk->wu.my_priority = 35;
    ewk->wu.position_y = 24;
    ewk->wu.position_z = 35;
}
