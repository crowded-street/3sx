/**
 * @file eff13_tengu.c
 * Tengu projectile behavior for effect 13.
 */

#include "sf33rd/Source/Game/effect/eff13_tengu.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff00.h"
#include "sf33rd/Source/Game/effect/eff13_internal.h"
#include "sf33rd/Source/Game/effect/eff96.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effi9.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void set_tengu_init_pos(WORK* ewk, WORK* mwk) {
    s16 scp = get_center_position();

    if (mwk->rl_flag) {
        scp -= 320;
    } else {
        scp += 320;
    }

    ewk->xyz[0].disp.pos = scp;
    ewk->xyz[1].disp.pos = ewk->direction;
}

static void update_tengu_home_phase(WORK_Other* ewk, TAMA* twk, PLW* mwk) {
    ewk->wu.position_z = mwk->wu.position_z + ewk->wu.old_pos[2];

    if (mwk->sa->ok != -1 || ewk->wu.dir_old != mwk->sa->id_arts) {
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.cg_hit_ix = 0;
        make_speed_xy_back(&ewk->wu, &mwk->wu, twk);
        return;
    }

    if (check_tengu_attack(&ewk->wu, &mwk->wu, twk)) {
        return;
    }

    set_tengu_my_home(&ewk->wu, &mwk->wu);

    if (ewk->wu.dir_step > 8) {
        ewk->wu.routine_no[2] = 1;
        ewk->wu.dir_timer = 8;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.dir_timer, ewk->wu.dmcal_m, ewk->wu.dmcal_d, 2, 2 });
    }
}

static void update_tengu_attack_phase(WORK_Other* ewk, TAMA* twk, PLW* mwk) {
    add_mvxy_speed_no_use_rl(&ewk->wu);
    cal_mvxy_speed(&ewk->wu);

    if (check_tengu_attack(&ewk->wu, &mwk->wu, twk) == 0 && --ewk->wu.dir_timer < 0) {
        ewk->wu.routine_no[2] = 2;
    }
}

static void update_tengu_return_phase(WORK_Other* ewk, TAMA* twk, PLW* mwk) {
    add_mvxy_speed_no_use_rl(&ewk->wu);
    cal_mvxy_speed(&ewk->wu);

    if (--ewk->wu.dir_timer < 0) {
        ewk->wu.routine_no[2] = 4;
        ewk->wu.att_hit_ok = 0;
        ewk->wu.dir_timer = twk->hos_x;
        set_tengu_my_home(&ewk->wu, &mwk->wu);
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.dir_timer, ewk->wu.dmcal_m, ewk->wu.dmcal_d, 2, 2 });
    }
}

static void update_tengu_routine(WORK_Other* ewk, TAMA* twk, PLW* mwk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] = 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.dir_timer = twk->data00;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.d[1].sp = -0x7000;
        cal_initial_speed(
            &ewk->wu,
            ewk->wu.dir_timer,
            mwk->wu.xyz[0].disp.pos + ewk->wu.old_pos[0],
            mwk->wu.xyz[1].disp.pos + ewk->wu.old_pos[1]
        );
        break;

    case 1:
        update_tengu_attack_phase(ewk, twk, mwk);
        break;

    case 2:
        update_tengu_home_phase(ewk, twk, mwk);
        break;

    case 3:
        update_tengu_return_phase(ewk, twk, mwk);
        break;

    case 4:
        add_mvxy_speed_no_use_rl(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        ewk->wu.cg_hit_ix = 0;

        if (--ewk->wu.dir_timer < 0) {
            ewk->wu.routine_no[2] = 2;
        }

        break;

    case 5:
        set_tengu_my_home(&ewk->wu, &mwk->wu);
        ewk->wu.routine_no[2] = 4;
        ewk->wu.cg_hit_ix = 0;
        ewk->wu.dir_timer = twk->hos_y;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.d[1].sp = -0x4000;
        cal_initial_speed(&ewk->wu, ewk->wu.dir_timer, ewk->wu.dmcal_m, ewk->wu.dmcal_d);
        break;
    }
}

void kotp_02000(WORK_Other* ewk, TAMA* twk) {
    PLW* mwk = (PLW*)ewk->my_master;

    enter_kotp_hit_phase(ewk);

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        char_move(&ewk->wu);

        update_tengu_routine(ewk, twk, mwk);

        break;

    case 1:
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 5;
        ewk->wu.hf.hit_flag = 0;
        ewk->wu.cg_hit_ix = 0;
        set_hit_stop_hit_quake(&ewk->wu);
        break;

    case 2:
        add_mvxy_speed_no_use_rl(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (--ewk->wu.dir_timer < 0) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
        }

        ewk->wu.cg_hit_ix = 0;
        break;
    }
}
void set_tengu_my_home(WORK* ewk, WORK* mwk) {
    if (mwk->pat_status < 32) {
        ewk->dmcal_m = mwk->xyz[0].disp.pos + ewk->old_pos[0];
        ewk->dmcal_d = mwk->xyz[1].disp.pos + ewk->old_pos[1];
    } else {
        ewk->dmcal_m = mwk->xyz[0].disp.pos + ewk->scr_mv_x;
        ewk->dmcal_d = mwk->xyz[1].disp.pos + ewk->scr_mv_y;
    }

    ewk->dir_step = cal_move_quantity2(ewk->xyz[0].disp.pos, ewk->xyz[1].disp.pos, ewk->dmcal_m, ewk->dmcal_d);
}

s32 check_tengu_attack(WORK* ewk, WORK* mwk, TAMA* twk) {
    if (mwk->cg_ja.atix == 0) {
        return 0;
    }

    ewk->routine_no[2] = 3;
    ewk->att_hit_ok = 1;
    ewk->rl_flag = mwk->rl_flag;
    ewk->dir_timer = twk->life_time;
    grade_add_att_renew((WORK_Other*)ewk);

    if (mwk->xyz[1].disp.pos > 0) {
        make_speed_xy_att(ewk, mwk, 2, 0);
    } else {
        make_speed_xy_att(ewk, mwk, 0, 2);
    }

    return 1;
}

void make_speed_xy_att(WORK* ewk, WORK* mwk, u8 xsw, u8 ysw) {
    s16 ax;
    s16 ay;

    get_target_att_position(mwk, &ax, &ay);
    cal_all_speed_data(ewk, &(Motion_Target) { ewk->dir_timer, ax, ay, xsw, ysw });
}

void make_speed_xy_back(WORK* ewk, WORK* mwk, TAMA* twk) {
    s16 bx;
    s16 by;

    ewk->dmcal_m = ewk->xyz[0].disp.pos;
    ewk->dmcal_d = ewk->xyz[1].disp.pos;
    ewk->mvxy.d[0].sp = 0;
    ewk->mvxy.d[1].sp = -0x7000;
    ewk->dir_timer = twk->data01;
    set_tengu_init_pos(ewk, mwk);
    bx = ewk->xyz[0].disp.pos;
    by = ewk->xyz[1].disp.pos;
    ewk->xyz[0].disp.pos = ewk->dmcal_m;
    ewk->xyz[1].disp.pos = ewk->dmcal_d;
    cal_initial_speed(ewk, ewk->dir_timer, bx, by);
}
