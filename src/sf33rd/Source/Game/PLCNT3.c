#include "sf33rd/Source/Game/PLCNT3.h"
#include "common.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/Manage.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLCNT2.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void plcnt_b2_move();
void plcnt_b2_die();

void (*const player_bonus2_process[3])() = { plcnt_b_init, plcnt_b2_move, plcnt_b2_die };

s32 Player_control_bonus2() {
    if (((pcon_rno[0] + pcon_rno[1]) == 0) || (!Game_pause && !EXE_flag)) {
        players_timer++;
        players_timer &= 0x7FFF;
        player_bonus2_process[pcon_rno[0]]();

        if (check_be_car_object()) {
            check_body_touch2();
            check_damage_hosei_bonus();
        }

        set_quake(&plw[0]);
        set_quake(&plw[1]);

        if (plw[0].zuru_flag == 0 && plw[0].zettai_muteki_flag == 0) {
            hit_push_request(&plw[0].wu);
        }

        if (plw[1].zuru_flag == 0 && plw[1].zettai_muteki_flag == 0) {
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

void plcnt_b2_move() {
    if (No_Death) {
        plw[0].wu.dm_vital = plw[1].wu.dm_vital = 0;
    }

    if (Break_Into) {
        plw[0].wu.dm_vital = plw[1].wu.dm_vital = 0;
    }

    move_player_work_bonus();

    if (Bonus_Stage_RNO[0] == 2) {
        Time_Stop = 1;
        pcon_rno[0] = 2;
        pcon_rno[1] = 0;
        pcon_rno[2] = 0;
    }

    if (Time_Over) {
        pcon_rno[0] = 2;
        pcon_rno[1] = 0;
        pcon_rno[2] = 0;
    }
}

void plcnt_b2_die() {
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

        if (plw[0].wu.operator) {
            plw[0].wu.routine_no[1] = 0;
            plw[0].wu.routine_no[2] = 40;
            plw[0].wu.routine_no[3] = 0;
        } else {
            plw[0].wu.routine_no[3] = 9;
        }

        if (plw[1].wu.operator) {
            plw[1].wu.routine_no[1] = 0;
            plw[1].wu.routine_no[2] = 40;
            plw[1].wu.routine_no[3] = 0;
        } else {
            plw[1].wu.routine_no[3] = 9;
        }

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
