/**
 * @file effd5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effd6.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se_data.h"

void effD5_main_process(WORK_Other* ewk);
void cal_speeds(WORK_Other* ewk, PLW* /* unused */, PLW* twk);
s32 my_rose_live_check(PLW* wk);

const s16 dm_sp_sel_tbl[4][2] = { { 0, 14 }, { 1, 16 }, { 2, 18 }, { 3, 20 } };

const s16 range_time_table[16] = { 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62 };

const s16 range_isp_table[16] = { 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5 };

void effect_D5_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.charset_id = 11;
        set_char_base_data(&ewk->wu);
        ewk->wu.my_col_code = ewk->wu.dm_vital;
        *ewk->wu.char_table = _plef_char_table;
        ewk->wu.type = 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.blink_timing = ewk->master_id;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 0;
        cal_speeds(ewk, (PLW*)ewk->my_master, (PLW*)ewk->wu.target_adrs);
        add_mvxy_speed(&ewk->wu);
        set_char_move_init(&ewk->wu, 0, 0x7C);
        sort_push_request(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (sa_stop_check() == 0) {
            if (ewk->wu.hit_stop < 0) {
                ewk->wu.hit_stop = -ewk->wu.hit_stop;
            }

            if (EXE_flag == 0 && Game_pause == 0) {
                effD5_main_process(ewk);

                if (ewk->wu.cg_type == 0xFF) {
                    ewk->wu.routine_no[0]++;
                    ewk->wu.disp_flag = 0;
                    break;
                }
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

void effD5_main_process(WORK_Other* ewk) {
    s16 dsst;

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
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < 0) {
            ewk->wu.xyz[1].disp.pos = 0;
            char_move_cmja(&ewk->wu);
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
        }

        break;

    case 1:
        if (ewk->wu.hf.hit.player) {
            if (ewk->wu.hf.hit.player & 0x33) {
                ewk->wu.routine_no[1] = 0;
                ewk->wu.mvxy.d[0].sp = 0;
                ewk->wu.mvxy.a[0].sp = 0;
                ewk->wu.mvxy.a[1].sp = 0;
                ewk->wu.direction = 2;

                if (ewk->wu.rl_flag) {
                    ewk->wu.direction = cal_attdir_flip(ewk->wu.direction);
                }

                setup_hana_extra(&ewk->wu, 0, 8);
            } else if (ewk->wu.hf.hit.player & 0xC0) {
                ewk->wu.routine_no[1] = 0;
                ewk->refrected = 1;
                ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
                ewk->wu.mvxy.a[0].sp = 0x60000;
                ewk->wu.mvxy.d[0].sp = -0x3000;
                ewk->wu.mvxy.a[1].sp /= 3;
                ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
                ewk->wu.hit_stop = 4;
                ewk->wu.direction = 0xD;

                if (ewk->wu.rl_flag) {
                    ewk->wu.direction = cal_attdir_flip(ewk->wu.direction);
                }

                setup_hana_extra(&ewk->wu, 1, 0x18);
            }
        } else {
            sound_effect_request[0x10B](ewk, 0x10B);
            ewk->wu.routine_no[1] = 2;
            ewk->wu.rl_flag = (ewk->wu.rl_flag + 1) & 1;
            ewk->wu.disp_flag = 2;
            ewk->wu.type = 0;
            ewk->wu.kage_flag = 0;
            ewk->wu.dir_timer = 16;
            ewk->wu.hit_stop = 2;
            ewk->wu.direction = ewk->wu.dm_dir;
            dsst = 3;

            if (!(ewk->wu.dm_kind_of_waza & 0xF8)) {
                dsst = (ewk->wu.dm_kind_of_waza / 2) & 3;
            }

            setup_hana_extra(&ewk->wu, dm_sp_sel_tbl[dsst][0], dm_sp_sel_tbl[dsst][1]);
        }

        ewk->wu.hf.hit_flag = 0;
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

void cal_speeds(WORK_Other* ewk, PLW* /* unused */, PLW* twk) {
    s16 tx = twk->wu.position_x;
    s16 rix = 0;

    if (ewk->wu.rl_flag) {
        tx -= 16;

        if (tx > ewk->wu.position_x) {
            rix = (tx - ewk->wu.position_x) / 32;
        } else {
            tx = ewk->wu.position_x + 16;
        }
    } else {
        tx += 16;

        if (tx < ewk->wu.position_x) {
            rix = (ewk->wu.position_x - tx) / 32;
        } else {
            tx = ewk->wu.position_x - 16;
        }
    }

    ewk->wu.mvxy.a[0].sp = 0;
    ewk->wu.mvxy.a[1].real.h = range_isp_table[rix];
    cal_delta_speed(&ewk->wu, range_time_table[rix], tx, 0, 2, 1);
    ewk->wu.mvxy.kop[0] = 1;

    if (ewk->wu.rl_flag == 0) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
    }
}

s32 effect_D5_init(WORK* wk, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if (my_rose_live_check((PLW*)wk) != 0) {
        return -1;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 135;
    ewk->wu.work_id = 2;
    ewk->wu.my_mts = 14;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.dm_vital = wk->my_col_code + 6;
    ewk->my_master = (u32*)wk;
    ewk->wu.target_adrs = wk->target_adrs;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;

    if (ewk->wu.rl_flag) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = wk->position_x + 28;
    } else {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = wk->position_x - 28;
    }

    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = wk->position_y + 124;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = 28;
    wk->cmwk[0] = 1;
    return 0;
}

s32 my_rose_live_check(PLW* wk) {
    WORK_Other* twk;
    s16 ix;

    if (wk->player_number != My_char[wk->wu.id]) {
        return 1;
    }

    if ((ix = search_effect_index(3, 0, 0x87)) == -1) {
        return 0;
    }

    twk = (WORK_Other*)frw[ix];

    if (twk->master_id == wk->wu.id) {
        return 1;
    }

    if ((ix = search_effect_index(3, 1, 0x87)) == -1) {
        return 0;
    }

    twk = (WORK_Other*)frw[ix];

    if (twk->master_id == wk->wu.id) {
        return 1;
    }

    return 0;
}
