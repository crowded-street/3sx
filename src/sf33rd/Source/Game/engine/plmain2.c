/**
 * @file plmain2.c
 * Player Character's Core Gameplay Logic for Bonus Stages
 */

#include "sf33rd/Source/Game/engine/plmain2.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/bbbscom.h"
#include "sf33rd/Source/Game/engine/bbbscom2.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plpnm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/meta_col.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void player_mvbs_0000(PLW* wk);
void player_mvbs_1000(PLW* wk);
void plmv_b_1010(PLW* wk);
void plmv_b_1020(PLW* wk, s16 step);
void player_mvbs_2000(PLW* wk);
void player_mvbs_3000(PLW* wk);
void player_mvbs_4000(PLW* wk);

void (*const plmain_b_lv_00[5])(PLW* wk);

void Player_move_bonus(PLW* wk, u16 lv_data) {
    s16 i;

    if (wk->wu.operator) {
        if (wk->metamor_over) {
            wk->cp->sw_lvbt = 0;
        } else {
            wk->cp->sw_lvbt = lv_data;
        }
    } else {
        if (Bonus_Game_Flag == 21) {
            bbbs_com_execute(wk);
        } else {
            bbbs_com_execute2(wk);
        }

        wk->cp->sw_lvbt = 0;
    }

    if (wk->dead_flag) {
        wk->cp->sw_lvbt = 0;
    }

    if (wk->wkey_flag) {
        wk->cp->sw_lvbt = 0;
    }

    wk->cp->sw_lvbt = check_illegal_lever_data(wk->cp->sw_lvbt);

    if ((wk->dead_flag + wk->wkey_flag) == 0) {
        wk->hurimukenai_flag = 0;
    }

    for (i = 0; i < 8; i++) {
        wk->wu.old_rno[(i)] = wk->wu.routine_no[(i)];
    }

    for (i = 0; i < 3; i++) {
        wk->wu.old_pos[(i)] = wk->wu.xyz[(i)].disp.pos;
    }

    get_saikinnno_idouryou(wk);
    wk->old_gdflag = wk->guard_flag;
    wk->wu.renew_attack = 0;
    wk->wu.vital_old = wk->wu.vital_new;

    if (wk->sa_stop_flag != 1) {
        waza_check(wk);
    }

    wk->wu.cmwk[10] = wk->cp->lgp;
    wk->wu.cmwk[11] += wk->cp->lgp;
    wk->wu.cmwk[11] &= 0x7FFF;
    wk->wu.cmwk[12] = wk->cp->sw_new;
    wk->wu.cmwk[13] = wk->cp->sw_now;
    plmain_b_lv_00[wk->wu.routine_no[0]](wk);
}

void player_mvbs_0000(PLW* wk) {
    s16 i;

    for (i = 0; i < 8; i++) {
        wk->old_pos_data[i] = 0;
    }

    setup_vitality(&wk->wu, wk->player_number);
    set_player_shadow(wk);
    wk->bullet_hcnt = wk->bhcnt_timer = 0;
    wk->auto_guard = 1;
    wk->wu.hit_stop = wk->wu.dm_stop = 0;
    wk->wu.hit_quake = wk->wu.dm_quake = 0;
    wk->tsukamarenai_flag = 0;
    wk->zuru_timer = 0;
    wk->zuru_flag = 0;
    wk->tsukami_f = wk->tsukamare_f = 0;
    clear_kizetsu_point(wk);
    wk->ukemi_ok_timer = 0;
    wk->uot_cd_ok_flag = 0;
    wk->ukemi_success = 0;
    clear_my_shell_ix(&wk->wu);
    wk->sa->mp = 0;
    wk->sa->ok = 0;
    wk->sa->ex = 0;
    wk->sa->mp_rno = 0;
    wk->sa->mp_rno2 = 0;
    wk->sa->sa_rno = 0;
    wk->sa->sa_rno2 = 0;
    wk->sa->ex_rno = 0;
    wk->metamorphose = 0;
    wk->metamor_over = 0;
    wk->sa_healing = 0;
    demo_set_sa_full(wk->sa);
    wk->dm_hos_flag = 0;
    wk->kezurijini_flag = 0;
    wk->wu.floor = 0;
    wk->bs2_area_car = 0;
    wk->bs2_over_car = 0;
    wk->bs2_on_car = 0;
    wk->wu.extra_col = wk->wu.extra_col_2 = 0;
    wk->sa_stop_flag = 0;
    clear_tk_flags(wk);
    wk->wu.routine_no[0] = 1;
    wk->wu.routine_no[6] = 0;
    wk->wu.cmwk[0] = 0;
    about_gauge_process(wk);

    if (wk->player_number == 18) {
        metamor_color_restore(wk->wu.id);
    }
}

void player_mvbs_1000(PLW* wk) {
    switch (appear_type) {
    case 0:
        plmv_b_1010(wk);
        plmv_b_1020(wk, 96);
        Appear_end++;
        break;

    case 3:
        plmv_b_1010(wk);
        plmv_b_1020(wk, 128);
        break;

    case 1:
    case 2:
        wk->wu.routine_no[0] = 2;

        if (Bonus_Game_Flag != 20 || wk->wu.operator) {
            wk->wu.routine_no[1] = 0;
            wk->wu.routine_no[2] = 0;
            wk->wu.routine_no[3] = 0;
            wk->wu.disp_flag = 1;
        }

        appear_data_init_set(wk);
        break;
    }

    if ((wk->wu.operator == 0) && (Bonus_Game_Flag == 20)) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 51;
        wk->wu.routine_no[3] = 0;
        wk->wu.xyz[0].disp.pos = 468;
        wk->wu.xyz[1].disp.pos = 0;
    }

    Player_normal(wk);
}

void plmv_b_1010(PLW* wk) {
    wk->wu.routine_no[0] = 3;

    if (Bonus_Game_Flag != 20 || wk->wu.operator) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
        wk->wu.disp_flag = 1;
    }
}

void plmv_b_1020(PLW* wk, s16 step) {
    if (wk->wu.id) {
        wk->wu.rl_flag = 0;
        wk->wu.xyz[0].disp.pos = step + get_center_position();
        wk->wu.xyz[1].disp.pos = 0;
        return;
    }

    wk->wu.rl_flag = 1;
    wk->wu.xyz[0].disp.pos = get_center_position() - step;
    wk->wu.xyz[1].disp.pos = 0;
}

void player_mvbs_2000(PLW* wk) {
    if (Bonus_Game_Flag != 20 || wk->wu.operator) {
        if (wk->wu.routine_no[2] == 1) {
            wk->wu.routine_no[0] = 3;
            wk->wu.disp_flag = 1;
            wk->wu.cg_type = 0;
        }
    } else {
        wk->wu.routine_no[0] = 3;
        wk->wu.cg_type = 0;
    }

    Player_normal(wk);
}

void player_mvbs_3000(PLW* wk) {
    Player_normal(wk);
}

void player_mvbs_4000(PLW* wk) {
    wk->permited_koa = 0;
    check_extra_jump_timer(wk);

    if (wk->sa_stop_flag != 1) {
        check_lever_data(wk);
    }

    if (wk->tsukamare_f) {
        wk->wu.hit_stop = wk->wu.dm_stop = 0;
    }

    if (!check_hit_stop(wk)) {
        plmain_lv_02[wk->wu.routine_no[1]](wk);

        if ((Timer_Freeze == 0) && (wk->wu.hit_stop == 0) && (wk->zuru_timer > 0)) {
            wk->zuru_timer -= 2;
        }

        if (wk->zuru_timer < 0) {
            wk->zuru_flag = 1;
        } else {
            wk->zuru_flag = 0;
        }
    }

    if (Timer_Freeze == 0) {
        look_after_timers(wk);
    }

    about_gauge_process(wk);
}

void (*const plmain_b_lv_00[5])(PLW* wk) = {
    player_mvbs_0000, player_mvbs_1000, player_mvbs_2000, player_mvbs_3000, player_mvbs_4000
};
