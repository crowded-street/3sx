/**
 * @file effd7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARID.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff03.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effi8.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/sound/se_data.h"

void effD7_main_process(WORK_Other* ewk);
void cal_speeds_to_me(WORK_Other* ewk, PLW* mwk);
void cal_speeds_to_em(WORK_Other* ewk, PLW* twk);
s32 my_ball_live_check(PLW* wk);

const s16 effD7_hit_box[2][4] = { { -9, 17, -6, 12 }, { -4, 10, 114, 9 } };

void effect_D7_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.charset_id = 11;
        set_char_base_data(&ewk->wu);
        ewk->wu.my_col_code = ewk->wu.dm_vital;
        *ewk->wu.char_table = _plef_char_table;
        ball_init_position_effD7(ewk, (PLW*)ewk->my_master);
        ewk->wu.type = 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.blink_timing = ewk->master_id;
        ewk->wu.shell_ix[1] = 0;
        ewk->wu.shell_ix[2] = 8;
        ewk->wu.shell_ix[3] = 16;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 0;
        cal_speeds_to_me(ewk, (PLW*)ewk->my_master);
        set_char_move_init(&ewk->wu, 0, 0x75);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (sa_stop_check() == 0) {
            if (ewk->wu.hit_stop < 0) {
                ewk->wu.hit_stop = -ewk->wu.hit_stop;
            }

            if (EXE_flag == 0 && Game_pause == 0) {
                effD7_main_process(ewk);
            }

            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;

            if (ewk->wu.type) {
                hit_push_request(&ewk->wu);
            }
        }

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

void effD7_main_process(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    if (ewk->wu.shell_ix[1]) {
        ewk->wu.kage_flag = ewk->wu.shell_ix[1] = 0;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        switch (ewk->wu.routine_no[2]) {
        case 0:
            char_move(&ewk->wu);
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);

            if (ewk->wu.xyz[1].disp.pos < ewk->wu.shell_ix[2]) {
                ewk->wu.xyz[1].disp.pos = ewk->wu.shell_ix[2];
                ewk->wu.shell_ix[2] -= ewk->wu.shell_ix[3];
                ewk->wu.shell_ix[2] -= random_16() & 3;

                if (ewk->wu.char_index != 0x77) {
                    set_char_move_init(&ewk->wu, 0, 0x77);
                }

                ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;

                switch (check_ball_mizushibuki(ewk->wu.xyz[0].disp.pos, ewk->wu.xyz[1].disp.pos)) {
                case 1:
                    effect_03_init(&ewk->wu, 0x84);
                    break;

                case 2:
                    effect_03_init(&ewk->wu, 0x85);
                    break;

                default:
                    sound_effect_request[0x157](ewk, 0x157);
                    break;
                }

                ewk->wu.mvxy.a[0].sp = (ewk->wu.mvxy.a[0].sp * 80) / 100;
                ewk->wu.mvxy.a[1].sp = -(ewk->wu.mvxy.a[1].sp / 2);
                ewk->wu.shell_ix[1] = 1;
                ewk->wu.hit_stop = 1;

                if (screen_range_check_effD7(&ewk->wu)) {
                    ewk->wu.disp_flag = 0;
                    ewk->wu.type = 0;
                    ewk->wu.routine_no[0] = 2;
                    break;
                }
            }

            if (ewk->wu.kage_flag && mwk->wu.routine_no[1] == 4 && mwk->wu.routine_no[2] == 30 &&
                mwk->wu.cg_type == 0x28 && mwk->tk_success == ewk->wu.shell_ix[0] &&
                hit_check_subroutine(&ewk->wu, (WORK*)ewk->my_master, effD7_hit_box[0], effD7_hit_box[1])) {
                mwk->wu.cmwk[7] = 1;
                ewk->wu.type = 0;
                ewk->wu.routine_no[2] = 1;
            }

            break;

        case 1:
            ewk->wu.disp_flag = 0;

            if (mwk->wu.routine_no[1] != 4) {
                ewk->wu.routine_no[2] = 0;
                ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos;
                ewk->wu.xyz[1].disp.pos = mwk->wu.xyz[1].disp.pos + 136;
                ewk->wu.disp_flag = 1;
                ewk->wu.type = 1;
                ewk->wu.mvxy.a[0].sp = 0x18000;
                ewk->wu.mvxy.d[0].sp = 0;
                ewk->wu.mvxy.a[1].sp = 0x20000;
                ewk->wu.mvxy.d[1].sp = -0x6800;
            } else if (mwk->wu.cmwk[6]) {
                ewk->wu.routine_no[2] = 0;

                if (mwk->wu.rl_flag) {
                    ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos + 10;
                } else {
                    ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos - 10;
                }

                ewk->wu.xyz[1].disp.pos = mwk->wu.xyz[1].disp.pos + 136;
                ewk->wu.disp_flag = 1;
                ewk->wu.type = 1;
                set_char_move_init(&ewk->wu, 0, 0x76);
                cal_speeds_to_em(ewk, (PLW*)ewk->wu.target_adrs);
                add_mvxy_speed(&ewk->wu);
                cal_mvxy_speed(&ewk->wu);
            }

            break;
        }

        break;

    case 1:
        if (ewk->wu.hf.hit.player) {
            if (ewk->wu.hf.hit.player & 0x33) {
                ewk->wu.routine_no[1] = 0;

                if (ewk->wu.hf.hit.player & 0x30) {
                    ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
                    ewk->wu.mvxy.a[0].sp /= 3;
                    ewk->wu.mvxy.a[1].sp = 0x10000;
                    ewk->wu.mvxy.d[1].sp = -0x6000;
                } else {
                    ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                    ewk->wu.mvxy.a[0].sp /= 2;
                    ewk->wu.mvxy.a[1].sp = 0;
                    ewk->wu.mvxy.d[1].sp = -0x6000;
                }
            } else if (ewk->wu.hf.hit.player & 0xC0) {
                ewk->wu.routine_no[1] = 0;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->wu.mvxy.a[0].sp = 0x30000;
                ewk->wu.mvxy.a[1].sp = 0x44000;
                ewk->wu.mvxy.d[1].sp = -0x5000;
                ewk->wu.hit_stop = 4;
            }
        } else if (ewk->wu.hf.hit.effect && ((WORK*)ewk->wu.hit_adrs)->id == 0x89) {
            sound_effect_request[0x157](ewk, 0x157);
            ewk->wu.routine_no[1] = 0;
            ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
            ewk->wu.mvxy.a[0].sp = (ewk->wu.mvxy.a[0].sp * 3) / 4;
            ewk->wu.hit_stop = 2;
        } else {
            if (ewk->wu.dmg_work_id != 1) {
                sound_effect_request[0x10B](ewk, 0x10B);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
            ewk->wu.disp_flag = 2;
            ewk->wu.type = 0;
            ewk->wu.kage_flag = 0;
            ewk->wu.dir_timer = 8;
            ewk->wu.hit_stop = 2;
        }

        ewk->wu.hit_work_id = ewk->wu.dmg_work_id = 0;
        ewk->wu.hf.hit_flag = 0;
        ewk->refrected = 1;
        break;

    case 2:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
        }

        break;
    }
}

void cal_speeds_to_me(WORK_Other* ewk, PLW* mwk) {
    s16 tx = mwk->wu.xyz[0].disp.pos;
    s16 ty = mwk->wu.xyz[1].disp.pos + 157;

    cal_speeds_effD7(ewk, 20, tx, ty, 6);
}

void cal_speeds_to_em(WORK_Other* ewk, PLW* twk) {
    s16 tx = twk->wu.position_x;
    s16 ty;

    if (ewk->wu.rl_flag) {
        if (ewk->wu.xyz[0].disp.pos > tx - 32) {
            tx = twk->wu.position_x + 32;
        }
    } else if (ewk->wu.xyz[0].disp.pos < tx - 32) {
        tx = twk->wu.position_x - 32;
    }

    ty = 48;
    cal_speeds_effD7(ewk, 40, tx, ty, 4);
}

void cal_speeds_effD7(WORK_Other* ewk, s16 tm, s16 tx, s16 ty, s16 ysp) {
    ewk->wu.mvxy.d[0].sp = 0;
    ewk->wu.mvxy.a[0].sp = 0;
    ewk->wu.mvxy.d[1].sp = 0;
    ewk->wu.mvxy.a[1].sp = 0;
    ewk->wu.mvxy.a[1].real.h = ysp;
    cal_delta_speed(&ewk->wu, tm, tx, ty, 0, 1);

    if (ewk->wu.rl_flag == 0) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
    }
}

void ball_init_position_effD7(WORK_Other* ewk, PLW* mwk) {
    s16 tx = get_center_position();

    if (mwk->wu.position_x - tx < 0) {
        ewk->wu.xyz[0].disp.pos = tx - 288;
    } else if (mwk->wu.position_x - tx == 0) {
        if (mwk->wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = tx + 288;
        } else {
            ewk->wu.xyz[0].disp.pos = tx - 288;
        }
    } else {
        ewk->wu.xyz[0].disp.pos = tx + 288;
    }

    ewk->wu.xyz[1].disp.pos = 128;
}

u8 screen_range_check_effD7(WORK* wk) {
    s16 scpx = get_center_position();
    s16 scpxr = scpx + 288;
    s16 scpxl = scpx - 288;

    scpx = wk->xyz[0].disp.pos;

    if (scpxl < 0) {
        scpxr -= scpxl;
        scpx -= scpxl;
        scpxl = 0;
    }

    if (scpx > scpxr || scpx < scpxl) {
        return 1;
    }

    if (wk->xyz[1].disp.pos < -40) {
        return 1;
    }

    return 0;
}

s32 effect_D7_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if (my_ball_live_check(wk) != 0) {
        return -1;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 137;
    ewk->wu.work_id = 2;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.dm_vital = wk->wu.my_col_code;
    ewk->wu.my_mts = 14;
    ewk->wu.shell_ix[0] = wk->tk_success;
    ewk->my_master = (u32*)wk;
    ewk->wu.target_adrs = wk->wu.target_adrs;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = 28;
    return 0;
}

s32 my_ball_live_check(PLW* wk) {
    WORK_Other* twk;
    s16 ix;

    if ((ix = search_effect_index(3, 0, 137)) == -1) {
        return 0;
    }

    twk = (WORK_Other*)frw[ix];

    if (twk->master_id == wk->wu.id) {
        return 1;
    }

    if ((ix = search_effect_index(3, 1, 137)) == -1) {
        return 0;
    }

    twk = (WORK_Other*)frw[ix];

    if (twk->master_id == wk->wu.id) {
        return 1;
    }

    return 0;
}
