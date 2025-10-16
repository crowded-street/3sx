/**
 * @file plcnt2.c
 * Player Controller for Bonus Stages
 */

#include "sf33rd/Source/Game/engine/plcnt2.h"
#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain2.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void setup_bs_scrrrl_bs();
void setup_bs_scrrrl_bs2();
void move_P1_move_P2_bonus(s16* field_work);
void move_P2_move_P1_bonus(s16* field_work);

const s16 bsmr_range_table[3][2][2] = { { { 192, 192 }, { 192, 192 } },
                                        { { 64, 192 }, { 224, -136 } },
                                        { { -112, 224 }, { 216, 40 } } };

void plcnt_b_move();
void plcnt_b_die();

void (*const player_bonus_process[3])() = { plcnt_b_init, plcnt_b_move, plcnt_b_die };

s32 Player_control_bonus() {
    if (((pcon_rno[0] + pcon_rno[1]) == 0) || (!Game_pause && !EXE_flag)) {
        players_timer++;
        players_timer &= 0x7FFF;
        player_bonus_process[pcon_rno[0]]();
        check_body_touch();
        check_damage_hosei_bonus();
        set_quake(&plw[0]);
        set_quake(&plw[1]);

        if (!plw[0].zuru_flag && !plw[0].zettai_muteki_flag) {
            hit_push_request(&plw[0].wu);
        }

        if (!plw[1].zuru_flag && !plw[1].zettai_muteki_flag) {
            hit_push_request(&plw[1].wu);
        }

        add_next_position(plw);
        add_next_position(&plw[1]);
        check_cg_zoom();
    }

    if (Game_pause != 0x81) {
        store_player_after_image_data();
    }

    if (pcon_rno[0] == 2 && pcon_rno[1] == 0 && pcon_rno[2] == 2) {
        return 1;
    }

    return 0;
}

void plcnt_b_init() {
    switch (pcon_rno[1]) {
    case 0:
        pcon_rno[1] = 2;
        work_init_zero((s32*)&plw[0], sizeof(PLW));
        work_init_zero((s32*)&plw[1], sizeof(PLW));
        setup_base_and_other_data();
        pcon_dp_flag = 0;
        round_slow_flag = 0;
        dead_voice_flag = 0;
        another_bg[0] = another_bg[1] = 0;
        plw[0].scr_pos_set_flag = plw[1].scr_pos_set_flag = 1;
        clear_super_arts_point(&plw[0]);
        clear_super_arts_point(&plw[1]);

        if (Bonus_Game_Flag == 21) {
            setup_bs_scrrrl_bs();
        }

        break;

    case 1:
        if (plw[0].wu.routine_no[0] != 3) {
            break;
        }

        if (plw[1].wu.routine_no[0] != 3) {
            break;
        }

        if (!Allow_a_battle_f) {
            break;
        }

        pcon_rno[0] = 1;
        pcon_rno[1] = 0;
        plw[0].wu.routine_no[0] = 4;
        plw[1].wu.routine_no[0] = 4;
        ca_check_flag = 1;

        break;

    case 2:
        pcon_rno[1] = 3;

        if (plw[0].wu.operator) {
            paring_ctr_vs[0][0] = paring_ctr_ori[0];
        } else {
            paring_ctr_vs[0][0] = 0;
        }

        if (plw[1].wu.operator) {
            paring_ctr_vs[0][1] = paring_ctr_ori[1];
        } else {
            paring_ctr_vs[0][1] = 0;
        }

        break;

    case 3:
        pcon_rno[1] = 1;
        pli_0002();
        break;
    }

    move_player_work_bonus();
}

void plcnt_b_move() {
    if (No_Death) {
        plw[0].wu.dm_vital = plw[1].wu.dm_vital = 0;
    }

    if (Break_Into) {
        plw[0].wu.dm_vital = plw[1].wu.dm_vital = 0;
    }

    move_player_work_bonus();

    if (aiuchi_flag) {
        subtract_dm_vital_aiuchi(&plw[0]);
        subtract_dm_vital_aiuchi(&plw[1]);

        if ((plw[0].dead_flag != 0) && (plw[1].dead_flag != 0)) {
            plw[0].wu.hit_stop = plw[1].wu.hit_stop = 2;
            plw[0].wu.dm_stop = plw[1].wu.dm_stop = 0;
            plw[0].wu.hit_quake = plw[1].wu.hit_quake = 4;
            plw[0].wu.dm_quake = plw[1].wu.dm_quake = 0;
        } else if ((plw[0].dead_flag != 0) || (plw[1].dead_flag != 0)) {
            plw[0].wu.hit_stop = plw[1].wu.hit_stop = 4;
            plw[0].wu.dm_stop = plw[1].wu.dm_stop = 0;
            plw[0].wu.hit_quake = plw[1].wu.hit_quake = 8;
            plw[0].wu.dm_quake = plw[1].wu.dm_quake = 0;
        }
    }

    if (Bonus_Stage_RNO[0] == 2) {
        pcon_rno[0] = 2;
    }
}

void plcnt_b_die() {
    plw[0].wu.dm_vital = plw[1].wu.dm_vital = 0;

    switch (pcon_rno[2]) {
    case 0:
        plw[0].wkey_flag = plw[1].wkey_flag = 1;
        plw[0].image_setup_flag = plw[1].image_setup_flag = 0;
        pcon_rno[2]++;
        /* fallthrough */

    case 1:
        if (footwork_check_bns(0) && footwork_check_bns(1)) {
            pcon_rno[2]++;
        }

        break;

    case 2:
        complete_victory_pause();
        plw[0].wu.routine_no[2] = 40;
        plw[1].wu.routine_no[2] = 40;
        plw[0].wu.routine_no[1] = plw[1].wu.routine_no[1] = 0;
        plw[0].wu.routine_no[3] = plw[1].wu.routine_no[3] = 0;
        plw[0].wu.cg_type = plw[1].wu.cg_type = 0;
        pcon_rno[2]++;
        break;

    case 3:
        if ((plw[0].wu.routine_no[3] == 9) && (plw[1].wu.routine_no[3] == 9)) {
            pcon_rno[2]++;
        }

        break;
    }

    move_player_work_bonus();
}

s16 footwork_check_bns(s8 ix) {
    s16 rnum = 0;

    if ((Bonus_Game_Flag == 20) && plw[ix].wu.operator == 0) {
        return 1;
    }

    if ((plw[ix].wu.routine_no[1] == 0) && (plw[ix].wu.routine_no[2] == 1)) {
        rnum = 1;
    }

    return rnum;
}

void setup_bs_scrrrl_bs() {
    s16 scrc = 512;

    switch (plw[0].wu.operator + (plw[1].wu.operator * 2)) {
    case 1:
        bs_scrrrl[0][0] = scrc + bsmr_range_table[1][0][0];
        bs_scrrrl[0][1] = scrc - bsmr_range_table[1][0][1];
        bs_scrrrl[1][0] = scrc + bsmr_range_table[1][1][0];
        bs_scrrrl[1][1] = scrc - bsmr_range_table[1][1][1];
        break;

    case 2:
        bs_scrrrl[0][0] = scrc + bsmr_range_table[2][0][0];
        bs_scrrrl[0][1] = scrc - bsmr_range_table[2][0][1];
        bs_scrrrl[1][0] = scrc + bsmr_range_table[2][1][0];
        bs_scrrrl[1][1] = scrc - bsmr_range_table[2][1][1];
        break;

    default:
        bs_scrrrl[0][0] = scrc + bsmr_range_table[0][0][0];
        bs_scrrrl[0][1] = scrc - bsmr_range_table[0][0][1];
        bs_scrrrl[1][0] = scrc + bsmr_range_table[0][1][0];
        bs_scrrrl[1][1] = scrc - bsmr_range_table[0][1][1];
        break;
    }
}

void setup_bs_scrrrl_bs2() {
    s16 scrc = get_center_position();

    bs_scrrrl[0][0] = scrc + 192;
    bs_scrrrl[0][1] = scrc - 192;
    bs_scrrrl[1][0] = bs_scrrrl[0][0];
    bs_scrrrl[1][1] = bs_scrrrl[0][1];
}

void move_player_work_bonus() {
    ichikannkei = check_work_position(&plw->wu, &plw[1].wu);
    set_rl_waza(&plw[0]);
    set_rl_waza(&plw[1]);
    Timer_Freeze = 0;

    if (Bonus_Game_Flag == 20) {
        setup_bs_scrrrl_bs2();
    }

    if (plw->wu.operator) {
        move_P1_move_P2_bonus(*bs_scrrrl);
        return;
    }

    move_P2_move_P1_bonus(*bs_scrrrl);
}

void move_P1_move_P2_bonus(s16* field_work) {
    Player_move_bonus(&plw[0], processed_lvbt(Convert_User_Setting(0)));

    if (set_field_hosei_flag(&plw[0], field_work[0], 1) != 0) {
        set_field_hosei_flag(&plw[0], field_work[1], 0);
    }

    Player_move_bonus(&plw[1], processed_lvbt(Convert_User_Setting(1)));

    if (set_field_hosei_flag(&plw[1], field_work[2], 1) != 0) {
        set_field_hosei_flag(&plw[1], field_work[3], 0);
    }

    if (Bonus_Game_Flag == 20) {
        plw[1].wu.disp_flag = 0;
    }
}

void move_P2_move_P1_bonus(s16* field_work) {
    Player_move_bonus(&plw[1], processed_lvbt(Convert_User_Setting(1)));

    if (set_field_hosei_flag(&plw[1], field_work[2], 1) != 0) {
        set_field_hosei_flag(&plw[1], field_work[3], 0);
    }

    Player_move_bonus(&plw[0], processed_lvbt(Convert_User_Setting(0)));

    if (set_field_hosei_flag(&plw[0], field_work[0], 1) != 0) {
        set_field_hosei_flag(&plw[0], field_work[1], 0);
    }

    if (Bonus_Game_Flag == 20) {
        plw[0].wu.disp_flag = 0;
    }
}

void check_damage_hosei_bonus() {
    plw[0].muriyari_ugoku = plw[0].hosei_amari;
    plw[1].muriyari_ugoku = plw[1].hosei_amari;

    switch ((plw[0].hosei_amari != 0) + ((plw[1].hosei_amari != 0) * 2)) {
    case 1:
        if ((!plw[0].tsukami_f || plw[0].kind_of_catch != 1) && (plw[0].tsukamare_f | plw[0].dm_hos_flag) == 0) {
            break;
        }

    one:
        plw[1].wu.xyz[0].disp.pos += plw[0].hosei_amari;
        plw[1].muriyari_ugoku += plw[0].hosei_amari;
        break;

    case 2:
        if ((!plw[1].tsukami_f || plw[1].kind_of_catch != 1) && (plw[1].tsukamare_f | plw[1].dm_hos_flag) == 0) {
            break;
        }

    two:
        plw[0].wu.xyz[0].disp.pos += plw[1].hosei_amari;
        plw[0].muriyari_ugoku += plw[1].hosei_amari;
        break;

    case 3:
        if (plw[0].hos_fi_flag == plw[1].hos_fi_flag) {
            if (plw[0].tsukamare_f) {
                goto one;
            }

            if (plw[1].tsukamare_f) {
                goto two;
            }
        }

        break;
    }

    plw[0].hosei_amari = plw[1].hosei_amari = 0;
}
