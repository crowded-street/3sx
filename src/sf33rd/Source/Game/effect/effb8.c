/**
 * @file effb8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb8.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effb6.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

u16 effb8_normal_or_senyou();
u16 effb8_sel_1_by_8();
void wk_set(WORK_Other_CONN* ewk);

s16 test_pl_no;
s16 test_mes_no;
s16 test_in;
s16 old_mes_no2;
s16 old_mes_no3;
s16 old_mes_no_pl;
s16 mes_timer;

void effect_B8_move(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.my_mts = 12;
        mes_timer = mes_timer - 1;

        if (--mes_timer > 0) {
            break;
        }

        ewk->wu.routine_no[0]++;
        get_message_conn_data(ewk, 0, ewk->master_player, mes_already);
        ewk->wu.disp_flag = 1;
        ewk->wu.vitality = 240;
        mes_timer = 55;
        ewk->wu.mvxy.a[0].sp = -0x100000;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 152;
        ewk->wu.mvxy.a[0].sp = -0x100000;
        ewk->wu.mvxy.d[0].sp = 0;
        break;

    case 1:
        if (Suicide[2] == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 3;
            break;
        }

        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.cg_number++;
        ewk->wu.cg_number &= 0x7FFF;
        sort_push_request3(&ewk->wu);
        break;

    case 2:
        if (Suicide[2] == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        sort_push_request3(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_B8_init(s8 WIN_PL_NO, s16 timer) {
    PLW* wk;
    WORK_Other_CONN* ewk;
    s16 ix;
    u16 mes_no;

    test_in = 0;
    wk = &plw[WIN_PL_NO];

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.routine_no[0] = 0;
    wk_set(ewk);
    mes_timer = timer;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->master_player = My_char[wk->wu.id];
    test_pl_no = My_char[wk->wu.id];

    if (effb8_normal_or_senyou()) {
        mes_no = My_char[wk->wu.id ^ 1] + 0;

        if (old_mes_no_pl == mes_no) {
            mes_no = effb8_sel_1_by_8();
        }

        old_mes_no_pl = mes_no;
    } else {
        mes_no = effb8_sel_1_by_8();
    }

    mes_already = mes_no;
    test_mes_no = mes_no;
    return 0;
}

u16 effb8_normal_or_senyou() {
    if (Country != 1) {
        return 0;
    }

    return random_16() & 1;
}

u16 effb8_sel_1_by_8() {
    u16 mes_no;

    mes_no = random_16() & 7;
    old_mes_no2 = old_mes_no2 & 7;
    old_mes_no3 = old_mes_no3 & 7;

    if (old_mes_no2 == mes_no) {
        mes_no = (mes_no + 1) & 7;
    }

    if (old_mes_no3 == mes_no) {
        mes_no = (mes_no + 1) & 7;

        if (old_mes_no2 == mes_no) {
            mes_no = (mes_no + 1) & 7;
        }
    }

    old_mes_no3 = old_mes_no2;
    old_mes_no2 = mes_no;
    mes_no = mes_no + 20;
    return mes_no;
}

void wk_set(WORK_Other_CONN* ewk) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = 118;
    ewk->wu.work_id = 16;
    ewk->wu.rl_flag = 0;
    ewk->wu.cgromtype = 1;
    ewk->wu.sync_suzi = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x37;
    ewk->wu.my_family = 1;
    ewk->wu.my_priority = 35;
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 168;
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 15;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 168;
    ewk->wu.position_z = 35;
}
