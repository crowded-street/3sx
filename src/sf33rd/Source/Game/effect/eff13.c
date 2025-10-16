/**
 * @file eff13.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff13.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff00.h"
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
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void tama_display(WORK* wk);
void set_tengu_init_pos(WORK* ewk, WORK* mwk);
void set_tengu_my_home(WORK* ewk, WORK* mwk);
s32 check_tengu_attack(WORK* ewk, WORK* mwk, TAMA* twk);
void make_speed_xy_att(WORK* ewk, WORK* mwk, s16 tm, u8 xsw, u8 ysw);
void make_speed_xy_back(WORK* ewk, WORK* mwk, TAMA* twk);

const s16 kotp_07_dm_vital[4];
const TAMA tama_data[243];
const s16 tcct[36];
void (*const kind_of_tama_process[17])();
const s16 kage_tbl[6][4];
const s16 homing_empos_hos[1][20][2];
const s16 enemy_pos_hos[1][20][2];
const s16 X_F_L_A_T_pos_hos[1][20][2];

void effect_13_move(WORK_Other* ewk) {
    TAMA* tama = (TAMA*)ewk->wu.my_effadrs;
    PLW* mwk;
    PLW* emwk;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.my_mts = 14;
        ewk->wu.charset_id = 11;
        set_char_base_data(&ewk->wu);
        ewk->wu.hf.hit_flag = 0;
        ewk->wu.work_id = tama->my_wkid;
        ewk->wu.dir_timer = tama->life_time;
        ewk->wu.disp_flag = tama->disp_type;
        ewk->wu.blink_timing = ewk->master_id;
        ewk->wu.at_koa = tama->koa;
        ewk->wu.vital_new = tama->def_power;
        ewk->wu.dm_vital = 0;
        ewk->wu.original_vitality = tama->waza_num;
        ewk->wu.shell_vs_refrect = tama->vs_refrect;
        ewk->wu.charset_id = tama->kind_of_tama;

        if (ewk->master_id) {
            if (tama->col_2p == 0) {
                ewk->wu.my_col_code = ewk->wu.old_rno[7];
            } else {
                ewk->wu.my_col_code = tcct[tama->col_2p];
            }
        } else if (tama->col_1p == 0) {
            ewk->wu.my_col_code = ewk->wu.old_rno[7];
        } else {
            ewk->wu.my_col_code = tcct[tama->col_1p];
        }

        if (tama->kage_index) {
            ewk->wu.kage_flag = 1;
            ewk->wu.kage_hx = kage_tbl[tama->kage_index][0];
            ewk->wu.kage_hy = kage_tbl[tama->kage_index][1];
            ewk->wu.kage_prio = kage_tbl[tama->kage_index][2];
            ewk->wu.kage_char = kage_tbl[tama->kage_index][3];
        } else {
            ewk->wu.kage_flag = 0;
        }

        if (tama->kind_of_tama == 2) {
            set_tengu_init_pos(&ewk->wu, (WORK*)ewk->my_master);
            ewk->wu.disp_flag = 0;
            ewk->wu.dir_old = ((PLW*)ewk->my_master)->sa->id_arts;
            set_char_move_init2(&ewk->wu, 0, tama->chix, random_16() & 7, 0);
        } else if (tama->kind_of_tama == 0xF) {
            mwk = (PLW*)ewk->my_master;

            if (tama->data01) {
                ewk->wu.mvxy.a[0].sp = mwk->wu.mvxy.a[0].sp;
                ewk->wu.mvxy.a[1].sp = mwk->wu.mvxy.a[1].sp ? mwk->wu.mvxy.a[1].sp : -0x80000;
                ewk->wu.mvxy.d[0].sp = mwk->wu.mvxy.d[0].sp;
                ewk->wu.mvxy.d[1].sp = mwk->wu.mvxy.d[1].sp;
                ewk->wu.mvxy.kop[1] = 0;
            } else {
                emwk = (PLW*)mwk->wu.target_adrs;
                ewk->wu.xyz[0].disp.pos = tama->hos_x;
                ewk->wu.xyz[0].disp.pos += emwk->wu.xyz[0].disp.pos;
                ewk->wu.xyz[0].disp.pos += X_F_L_A_T_pos_hos[0][emwk->player_number][0];
                ewk->wu.xyz[1].disp.pos = tama->hos_y;
                ewk->wu.xyz[1].disp.pos += emwk->wu.xyz[1].disp.pos;
                ewk->wu.xyz[1].disp.pos += X_F_L_A_T_pos_hos[0][emwk->player_number][1];
                ewk->wu.rl_flag = ewk->wu.xyz[0].disp.pos > emwk->wu.xyz[0].disp.pos ? 0 : 1;
                setup_mvxy_data(&ewk->wu, tama->data00);
            }

            set_char_move_init(&ewk->wu, 0, tama->chix);
        } else {
            if (ewk->wu.rl_flag) {
                ewk->wu.xyz[0].disp.pos -= tama->hos_x;
            } else {
                ewk->wu.xyz[0].disp.pos += tama->hos_x;
            }

            ewk->wu.xyz[1].disp.pos += tama->hos_y;
            ewk->wu.position_z = ewk->wu.my_priority;

            if (tama->kind_of_tama == 7) {
                ewk->wu.position_z += 2;
            }

            setup_mvxy_data(&ewk->wu, tama->data00);
            set_char_move_init(&ewk->wu, 0, tama->chix);
        }

        if (tama->kind_of_tama == 11) {
            ewk->wu.next_z = 71;
            ewk->wu.my_mr_flag = 1;
            ewk->wu.my_mr.size.x = 127;
            ewk->wu.my_mr.size.y = 127;
        }

        if (tama->kind_of_tama == 10) {
            ewk->wu.rl_flag = ((WORK*)ewk->my_master)->rl_waza;
        }

        tama_display(&ewk->wu);
        effect_00_init(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[6] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (ewk->wu.hit_stop < 0) {
            ewk->wu.hit_stop = -ewk->wu.hit_stop;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            kind_of_tama_process[tama->kind_of_tama](ewk, tama);
        }

        tama_display(&ewk->wu);

        if (ewk->wu.floor) {
            ewk->wu.kind_of_waza |= 0x20;
            ewk->wu.at_koa = 0x80;
        }

        hit_push_request(&ewk->wu);
        break;

    case 2:
        erase_my_shell_ix((WORK*)ewk->my_master, ewk->wu.myself);
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void tama_display(WORK* wk) {
    set_quake((PLW*)wk);
    wk->position_x = wk->xyz[0].disp.pos + wk->next_x;
    wk->position_y = wk->xyz[1].disp.pos;
    sort_push_request(wk);
}

s32 screen_x_range_check(WORK* wk) {
    s16 scpx = get_center_position();
    s16 scpxr = scpx + 256;
    s16 scpxl = scpx - 256;

    scpx = wk->xyz[0].disp.pos;

    if (scpxl < 0) {
        scpxr -= scpxl;
        scpx -= scpxl;
        scpxl = 0;
    }

    if (scpx > scpxr || scpx < scpxl) {
        return 1;
    }

    return 0;
}

s32 screen_range_check(WORK* wk) {
    s16 scpx = get_center_position();
    s16 scpxr = scpx + 256;
    s16 scpxl = scpx - 256;
    s16 scpy;
    s16 scpyu;

    scpx = wk->xyz[0].disp.pos;

    if (scpxl < 0) {
        scpxr -= scpxl;
        scpx -= scpxl;
        scpxl = 0;
    }

    if (scpx > scpxr || scpx < scpxl) {
        return 1;
    }

    scpy = get_height_position();
    scpyu = scpy + 288;
    scpy = wk->xyz[1].disp.pos;

    if (scpy > scpyu) {
        return 1;
    }

    return 0;
}

s32 tama15_screen_check(WORK* wk) {
    s16 scpx = get_center_position();
    s16 scpxr = scpx + 512;
    s16 scpxl = scpx - 512;
    s16 scpy;
    s16 scpyu;
    s16 scpyd;

    scpx = wk->xyz[0].disp.pos;

    if (scpxl < 0) {
        scpxr -= scpxl;
        scpx -= scpxl;
        scpxl = 0;
    }

    if (scpx > scpxr || scpx < scpxl) {
        return 1;
    }

    scpy = get_height_position();
    scpyu = scpy + 512;
    scpyd = scpy - 288;
    scpy = wk->xyz[1].disp.pos;

    if (scpyd < 0) {
        scpyu -= scpyd;
        scpy -= scpyd;
        scpyd = 0;
    }

    if (scpy > scpyu || scpy < scpyd) {
        return 1;
    }

    return 0;
}

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

void kotp_00000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            if (ewk->wu.hit_stop == 1) {
                ewk->wu.hit_stop = 0;
                add_mvxy_speed_exp(&ewk->wu, 2);
            } else {
                ewk->wu.hit_stop--;
                break;
            }
        } else {
            add_mvxy_speed(&ewk->wu);
        }

        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && screen_range_check(&ewk->wu) == 0) {
            break;
        }

        ewk->wu.mvxy.a[0].sp /= 4;
        ewk->wu.mvxy.a[1].sp /= 4;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 0x100) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_01000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if (screen_range_check(&ewk->wu)) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
        }

        break;

    case 1:
        if (ewk->wu.hf.hit.player) {
            if (ewk->wu.hf.hit.player & 0xF0) {
                set_char_move_init(&ewk->wu, 0, twk->erdf);
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erht);
            }
        } else {
            set_char_move_init(&ewk->wu, 0, twk->erex);
        }

        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
        }

        break;
    }
}

void kotp_02000(WORK_Other* ewk, TAMA* twk) {
    PLW* mwk = (PLW*)ewk->my_master;

    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        char_move(&ewk->wu);

        switch (ewk->wu.routine_no[2]) {
        case 0:
            ewk->wu.routine_no[2] = 1;
            ewk->wu.disp_flag = 1;
            ewk->wu.dir_timer = twk->data00;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = -0x7000;
            cal_initial_speed(&ewk->wu,
                              ewk->wu.dir_timer,
                              mwk->wu.xyz[0].disp.pos + ewk->wu.old_pos[0],
                              mwk->wu.xyz[1].disp.pos + ewk->wu.old_pos[1]);
            break;

        case 1:
            add_mvxy_speed_no_use_rl(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);

            if (check_tengu_attack(&ewk->wu, &mwk->wu, twk) == 0 && --ewk->wu.dir_timer < 0) {
                ewk->wu.routine_no[2] = 2;
            }

            break;

        case 2:
            ewk->wu.position_z = mwk->wu.position_z + ewk->wu.old_pos[2];

            if (mwk->sa->ok != -1 || ewk->wu.dir_old != mwk->sa->id_arts) {
                ewk->wu.routine_no[1] = 2;
                ewk->wu.routine_no[2] = 0;
                ewk->wu.cg_hit_ix = 0;
                make_speed_xy_back(&ewk->wu, &mwk->wu, twk);
                break;
            }

            if (check_tengu_attack(&ewk->wu, &mwk->wu, twk)) {
                break;
            }

            set_tengu_my_home(&ewk->wu, &mwk->wu);

            if (ewk->wu.dir_step > 8) {
                ewk->wu.routine_no[2] = 1;
                ewk->wu.dir_timer = 8;
                cal_all_speed_data(&ewk->wu, ewk->wu.dir_timer, ewk->wu.dmcal_m, ewk->wu.dmcal_d, 2, 2);
            }

            break;

        case 3:
            add_mvxy_speed_no_use_rl(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);

            if (--ewk->wu.dir_timer < 0) {
                ewk->wu.routine_no[2] = 4;
                ewk->wu.att_hit_ok = 0;
                ewk->wu.dir_timer = twk->hos_x;
                set_tengu_my_home(&ewk->wu, &mwk->wu);
                cal_all_speed_data(&ewk->wu, ewk->wu.dir_timer, ewk->wu.dmcal_m, ewk->wu.dmcal_d, 2, 2);
            }

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
        make_speed_xy_att(ewk, mwk, ewk->dir_timer, 2, 0);
    } else {
        make_speed_xy_att(ewk, mwk, ewk->dir_timer, 0, 2);
    }

    return 1;
}

void make_speed_xy_att(WORK* ewk, WORK* mwk, s16 tm, u8 xsw, u8 ysw) {
    s16 ax;
    s16 ay;

    get_target_att_position(mwk, &ax, &ay);
    cal_all_speed_data(ewk, tm, ax, ay, xsw, ysw);
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

void kotp_03000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !screen_range_check(&ewk->wu)) {
            break;
        }

        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 2;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.hit_stop = 0;
        ewk->wu.hit_quake = 0;

        if (ewk->wu.vital_new < 0x100) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                    ewk->wu.routine_no[1] = 2;
                    ewk->wu.routine_no[2] = 1;
                    ewk->wu.mvxy.a[0].sp = 0;
                    ewk->wu.mvxy.a[1].sp = 0;
                    ewk->wu.mvxy.d[0].sp = 0;
                    ewk->wu.mvxy.d[1].sp = 0;
                    ewk->wu.hf.hit_flag = 0;
                    ewk->wu.kage_flag = 0;
                    break;
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            ewk->wu.disp_flag = 2;
            ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
            ewk->wu.mvxy.a[0].sp /= 3;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.hf.hit_flag = 0;
            ewk->wu.kage_flag = 0;
            break;
        }

        ewk->wu.routine_no[1] = 0;
        ewk->wu.hf.hit_flag = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_04000(WORK_Other* ewk, TAMA* /* unused */) {
    ewk->wu.disp_flag = 0;
    ewk->wu.routine_no[0] = 2;
}

void kotp_05000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.routine_no[3] == 0) {
            ewk->wu.routine_no[3]++;
            ewk->wu.xyz[1].disp.pos = get_height_position() + 256;
        }

        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && screen_x_range_check(&ewk->wu) == 0) {
            break;
        }

        ewk->wu.mvxy.a[0].sp /= 4;
        ewk->wu.mvxy.a[1].sp /= 4;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_06000(WORK_Other* ewk, TAMA* twk) {
    PLW* mwk;
    PLW* emwk;
    s16 dir;
    s16 emdir;
    s16* target_x;
    s16* target_y;

    mwk = (PLW*)ewk->my_master;
    emwk = (PLW*)mwk->wu.target_adrs;
    target_x = &ewk->wu.E3_work_index;
    target_y = &ewk->wu.E4_work_index;

    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        switch (ewk->wu.routine_no[3]) {
        case 0:
            ewk->wu.routine_no[3]++;
            ewk->wu.kezurare_flag = 6;
            ewk->wu.direction = ewk->wu.rl_flag ? twk->data01 : 256 - twk->data01 & 0xFF;
            effect_I9_init(ewk, 2, 3, 0x77);
            break;

        case 1:
            if (ewk->wu.kezurare_flag--) {
                break;
            }

            ewk->wu.routine_no[3]++;
            ewk->wu.kezurare_flag = 0x70;
            /* fallthrough */

        case 2:
            *target_x = homing_empos_hos[0][ewk->master_player][0];
            *target_x = mwk->wu.rl_flag ? emwk->wu.xyz[0].disp.pos - *target_x : emwk->wu.xyz[0].disp.pos + *target_x;
            *target_y = homing_empos_hos[0][ewk->master_player][1] + emwk->wu.xyz[1].disp.pos;
            dir = ewk->wu.direction;
            emdir = caldir_pos_256(ewk->wu.xyz[0].disp.pos, ewk->wu.xyz[1].disp.pos, *target_x, *target_y);
            dir += (emdir - (dir - 0x80) & 0xFF) > 0x80 ? 4 : -4;
            dir = dir & 0xFF;
            ewk->wu.mvxy.a[0].sp = (rate_256_table[dir][0] * 480) / 256;
            ewk->wu.mvxy.a[0].sp *= ewk->wu.rl_flag ? 1 : -1;
            ewk->wu.mvxy.a[1].sp = (rate_256_table[dir][1] * 512) / 256;
            ewk->wu.direction = dir;

            if (ewk->wu.mvxy.a[0].sp < 0) {
                ewk->wu.routine_no[3]++;
            } else if (!ewk->wu.kezurare_flag--) {
                ewk->wu.routine_no[3]++;
            }

            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            return;
        }

        if (--ewk->wu.dir_timer < 0 || screen_range_check(&ewk->wu) != 0) {
            ewk->wu.mvxy.a[0].sp /= 4;
            ewk->wu.mvxy.a[1].sp /= 4;
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
        }

        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_07000(WORK_Other* ewk, TAMA* twk) {
    WORK* awk;
    s16 dsst;
    PLW* mwk;
    PLW* emwk;
    s16 tama_x;

    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (bg_w.stage == 20) {
            ewk->wu.vs_id = 7;
        }

        if (ewk->wu.cg_type == 20) {
            setup_mvxy_data(&ewk->wu, ewk->wu.mvxy.index);
            ewk->wu.mvxy.index++;
            ewk->wu.cg_type = 0;
            mwk = (PLW*)ewk->my_master;
            emwk = (PLW*)mwk->wu.target_adrs;
            tama_x = ewk->wu.xyz[0].disp.pos;

            if (tama_x > emwk->wu.xyz[0].disp.pos) {
                ewk->wu.mvxy.a[0].sp *= ewk->wu.rl_flag ? -1 : 1;
                ewk->wu.mvxy.d[0].sp *= ewk->wu.rl_flag ? -1 : 1;
            } else {
                ewk->wu.mvxy.a[0].sp *= ewk->wu.rl_flag ? 1 : -1;
                ewk->wu.mvxy.d[0].sp *= ewk->wu.rl_flag ? 1 : -1;
            }
        }

        if (--ewk->wu.dir_timer < 0) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
        }

        break;

    case 1:
        if (ewk->wu.hf.hit_flag == 0) {
            awk = (WORK*)ewk->wu.dmg_adrs;

            if (awk->work_id == 1) {
                dsst = 3;

                if (!(ewk->wu.dm_kind_of_waza & 0xF8)) {
                    dsst = (ewk->wu.dm_kind_of_waza / 2) & 3;
                }

                ewk->wu.dm_vital = kotp_07_dm_vital[dsst];
            } else {
                ewk->wu.dm_vital = kotp_07_dm_vital[2];
            }
        }

        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 0x100) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_08000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !screen_range_check(&ewk->wu)) {
            break;
        }

        ewk->wu.mvxy.a[0].sp /= 4;
        ewk->wu.mvxy.a[1].sp /= 4;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else if (ewk->dm_refrect) {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
                ewk->wu.routine_no[1] = 2;
                ewk->wu.routine_no[2] = 1;
                ewk->wu.kage_flag = 0;
                ewk->wu.hit_stop = 0;
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);
            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }
            break;
        }

        break;
    }
}

void kotp_09000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !screen_range_check(&ewk->wu)) {
            break;
        }

        ewk->wu.mvxy.a[0].sp /= 4;
        ewk->wu.mvxy.a[1].sp /= 4;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }

                ewk->wu.routine_no[1] = 2;
                ewk->wu.routine_no[2] = 1;
                ewk->wu.kage_flag = 0;
                ewk->wu.hit_stop = 0;
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag) + 1 & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_10000(WORK_Other* ewk, TAMA* twk) {
    char_move(&ewk->wu);

    if (ewk->wu.cg_type == 0xFF) {
        ewk->wu.routine_no[0] = 2;
        return;
    }
}

void kotp_11000(WORK_Other* ewk, TAMA* twk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
    case 1:
        if (mwk->sa_stop_flag == 1) {
            break;
        }

        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos <= 0) {
            ewk->wu.mvxy.a[1].sp = ewk->wu.mvxy.d[1].sp = ewk->wu.mvxy.kop[1] = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.xyz[1].disp.pos = 0;
            ewk->wu.routine_no[1] = 2;
        }

        break;

    case 2:
        if (mwk->sa_stop_flag == 1) {
            break;
        }

        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
        }

        break;
    }

    if (ewk->wu.position_z == ewk->wu.next_z) {
        ewk->wu.position_z = ewk->wu.my_priority;
        return;
    }

    ewk->wu.position_z = ewk->wu.next_z;
}

void kotp_12000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            if (ewk->wu.hit_stop == 1) {
                ewk->wu.hit_stop = 0;
                add_mvxy_speed_exp(&ewk->wu, 2);
            } else {
                ewk->wu.hit_stop--;
                break;
            }
        } else {
            add_mvxy_speed(&ewk->wu);
        }

        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 10) {
            add_to_mvxy_data(&ewk->wu, twk->data01);
            ewk->wu.cg_type = 0;
            break;
        }

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !screen_range_check(&ewk->wu)) {
            break;
        }

        ewk->wu.mvxy.a[0].sp /= 4;
        ewk->wu.mvxy.a[1].sp /= 4;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_13000(WORK_Other* ewk, TAMA* twk) {
    PLW* mwk;
    PLW* emwk;
    s16 ipos_x;

    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    mwk = (PLW*)ewk->my_master;

    if (mwk->wu.routine_no[1] != 4) {
        ewk->wu.routine_no[0] = 2;
        ewk->wu.disp_flag = 0;
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        if (!ewk->wu.routine_no[3]) {
            ewk->wu.xyz[1].disp.pos = 0;
            ewk->wu.routine_no[3]++;

            if (twk->data00) {
                mwk = (PLW*)ewk->my_master;
                emwk = (PLW*)mwk->wu.target_adrs;
                ipos_x = enemy_pos_hos[0][emwk->player_number][0];
                ewk->wu.xyz[0].disp.pos =
                    emwk->wu.rl_flag ? emwk->wu.xyz[0].disp.pos + ipos_x : emwk->wu.xyz[0].disp.pos - ipos_x;
            }
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if (screen_range_check(&ewk->wu)) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
            break;
        }

        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
            ewk->wu.att_hit_ok = 0;
        } else {
            ewk->wu.routine_no[1] = 0;
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        char_move(&ewk->wu);
        ewk->wu.att_hit_ok = 0;

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
        }

        break;
    }
}

void kotp_14000(WORK_Other* ewk, TAMA* /* unused */) {
    char_move(&ewk->wu);

    if (ewk->wu.hf.hit_flag) {
        ((WORK*)ewk->my_master)->hf.hit_flag = ewk->wu.hf.hit_flag;
        ewk->wu.hf.hit_flag = 0;
    }

    if (ewk->wu.cg_type == 0xFF) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 2;
    }
}

void kotp_15000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            ewk->wu.hit_stop--;
            break;
        }

        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !tama15_screen_check(&ewk->wu)) {
            break;
        }

        ewk->wu.mvxy.a[0].sp = 0;
        ewk->wu.mvxy.a[1].sp = 0;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.d[1].sp = 0;
        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
        } else {
            ewk->wu.routine_no[1] = 0;
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        pp_pulpara_hit((WORK*)ewk->my_master);
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

void kotp_16000(WORK_Other* ewk, TAMA* twk) {
    if (ewk->wu.hf.hit_flag) {
        ewk->wu.routine_no[1] = 1;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (ewk->wu.hit_stop) {
            if (ewk->wu.hit_stop == 1) {
                ewk->wu.hit_stop = 0;
            } else {
                ewk->wu.hit_stop--;
                break;
            }
        } else {
            add_mvxy_speed(&ewk->wu);

            if (ewk->wu.rl_flag) {
                ewk->wu.xyz[0].cal += 0x38000;
            } else {
                ewk->wu.xyz[0].cal += -0x38000;
            }
        }

        cal_mvxy_speed(&ewk->wu);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            set_char_move_init(&ewk->wu, 0, twk->ernm);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        if ((ewk->wu.xyz[1].disp.pos + ewk->wu.cg_jphos) <= 0) {
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            set_char_move_init(&ewk->wu, 0, twk->erex);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = -ewk->wu.cg_jphos;
            break;
        }

        if (--ewk->wu.dir_timer >= 0 && !screen_range_check(&ewk->wu)) {
            break;
        }

        set_char_move_init(&ewk->wu, 0, twk->ernm);
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        ewk->wu.vital_new -= ewk->wu.dm_vital;
        ewk->wu.dm_vital = 0;

        if (ewk->wu.vital_new < 256) {
            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    set_char_move_init(&ewk->wu, 0, twk->erdf);
                } else {
                    set_char_move_init(&ewk->wu, 0, twk->erht);
                }
            } else {
                set_char_move_init(&ewk->wu, 0, twk->erex);
            }

            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_flag = 0;
            ewk->wu.hit_stop = 0;
        } else {
            ewk->wu.routine_no[1] = 0;

            if (ewk->wu.hf.hit.player) {
                if (ewk->wu.hf.hit.player & 0xF0) {
                    effect_96_init(&ewk->wu, twk->erdf, ewk->wu.disp_flag, ewk->wu.hit_stop);
                } else {
                    effect_96_init(&ewk->wu, twk->erht, ewk->wu.disp_flag, ewk->wu.hit_stop);
                }
            } else {
                effect_96_init(&ewk->wu, twk->erex, ewk->wu.disp_flag, ewk->wu.hit_stop);
            }

            if (ewk->dm_refrect) {
                ewk->master_id = (ewk->master_id + 1) & 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->dm_refrect = 0;
            }
        }

        ewk->wu.hf.hit_flag = 0;
        ewk->wu.hit_quake = 0;
        break;

    case 2:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }

            break;
        }

        break;
    }
}

s32 effect_13_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    write_my_shell_ix(wk, ix);

    if (wk->work_id == 1 && wk->rl_flag == 1 && ((PLW*)wk)->player_number == 0) {
        data++;
    }

    ewk->wu.be_flag = 1;
    ewk->wu.id = 13;
    ewk->wu.type = data;
    ewk->wu.operator = wk->operator;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = wk->cgromtype;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.old_rno[7] = wk->my_col_code;
    ewk->wu.weight_level = wk->weight_level;
    ewk->wu.rl_waza = Round_num;
    ewk->my_master = (u32*)wk;

    if (wk->work_id == 1) {
        ewk->master_player = ((PLW*)wk)->player_number;
        ewk->master_id = wk->id;
        ewk->master_work_id = wk->work_id;
        ewk->wu.olc_work_ix[0] = ((PLW*)wk)->tk_dageki;
        ewk->wu.olc_work_ix[1] = ((PLW*)wk)->tk_nage;
        ewk->wu.olc_work_ix[2] = ((PLW*)wk)->tk_kizetsu;
        ewk->wu.olc_work_ix[3] = wk->routine_no[1];
    } else {
        ewk->master_player = ((WORK_Other*)wk)->master_player;
        ewk->master_id = ((WORK_Other*)wk)->master_id;
        ewk->master_work_id = ((WORK_Other*)wk)->master_work_id;
    }

    ewk->wu.xyz[0] = wk->xyz[0];
    ewk->wu.xyz[1] = wk->xyz[1];
    ewk->wu.my_effadrs = (u32*)&tama_data[data];

    if (wk->work_id == 1) {
        ewk->wu.floor = ((PLW*)wk)->metamorphose;
    }

    return 0;
}

const s16 kotp_07_dm_vital[4] = { 96, 240, 384, 384 };

const TAMA tama_data[243] = {
    { 4, 29, 0, 16, 0, 1, 0, 1, 2, 3, 4, 11, 12, 0, 0, 1, 384, 999, -64, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 0, 1, 2, 3, 4, 11, 12, 1, 0, 1, 384, 999, -64, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 0, 1, 2, 3, 4, 11, 12, 2, 0, 1, 384, 999, -64, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 3, 74, 75, 76, 77, 78, 17, 18, 3, 0, 1, 640, 999, -64, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 5, 1, 2, 3, 4, 13, 14, 4, 0, 1, 1280, 999, -64, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 5, 1, 2, 3, 4, 13, 14, 5, 0, 1, 1280, 999, -64, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 5, 1, 2, 3, 4, 13, 14, 6, 0, 1, 1280, 999, -64, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 5, 1, 2, 3, 4, 13, 14, 7, 0, 1, 1280, 999, -64, 66, 0, 0 },
    { 4, 0, 1, 128, 0, 1, 6, 1, 2, 3, 4, 15, 16, 8, 0, 1, 384, 999, -64, 66, 0, 0 },
    { 4, 0, 1, 128, 0, 1, 6, 1, 2, 3, 4, 15, 16, 9, 0, 1, 640, 999, -64, 66, 0, 0 },
    { 4, 0, 1, 128, 0, 1, 6, 1, 2, 3, 4, 15, 16, 10, 0, 1, 896, 999, -64, 66, 0, 0 },
    { 4, 0, 1, 128, 0, 1, 6, 1, 2, 3, 4, 15, 16, 11, 0, 1, 1152, 999, -64, 66, 0, 0 },
    { 4, 30, 0, 16, 0, 4, 27, 28, 29, 30, 31, 2, 3, 12, 0, 1, 384, 120, -104, 68, 0, 0 },
    { 4, 30, 0, 16, 0, 4, 37, 28, 29, 30, 31, 2, 3, 13, 0, 1, 384, 90, -90, 92, 0, 0 },
    { 4, 30, 0, 16, 0, 4, 38, 28, 29, 30, 31, 2, 3, 14, 0, 1, 384, 60, -70, 104, 0, 0 },
    { 4, 30, 0, 16, 6, 4, 39, 28, 29, 30, 31, 2, 3, 15, 64, 1, 640, 120, -104, 68, 0, 0 },
    { 4, 46, 0, 16, 0, 4, 40, 28, 29, 30, 31, 2, 3, 16, 0, 1, 384, 120, -80, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 4, 40, 28, 29, 30, 31, 2, 3, 17, 0, 1, 384, 120, -80, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 4, 40, 28, 29, 30, 31, 2, 3, 18, 0, 1, 384, 120, -80, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 4, 40, 28, 29, 30, 31, 2, 3, 19, 0, 1, 384, 240, -80, 48, 0, 0 },
    { 4, 0, 0, 16, 1, 0, 10, 11, 12, 13, 14, 6, 7, 59, 0, 1, 128, 0, -16, 0, 0, 0 },
    { 4, 0, 0, 16, 1, 0, 71, 11, 12, 13, 14, 6, 7, 59, 0, 1, 128, 0, -16, 0, 0, 0 },
    { 4, 0, 0, 16, 1, 0, 10, 11, 12, 13, 14, 6, 7, 59, 0, 1, 128, 0, -16, 0, 0, 0 },
    { 4, 0, 0, 16, 1, 0, 10, 11, 12, 13, 14, 6, 7, 59, 0, 1, 128, 0, -16, 0, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 15, 0, 0, 0, 0, 1, 1, 40, 50, 0, 256, 12, 14, 32, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 16, 0, 0, 0, 0, 1, 1, 36, 46, 0, 384, 14, 16, 36, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 17, 0, 0, 0, 0, 1, 1, 32, 42, 0, 384, 16, 16, 32, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 18, 0, 0, 0, 0, 1, 1, 28, 38, 0, 384, 18, 20, 34, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 19, 0, 0, 0, 0, 1, 1, 50, 60, 0, 256, 8, 8, 46, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 20, 0, 0, 0, 0, 1, 1, 52, 62, 0, 256, 10, 12, 40, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 21, 0, 0, 0, 0, 1, 1, 44, 54, 0, 256, 6, 10, 44, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 22, 0, 0, 0, 0, 1, 1, 35, 50, 0, 256, 14, 16, 26, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 23, 0, 0, 0, 0, 1, 1, 31, 46, 0, 384, 16, 18, 28, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 24, 0, 0, 0, 0, 1, 1, 45, 60, 0, 256, 10, 12, 38, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 25, 0, 0, 0, 0, 1, 1, 48, 62, 0, 256, 12, 14, 32, 0, 0 },
    { 2, 0, 0, 128, 2, 2, 26, 0, 0, 0, 0, 1, 1, 38, 54, 0, 256, 8, 10, 28, 0, 0 },
    { 4, 0, 0, 128, 0, 3, 32, 33, 34, 35, 36, 2, 3, 20, 0, 1, 1024, 999, -70, 40, 0, 0 },
    { 4, 0, 0, 128, 0, 3, 32, 33, 34, 35, 36, 2, 3, 21, 0, 1, 1024, 999, -70, 40, 0, 0 },
    { 4, 0, 0, 128, 0, 3, 32, 33, 34, 35, 36, 2, 3, 22, 0, 1, 1024, 999, -70, 40, 0, 0 },
    { 4, 0, 0, 128, 0, 3, 32, 33, 34, 35, 36, 2, 3, 23, 0, 1, 1024, 999, -70, 40, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 7, 1, 2, 3, 4, 19, 20, 24, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 7, 1, 2, 3, 4, 19, 20, 25, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 7, 1, 2, 3, 4, 19, 20, 26, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 79, 1, 2, 3, 4, 19, 20, 27, 0, 1, 640, 999, -56, 64, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 5, 1, 2, 3, 4, 0, 0, 4, 0, 2, 1024, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 5, 1, 2, 3, 4, 0, 0, 5, 0, 2, 1024, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 5, 1, 2, 3, 4, 0, 0, 6, 0, 2, 1024, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 5, 1, 2, 3, 4, 0, 0, 7, 0, 2, 1024, 999, -62, 66, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 44, 56, 49, 51, 52, 4, 5, 28, 0, 1, 128, 999, -30, 56, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 43, 55, 48, 51, 52, 4, 5, 29, 0, 1, 128, 999, -39, 54, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 42, 54, 47, 51, 52, 4, 5, 30, 0, 1, 128, 999, -30, 58, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 84, 54, 47, 51, 52, 4, 5, 31, 0, 1, 128, 999, 24, 56, 0, 0 },
    { 4, 0, 1, 128, 0, 1, 6, 1, 2, 3, 4, 15, 16, 32, 0, 1, 1280, 999, -72, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 8, 1, 2, 3, 4, 23, 24, 33, 0, 1, 640, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 8, 1, 2, 3, 4, 23, 24, 34, 0, 1, 640, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 8, 1, 2, 3, 4, 23, 24, 35, 0, 1, 640, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 9, 1, 8, 1, 2, 3, 4, 23, 24, 36, 0, 1, 640, 999, -62, 66, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 59, 56, 49, 51, 52, 4, 5, 37, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 56, 49, 51, 52, 4, 5, 38, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 59, 56, 49, 51, 52, 4, 5, 39, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 56, 49, 51, 52, 4, 5, 40, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 59, 56, 49, 51, 52, 4, 5, 41, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 56, 49, 51, 52, 4, 5, 42, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 59, 55, 48, 51, 52, 4, 5, 43, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 44, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 45, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 46, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 47, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 48, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 49, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 50, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 51, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 52, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 53, 0, 1, 512, 999, -46, 48, 0, 0 },
    { 4, 0, 0, 128, 3, 5, 58, 55, 48, 51, 52, 4, 5, 54, 0, 1, 512, 999, 10, 40, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 62, 64, 66, 68, 70, 0, 0, 55, 0, 1, 640, 999, -42, 76, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 61, 63, 65, 67, 69, 0, 0, 55, 0, 1, 640, 999, -42, 76, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 73, 64, 66, 68, 70, 0, 0, 56, 0, 1, 640, 999, -36, 92, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 72, 63, 65, 67, 69, 0, 0, 56, 0, 1, 640, 999, -36, 92, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 73, 64, 66, 68, 70, 0, 0, 57, 0, 1, 640, 999, -22, 102, 0, 0 },
    { 4, 30, 1, 16, 0, 1, 72, 63, 65, 67, 69, 0, 0, 57, 0, 1, 640, 999, -22, 102, 0, 0 },
    { 4, 0, 0, 4, 4, 6, 74, 74, 74, 74, 74, 8, 9, 60, 0, 1, 256, 999, -18, 140, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 85, 55, 48, 51, 52, 4, 5, 61, 0, 1, 128, 999, -64, 62, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 86, 87, 88, 89, 90, 10, 10, 62, 0, 1, 384, 999, -42, 76, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 91, 87, 88, 89, 90, 10, 10, 63, 0, 1, 384, 999, -36, 92, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 91, 87, 88, 89, 90, 10, 10, 64, 0, 1, 384, 999, -22, 102, 0, 0 },
    { 4, 30, 0, 16, 16, 1, 144, 87, 88, 89, 90, 10, 10, 65, 0, 1, 384, 999, -44, 76, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 66, 0, 1, 384, 999, -128, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 67, 0, 1, 384, 999, 0, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 68, 0, 1, 384, 999, -120, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 69, 0, 1, 384, 999, -8, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 70, 0, 1, 384, 999, -112, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 71, 0, 1, 384, 999, -16, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 72, 0, 1, 384, 999, -104, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 73, 0, 1, 384, 999, -24, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 74, 0, 1, 384, 999, -96, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 75, 0, 1, 384, 999, -32, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 76, 0, 1, 384, 999, -88, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 77, 0, 1, 384, 999, -40, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 78, 0, 1, 384, 999, -80, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 79, 0, 1, 384, 999, -48, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 80, 0, 1, 384, 999, -72, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 93, 95, 97, 99, 95, 0, 0, 81, 0, 1, 384, 999, -56, 768, 0, 0 },
    { 4, 0, 1, 128, 5, 1, 92, 94, 96, 98, 94, 0, 0, 82, 0, 1, 384, 999, -64, 768, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 102, 103, 104, 105, 106, 27, 27, 83, 0, 1, 1408, 999, -44, 76, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 107, 108, 109, 110, 111, 25, 26, 85, 0, 1, 1536, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 107, 108, 109, 110, 111, 25, 26, 86, 0, 1, 1536, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 107, 108, 109, 110, 111, 25, 26, 87, 0, 1, 1536, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 107, 108, 109, 110, 111, 25, 26, 88, 0, 1, 1536, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 16, 0, 0, 132, 133, 133, 133, 133, 0, 0, 0, 0, 1, 2048, 999, 0, 0, 0, 0 },
    { 4, 33, 0, 16, 8, 1, 117, 118, 119, 120, 121, 28, 29, 89, 0, 1, 256, 999, -56, 66, 0, 0 },
    { 4, 33, 0, 16, 8, 1, 145, 118, 119, 120, 121, 28, 29, 90, 0, 1, 512, 999, -56, 66, 0, 0 },
    { 4, 33, 0, 16, 8, 1, 146, 118, 119, 120, 121, 28, 29, 91, 0, 1, 768, 999, -56, 66, 0, 0 },
    { 4, 33, 0, 16, 8, 1, 146, 118, 119, 120, 121, 28, 29, 92, 0, 1, 768, 999, -56, 66, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 122, 123, 124, 125, 126, 25, 26, 93, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 122, 123, 124, 125, 126, 25, 26, 94, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 122, 123, 124, 125, 126, 25, 26, 95, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 122, 123, 124, 125, 126, 25, 26, 96, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 127, 128, 129, 130, 131, 25, 26, 97, 0, 1, 1536, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 127, 128, 129, 130, 131, 25, 26, 98, 0, 1, 1536, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 127, 128, 129, 130, 131, 25, 26, 99, 0, 1, 1536, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 127, 128, 129, 130, 131, 25, 26, 100, 0, 1, 1536, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 16, 6, 4, 39, 28, 29, 30, 31, 2, 3, 105, 32, 1, 640, 120, -70, 104, 0, 0 },
    { 4, 0, 0, 128, 7, 0, 112, 113, 114, 115, 116, 0, 0, 101, 0, 1, 1536, 240, -36, 1, 0, 0 },
    { 4, 0, 0, 128, 7, 0, 141, 113, 114, 115, 116, 0, 0, 102, 0, 1, 1536, 240, -36, 1, 0, 0 },
    { 4, 0, 0, 128, 7, 0, 142, 113, 114, 115, 116, 0, 0, 103, 0, 1, 1536, 240, -36, 1, 0, 0 },
    { 4, 0, 0, 128, 7, 0, 143, 113, 114, 115, 116, 0, 0, 104, 0, 1, 1536, 240, -36, 1, 0, 0 },
    { 4, 0, 0, 16, 0, 0, 134, 135, 135, 135, 135, 0, 0, 0, 0, 1, 2048, 999, 0, 0, 0, 0 },
    { 4, 31, 0, 16, 0, 1, 136, 137, 138, 139, 140, 25, 26, 106, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 0, 1, 136, 137, 138, 139, 140, 25, 26, 107, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 0, 1, 136, 137, 138, 139, 140, 25, 26, 108, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 0, 1, 136, 137, 138, 139, 140, 25, 26, 109, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 144, 87, 88, 89, 90, 10, 10, 110, 0, 1, 640, 999, -44, 76, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 84, 54, 47, 51, 52, 4, 5, 112, 0, 1, 128, 999, 24, 56, 0, 0 },
    { 2, 0, 0, 16, 3, 5, 84, 54, 47, 51, 52, 4, 5, 113, 0, 1, 128, 999, -64, 62, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 147, 108, 109, 110, 111, 25, 26, 85, 0, 1, 1792, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 147, 108, 109, 110, 111, 25, 26, 86, 0, 1, 1792, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 147, 108, 109, 110, 111, 25, 26, 87, 0, 1, 1792, 999, -56, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 147, 108, 109, 110, 111, 25, 26, 88, 0, 1, 1792, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 8, 1, 151, 118, 119, 120, 121, 28, 29, 89, 0, 1, 256, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 8, 1, 152, 118, 119, 120, 121, 28, 29, 90, 0, 1, 512, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 8, 1, 153, 118, 119, 120, 121, 28, 29, 91, 0, 1, 768, 999, -56, 66, 0, 0 },
    { 4, 31, 0, 16, 8, 1, 153, 118, 119, 120, 121, 28, 29, 92, 0, 1, 768, 999, -56, 66, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 148, 123, 124, 125, 126, 25, 26, 93, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 148, 123, 124, 125, 126, 25, 26, 94, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 148, 123, 124, 125, 126, 25, 26, 95, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 46, 0, 16, 0, 1, 148, 123, 124, 125, 126, 25, 26, 96, 0, 1, 384, 999, -72, 48, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 149, 128, 129, 130, 131, 25, 26, 97, 0, 1, 1792, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 149, 128, 129, 130, 131, 25, 26, 98, 0, 1, 1792, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 149, 128, 129, 130, 131, 25, 26, 99, 0, 1, 1792, 999, -70, 52, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 149, 128, 129, 130, 131, 25, 26, 100, 0, 1, 1792, 999, -70, 52, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 150, 137, 138, 139, 140, 25, 26, 106, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 150, 137, 138, 139, 140, 25, 26, 107, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 150, 137, 138, 139, 140, 25, 26, 108, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 150, 137, 138, 139, 140, 25, 26, 109, 0, 1, 384, 999, -56, 66, 0, 0 },
    { 4, 0, 1, 128, 10, 0, 154, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28784, 999, 0, 0, 0, 0 },
    { 4, 0, 1, 128, 10, 0, 154, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28784, 999, 0, 0, 0, 0 },
    { 4, 0, 1, 128, 10, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28784, 999, 0, 0, 0, 0 },
    { 4, 0, 1, 128, 10, 0, 155, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28784, 999, 0, 0, 0, 0 },
    { 4, 0, 1, 128, 11, 0, 156, 157, 157, 157, 157, 30, 31, 114, 0, 1, 28784, 999, -90, 59, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 115, 0, 1, 384, 999, -24, 66, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 116, 0, 1, 384, 999, -41, 81, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 117, 0, 1, 384, 999, -37, 45, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 118, 0, 1, 384, 999, -53, 58, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 119, 0, 1, 384, 999, -48, 76, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 120, 0, 1, 384, 999, -54, 56, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 166, 159, 160, 161, 162, 32, 33, 121, 0, 1, 384, 999, -48, 36, 0, 0 },
    { 4, 0, 0, 128, 0, 1, 158, 159, 160, 161, 162, 32, 33, 122, 0, 1, 384, 999, -35, 39, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -64, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -72, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -80, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -88, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -96, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -104, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -112, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -120, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -128, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -136, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -144, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -152, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -160, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -168, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -176, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -184, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -192, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -200, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -208, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -216, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -224, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -232, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -240, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -248, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -256, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 170, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -264, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 175, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -272, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 176, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -280, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 177, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -288, 0, 0, 0 },
    { 4, 0, 0, 128, 1, 0, 178, 171, 172, 173, 174, 0, 0, 0, 0, 1, 128, 999, -296, 0, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 158, 159, 160, 161, 162, 32, 33, 124, 0, 1, 384, 999, -32, 80, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 158, 159, 160, 161, 162, 32, 33, 125, 0, 1, 384, 999, -32, 80, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 158, 159, 160, 161, 162, 32, 33, 126, 0, 1, 384, 999, -32, 80, 0, 0 },
    { 4, 29, 0, 16, 0, 1, 167, 159, 160, 161, 162, 32, 33, 127, 0, 1, 384, 999, -32, 80, 0, 0 },
    { 4, 29, 0, 16, 12, 1, 163, 159, 160, 161, 162, 32, 33, 128, 129, 1, 384, 999, -32, 80, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 164, 159, 160, 161, 162, 32, 33, 130, 0, 1, 384, 999, -32, 30, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 164, 159, 160, 161, 162, 32, 33, 131, 0, 1, 384, 999, -32, 30, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 164, 159, 160, 161, 162, 32, 33, 132, 0, 1, 384, 999, -32, 30, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 168, 159, 160, 161, 162, 32, 33, 133, 0, 1, 384, 999, -32, 30, 0, 0 },
    { 4, 30, 0, 16, 12, 1, 165, 159, 160, 161, 162, 32, 33, 134, 135, 1, 384, 999, -32, 30, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 179, 180, 181, 182, 183, 34, 35, 136, 0, 1, 384, 35, -51, 60, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 179, 180, 181, 182, 183, 34, 35, 137, 0, 1, 384, 16, -51, 60, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 179, 180, 181, 182, 183, 34, 35, 138, 0, 1, 384, 6, -51, 60, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 184, 185, 186, 187, 188, 34, 35, 139, 0, 1, 384, 999, -51, 60, 0, 0 },
    { 4, 0, 0, 16, 13, 0, 189, 190, 191, 192, 193, 0, 0, 1, 0, 1, 640, 999, 0, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 194, 194, 194, 194, 194, 4, 5, 0, 0, 1, 128, 999, -72, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 197, 197, 197, 197, 197, 4, 5, 0, 0, 1, 128, 999, -106, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 200, 200, 200, 200, 200, 4, 5, 0, 0, 1, 128, 999, -148, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 194, 194, 194, 194, 194, 4, 5, 0, 0, 1, 128, 999, -116, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 197, 197, 197, 197, 197, 4, 5, 0, 0, 1, 128, 999, -150, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 200, 200, 200, 200, 200, 4, 5, 0, 0, 1, 128, 999, -192, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 194, 194, 194, 194, 194, 4, 5, 0, 0, 1, 128, 999, -160, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 197, 197, 197, 197, 197, 4, 5, 0, 0, 1, 128, 999, -194, 0, 0, 0 },
    { 4, 0, 1, 16, 14, 0, 200, 200, 200, 200, 200, 4, 5, 0, 0, 1, 128, 999, -236, 0, 0, 0 },
    { 4, 0, 1, 128, 10, 0, 203, 203, 203, 203, 203, 25, 26, 0, 0, 1, 0, 999, 0, 0, 0, 0 },
    { 2, 30, 0, 16, 13, 0, 204, 205, 206, 207, 208, 0, 0, 0, 0, 1, 384, 999, -96, 0, 0, 0 },
    { 2, 30, 0, 16, 13, 0, 209, 205, 206, 207, 208, 0, 0, 0, 0, 1, 384, 999, -176, 0, 0, 0 },
    { 2, 30, 0, 16, 13, 0, 210, 205, 206, 207, 208, 0, 0, 0, 0, 1, 384, 999, -256, 0, 0, 0 },
    { 4, 30, 0, 16, 1, 0, 204, 205, 206, 207, 208, 0, 0, 0, 0, 1, 128, 999, -48, 0, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 223, 213, 214, 215, 216, 0, 0, 151, 1, 1, 32639, 60, 0, 0, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 221, 213, 214, 215, 216, 0, 0, 149, 0, 1, 32639, 180, -85, -170, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 220, 213, 214, 215, 216, 0, 0, 148, 0, 1, 32639, 180, -170, -85, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 219, 213, 214, 215, 216, 0, 0, 147, 0, 1, 32639, 180, -255, 0, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 218, 213, 214, 215, 216, 0, 0, 146, 0, 1, 32639, 180, -170, 85, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 217, 213, 214, 215, 216, 0, 0, 145, 0, 1, 32639, 180, -85, 170, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 212, 213, 214, 215, 216, 0, 0, 144, 0, 1, 32639, 180, 0, 255, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 217, 213, 214, 215, 216, 0, 0, 145, 0, 1, 32639, 180, 85, 170, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 218, 213, 214, 215, 216, 0, 0, 146, 0, 1, 32639, 180, 170, 85, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 219, 213, 214, 215, 216, 0, 0, 147, 0, 1, 32639, 180, 255, 0, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 220, 213, 214, 215, 216, 0, 0, 148, 0, 1, 32639, 180, 170, -85, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 221, 213, 214, 215, 216, 0, 0, 149, 0, 1, 32639, 180, 85, -170, 0, 0 },
    { 4, 38, 0, 128, 15, 1, 222, 213, 214, 215, 216, 0, 0, 150, 0, 1, 32639, 180, 0, -255, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 86, 87, 88, 89, 90, 10, 10, 151, 0, 1, 384, 999, -42, 76, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 91, 87, 88, 89, 90, 10, 10, 152, 0, 1, 384, 999, -36, 92, 0, 0 },
    { 4, 30, 0, 16, 0, 1, 91, 87, 88, 89, 90, 10, 10, 153, 0, 1, 384, 999, -22, 102, 0, 0 }
};

const s16 tcct[36] = { 8192, 8192, 8192, 8208, 8192, 8192, 8192, 8208, 8192, 8208, 8192, 8196,
                       8212, 8196, 8212, 8196, 8212, 8196, 8212, 8196, 8212, 8196, 8212, 8196,
                       8212, 8196, 8212, 8192, 8196, 8212, 8198, 8214, 8196, 8212, 8196, 8212 };

void (*const kind_of_tama_process[17])() = { kotp_00000, kotp_01000, kotp_02000, kotp_03000, kotp_04000, kotp_05000,
                                             kotp_06000, kotp_07000, kotp_08000, kotp_09000, kotp_10000, kotp_11000,
                                             kotp_12000, kotp_13000, kotp_14000, kotp_15000, kotp_16000 };

const s16 kage_tbl[6][4] = { { 0, 0, 71, 0 }, { 0, 0, 71, 3 }, { 0, 0, 71, 4 },
                             { 0, 0, 71, 9 }, { 0, 0, 71, 1 }, { 0, 0, 71, 0 } };

const s16 homing_empos_hos[1][20][2] = {
    { { 12, 76 }, { 0, 72 }, { 6, 66 }, { 2, 52 },  { 2, 74 }, { 2, 66 }, { 4, 82 }, { 6, 60 },  { 6, 58 }, { 6, 48 },
      { 2, 52 },  { 6, 66 }, { 6, 66 }, { 12, 76 }, { 6, 66 }, { 6, 66 }, { 6, 66 }, { 12, 76 }, { 2, 66 }, { 6, 66 } }
};

const s16 enemy_pos_hos[1][20][2] = { { { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 },
                                        { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 },
                                        { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 }, { 32, 0 } } };

const s16 X_F_L_A_T_pos_hos[1][20][2] = { { { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 },
                                            { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 },
                                            { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 },
                                            { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 }, { 0, -16 } } };
