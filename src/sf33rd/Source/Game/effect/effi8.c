/**
 * @file effi8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff03.h"
#include "sf33rd/Source/Game/effect/effd7.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sound/se_data.h"
#include "sf33rd/Source/Game/stage/bg.h"

const s16 effI8_hit_box[2][4] = { { -9, 17, -6, 12 }, { -4, 10, 114, 9 } };

const s16 bbbs_emtall[20] = { 84, 76, 68, 58, 76, 64, 92, 60, 56, 52, 58, 68, 68, 84, 68, 64, 58, 88, 58, 76 };

const u16 cbm_table[8][5] = { { 0x3FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFF00 }, { 0x1FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFF00 },
                              { 0x0FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFE00 }, { 0x01FF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFC00 },
                              { 0x0003, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800 }, { 0x0001, 0xFFFF, 0xFFFF, 0xFFFF, 0xF000 },
                              { 0x0000, 0x000F, 0xFFFF, 0xFFFF, 0x8000 }, { 0x0000, 0x0000, 0x0FFF, 0xFFFE, 0x0000 } };

void effI8_main_process(WORK_Other* ewk);
void cal_speeds_to_me_effI8(WORK_Other* ewk, PLW* mwk);
void cal_speeds_to_em_effI8(WORK_Other* ewk, PLW* twk);

void effect_I8_move(WORK_Other* ewk) {
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
        ewk->wu.shell_ix[3] = 24;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 0;
        cal_speeds_to_me_effI8(ewk, (PLW*)ewk->my_master);
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
                effI8_main_process(ewk);
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

void effI8_main_process(WORK_Other* ewk) {
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
                ewk->wu.shell_ix[2] += (random_16() & 7) - 3;

                if (ewk->wu.char_index != 0x8A) {
                    set_char_move_init(&ewk->wu, 0, 0x8A);
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
                ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
                ewk->wu.mvxy.a[1].sp = (ewk->wu.mvxy.a[1].sp * 3) / 5;
                ewk->wu.shell_ix[1] = 1;
                ewk->wu.hit_stop = 1;

                if (screen_range_check_effD7(&ewk->wu)) {
                    ewk->wu.disp_flag = 0;
                    ewk->wu.type = 0;
                    ewk->wu.routine_no[0] = 2;
                    break;
                }
            }

            if (ewk->wu.kage_flag != 0 && ewk->refrected == 0 && mwk->wu.routine_no[1] == 4 &&
                mwk->wu.routine_no[2] == 31 && mwk->wu.cg_type == 0x28 &&
                hit_check_subroutine(&ewk->wu, (WORK*)ewk->my_master, effI8_hit_box[0], effI8_hit_box[1])) {
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
                mwk->wu.cmwk[6] = 0;

                if (mwk->wu.rl_flag) {
                    ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos + 10;
                } else {
                    ewk->wu.xyz[0].disp.pos = mwk->wu.xyz[0].disp.pos - 10;
                }

                ewk->wu.xyz[1].disp.pos = mwk->wu.xyz[1].disp.pos + 136;
                Bonus_Game_Work--;
                ewk->wu.disp_flag = 1;
                ewk->wu.type = 1;
                set_char_move_init(&ewk->wu, 0, 0x89);
                cal_speeds_to_em_effI8(ewk, (PLW*)ewk->wu.target_adrs);
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
                Bonus_Game_result++;

                if (ewk->wu.hf.hit.player & 0x80) {
                    Additinal_Score_DM((WORK_Other*)ewk->wu.target_adrs, 8);
                } else {
                    Additinal_Score_DM((WORK_Other*)ewk->wu.target_adrs, 6);
                }

                set_char_move_init(&ewk->wu, 0, 0x8B);
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
            Additinal_Score_DM((WORK_Other*)ewk->wu.target_adrs, 6);
            Bonus_Game_ex_result++;
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

void cal_speeds_to_me_effI8(WORK_Other* ewk, PLW* mwk) {
    s16 tx = mwk->wu.xyz[0].disp.pos;
    s16 ty = cal_move_quantity3(&mwk->wu, ewk->wu.dir_timer) + 128;

    cal_speeds_effD7(ewk, ewk->wu.dir_timer, tx, ty, 5);
}

void cal_speeds_to_em_effI8(WORK_Other* ewk, PLW* twk) {
    s16 tx = twk->wu.position_x;
    s16 ty;

    if (twk->wu.rl_flag) {
        tx -= ewk->wu.next_x;
    } else {
        tx += ewk->wu.next_x;
    }

    ty = bbbs_emtall[twk->player_number];

    cal_speeds_effD7(ewk, ewk->wu.dir_step, tx, ty, ewk->wu.dir_old);
}

s32 check_ball_mizushibuki(s16 xx, s16 yy) {
    s16 ix;
    s16 iy;
    u16 ans;

    if (bg_w.stage == 10) {
        return 2;
    }

    if (Bonus_Game_Flag != 21) {
        return 0;
    }

    yy -= 6;

    if (xx >= 689 || xx <= 399) {
        return 0;
    }

    if ((yy >= -7) || (yy < -24)) {
        return 0;
    }

    if ((ix = (xx - 400) / 64) > 4) {
        ix = 4;
    }

    if ((iy = (yy + 24) / 2) > 7) {
        iy = 7;
    }

    ans = cbm_table[iy][ix];

    if ((ix = ((xx - 400) % 64) / 4) > 15) {
        ix = 15;
    }

    if (ans << ix & 0x8000) {
        return 1;
    }

    return 0;
}

s32 effect_I8_init(PLW* wk, s16 top, const s16* sptr) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0xBC;
    ewk->wu.work_id = 2;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.dm_vital = wk->wu.my_col_code;
    ewk->wu.my_mts = 0xE;
    ewk->wu.dir_timer = top;
    ewk->wu.dir_step = sptr[1];
    ewk->wu.dir_old = sptr[2];
    ewk->wu.next_x = sptr[3];
    ewk->my_master = (u32*)wk;
    ewk->wu.target_adrs = (u32*)wk->wu.target_adrs;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos = 0x1C;
    return 0;
}

void setup_effI8(PLW* wk, const BBBSTable* dadr) {
    s16 i;
    s16 ttime = 0;

    for (i = 0; i < dadr->kosuu; i++) {
        ttime += dadr->bbdat[i][0];
        effect_I8_init(wk, ttime, &dadr->bbdat[i][0]);
    }
}
