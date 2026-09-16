/**
 * @file plcnt2.c
 * Player Controller for Bonus Stages
 */

#include "sf33rd/Source/Game/engine/plcnt2.h"
#include "common.h"
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
#include "sf33rd/Source/Game/system/sys_sub.h"

#include <SDL3/SDL.h>

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

static s32 hosei_transfer_blocked(s16 i) {
    return (!plw[i].tsukami_f || plw[i].kind_of_catch != 1) && (plw[i].tsukamare_f | plw[i].dm_hos_flag) == 0;
}

static s32 round_is_in_play(void) {
    return pcon_rno[0] == 2 && pcon_rno[1] == 0 && pcon_rno[2] == 2;
}

/* The bonus stage runs either before it has started at all, or whenever the
 * game is neither paused nor in the test menu. */
static s32 bonus_players_should_run() {
    return ((pcon_rno[0] + pcon_rno[1]) == 0) || (!Game_pause && !EXE_flag);
}

/* Both players ask for their push-out the same way. */
static void request_hit_push(PLW* wk) {
    if (!wk->zuru_flag && !wk->zettai_muteki_flag) {
        hit_push_request(&wk->wu);
    }
}

s32 Player_control_bonus() {
    if (bonus_players_should_run()) {
        players_timer++;
        players_timer &= 0x7FFF;
        player_bonus_process[pcon_rno[0]]();
        check_body_touch();
        check_damage_hosei_bonus();
        set_quake(&plw[0]);
        set_quake(&plw[1]);

        request_hit_push(&plw[0]);
        request_hit_push(&plw[1]);

        add_next_position(plw);
        add_next_position(&plw[1]);
        check_cg_zoom();
    }

    if (Game_pause != 0x81) {
        store_player_after_image_data();
    }

    if (round_is_in_play()) {
        return 1;
    }

    return 0;
}

/* Both works and the round-level flags go back to nothing. */
static void clear_bonus_players() {
    pcon_rno[1] = 2;
    SDL_zeroa(plw);
    setup_base_and_other_data();
    pcon_dp_flag = false;
    round_slow_flag = false;
    dead_voice_flag = false;
    another_bg[0] = another_bg[1] = 0;
    plw[0].scr_pos_set_flag = plw[1].scr_pos_set_flag = 1;
    clear_super_arts_point(&plw[0]);
    clear_super_arts_point(&plw[1]);

    if (Bonus_Game_Flag == 21) {
        setup_bs_scrrrl_bs();
    }
}

/* Both works have to be ready, and the battle allowed, before the stage runs. */
static void start_bonus_stage() {
    if (plw[0].wu.routine_no[0] != 3) {
        return;
    }

    if (plw[1].wu.routine_no[0] != 3) {
        return;
    }

    if (!Allow_a_battle_f) {
        return;
    }

    pcon_rno[0] = 1;
    pcon_rno[1] = 0;
    plw[0].wu.routine_no[0] = 4;
    plw[1].wu.routine_no[0] = 4;
    ca_check_flag = 1;
}

/* A human player keeps the parry counter it came in with; a CPU one starts at
 * nothing. */
static void setup_bonus_parry_counters() {
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
}

void plcnt_b_init() {
    switch (pcon_rno[1]) {
    case 0:
        clear_bonus_players();
        break;

    case 1:
        start_bonus_stage();
        break;

    case 2:
        setup_bonus_parry_counters();
        break;

    case 3:
        pcon_rno[1] = 1;
        pli_0002();
        break;
    }

    move_player_work_bonus();
}

/* Both players hit each other on the same frame. A double KO stops the game
 * harder than a single one. */
static void settle_simultaneous_hit() {
    subtract_dm_vital_aiuchi(&plw[0]);
    subtract_dm_vital_aiuchi(&plw[1]);

    if ((plw[0].dead_flag != 0) && (plw[1].dead_flag != 0)) {
        plw[0].wu.hit_stop = plw[1].wu.hit_stop = 2;
        plw[0].wu.dm_stop = plw[1].wu.dm_stop = 0;
        plw[0].wu.hit_quake = plw[1].wu.hit_quake = 4;
        plw[0].wu.dm_quake = plw[1].wu.dm_quake = 0;
        return;
    }

    if ((plw[0].dead_flag != 0) || (plw[1].dead_flag != 0)) {
        plw[0].wu.hit_stop = plw[1].wu.hit_stop = 4;
        plw[0].wu.dm_stop = plw[1].wu.dm_stop = 0;
        plw[0].wu.hit_quake = plw[1].wu.hit_quake = 8;
        plw[0].wu.dm_quake = plw[1].wu.dm_quake = 0;
    }
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
        settle_simultaneous_hit();
    }

    if (Bonus_Stage_RNO[0] == 2) {
        pcon_rno[0] = 2;
    }
}

/* Both players have stopped moving in the bonus stage. */
static s32 both_bonus_players_settled() {
    return footwork_check_bns(0) && footwork_check_bns(1);
}

/* Both have reached the end of their end-of-stage routine. */
static s32 both_bonus_routines_finished() {
    return (plw[0].wu.routine_no[3] == 9) && (plw[1].wu.routine_no[3] == 9);
}

/* The stage is over: both players go into the same end routine. */
static void end_bonus_stage() {
    complete_victory_pause();
    plw[0].wu.routine_no[2] = 40;
    plw[1].wu.routine_no[2] = 40;
    plw[0].wu.routine_no[1] = plw[1].wu.routine_no[1] = 0;
    plw[0].wu.routine_no[3] = plw[1].wu.routine_no[3] = 0;
    plw[0].wu.cg_type = plw[1].wu.cg_type = 0;
    pcon_rno[2]++;
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
        if (both_bonus_players_settled()) {
            pcon_rno[2]++;
        }

        break;

    case 2:
        end_bonus_stage();
        break;

    case 3:
        if (both_bonus_routines_finished()) {
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

/* One player's bonus-stage step: move, then correct against the near field
 * edge and, if that moved them, the far one. Both bonus orderings do this to
 * each player; only the order differs. */
static void move_one_bonus_player(PLW* wk, s32 setting, s16 near_edge, s16 far_edge) {
    Player_move_bonus(wk, processed_lvbt(Convert_User_Setting(setting)));

    if (set_field_hosei_flag(wk, near_edge, 1) != 0) {
        set_field_hosei_flag(wk, far_edge, 0);
    }
}

void move_P1_move_P2_bonus(s16* field_work) {
    move_one_bonus_player(&plw[0], 0, field_work[0], field_work[1]);

    move_one_bonus_player(&plw[1], 1, field_work[2], field_work[3]);

    if (Bonus_Game_Flag == 20) {
        plw[1].wu.disp_flag = 0;
    }
}

void move_P2_move_P1_bonus(s16* field_work) {
    move_one_bonus_player(&plw[1], 1, field_work[2], field_work[3]);

    move_one_bonus_player(&plw[0], 0, field_work[0], field_work[1]);

    if (Bonus_Game_Flag == 20) {
        plw[0].wu.disp_flag = 0;
    }
}

/* One player's leftover correction is pushed into the other. The two arms of
 * check_damage_hosei_bonus that do this, and the two gotos that used to jump
 * into them, differ only in which way round it goes. */
static void transfer_hosei(s16 from, s16 to) {
    plw[to].wu.xyz[0].disp.pos += plw[from].hosei_amari;
    plw[to].muriyari_ugoku += plw[from].hosei_amari;
}

void check_damage_hosei_bonus() {
    plw[0].muriyari_ugoku = plw[0].hosei_amari;
    plw[1].muriyari_ugoku = plw[1].hosei_amari;

    switch ((plw[0].hosei_amari != 0) + ((plw[1].hosei_amari != 0) * 2)) {
    case 1:
        if (hosei_transfer_blocked(0)) {
            break;
        }

        transfer_hosei(0, 1);
        break;

    case 2:
        if (hosei_transfer_blocked(1)) {
            break;
        }

        transfer_hosei(1, 0);
        break;

    case 3:
        if (plw[0].hos_fi_flag == plw[1].hos_fi_flag) {
            if (plw[0].tsukamare_f) {
                transfer_hosei(0, 1);
                break;
            }

            if (plw[1].tsukamare_f) {
                transfer_hosei(1, 0);
                break;
            }
        }

        break;
    }

    plw[0].hosei_amari = plw[1].hosei_amari = 0;
}
