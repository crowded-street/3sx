/**
 * @file plmain.c
 * Player Character's Core Gameplay Logic
 */

#include "sf33rd/Source/Game/engine/plmain.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpca.h"
#include "sf33rd/Source/Game/engine/plpcu.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/plpnm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/meta_col.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sysdir.h"

void plmv_1010(PLW* wk);
void plmv_1020(PLW* wk, s16 step);
void mpg_union(PLW* wk);
void eag_union(PLW* wk);
void sag_union(PLW* wk);
void addSAAttribute(u8* kow, u16* koa);
void check_omop_vital(PLW* wk);
s16 select_hit_stop(s16 ms, s16 sb);

void Player_move(PLW* wk, u16 lv_data) {
    s16 i;

    if (wk->wu.operator) {
        wk->cp->sw_lvbt = lv_data;
    } else {
        wk->cp->sw_lvbt = processed_lvbt(cpu_algorithm(wk));
    }

    wk->cp->sw_lvbt = check_illegal_lever_data(wk->cp->sw_lvbt);

    if (wk->metamor_over) {
        wk->cp->sw_lvbt = 0;
    }

    if (wk->resurrection_resv) {
        wk->cp->sw_lvbt = 0;
    }

    if (wk->dead_flag) {
        wk->cp->sw_lvbt = 0;
    }

    if (wk->wkey_flag) {
        wk->cp->sw_lvbt = 0;
    }

    if ((wk->dead_flag + wk->wkey_flag) == 0) {
        wk->hurimukenai_flag = 0;
    }

    for (i = 0; i < 8; i++) {
        wk->wu.old_rno[i] = wk->wu.routine_no[i];
    }

    for (i = 0; i < 3; i++) {
        wk->wu.old_pos[i] = wk->wu.xyz[i].disp.pos;
    }

    get_saikinnno_idouryou(wk);
    wk->old_gdflag = wk->guard_flag;
    wk->wu.renew_attack = 0;
    wk->wu.vital_old = wk->wu.vital_new;

    if (wk->sa_stop_flag != 1) {
        waza_check(wk);
    } else {
        key_thru(wk);
    }

    wk->wu.cmwk[10] = wk->cp->lgp;
    wk->wu.cmwk[11] += wk->cp->lgp;
    wk->wu.cmwk[11] &= 0x7FFF;
    wk->wu.cmwk[12] = wk->cp->sw_new;
    wk->wu.cmwk[13] = wk->cp->sw_now;
    plmain_lv_00[wk->wu.routine_no[0]](wk);
}

u16 check_illegal_lever_data(u16 data) {
    u16 lever = data & 0xF;

    data = (data & ~0xF) | Correct_Lv_Data[lever];
    return data;
}

void player_mv_0000(PLW* wk) {
    s16 i;

    for (i = 0; i < 8; i++) {
        wk->old_pos_data[i] = 0;
    }

    setup_vitality(&wk->wu, (wk->player_number));
    set_player_shadow(wk);
    wk->bullet_hcnt = wk->bhcnt_timer = 0;
    wk->auto_guard = 0;
    wk->wu.hit_stop = wk->wu.dm_stop = 0;
    wk->wu.hit_quake = wk->wu.dm_quake = 0;
    wk->tsukamarenai_flag = 0;
    wk->zuru_timer = 0;
    wk->zuru_flag = false;
    wk->tsukami_f = wk->tsukamare_f = false;
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
    wk->resurrection_resv = 0;
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
    wk->omop_vital_timer = 40;

    if (wk->player_number == 18) {
        metamor_color_restore(wk->wu.id);
    }

    switch (wk->spmv_ng_flag2 & 0x60000) {
    case 0x40000:
        clear_super_arts_point(wk);
        spgauge_cont_init();
        break;

    case 0x20000:
        if (Round_num == 0) {
        case 0:
            demo_set_sa_full(wk->sa);
            spgauge_cont_demo_init();
        }

        break;
    }

    about_gauge_process(wk);
}

void player_mv_1000(PLW* wk) {
    switch (appear_type) {
    case 0:
        plmv_1010(wk);

        if (Combo_Demo_Flag == 0) {
            plmv_1020(wk, 0x58);
        } else {
            set_super_arts_status(wk->wu.id);
            demo_set_sa_full(wk->sa);
        }

        Appear_end++;
        break;

    case 3:
        plmv_1010(wk);
        plmv_1020(wk, 0x80);
        break;

    case 1:
    case 2:
        wk->wu.routine_no[0] = 2;
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 0;
        wk->wu.routine_no[3] = 0;

        if (Combo_Demo_Flag == 0) {
            wk->wu.disp_flag = 1;
        }

        appear_data_init_set(wk);
        break;
    }

    Player_normal(wk);
    about_gauge_process(wk);
}

void plmv_1010(PLW* wk) {
    wk->wu.routine_no[0] = 3;
    wk->wu.routine_no[1] = 0;
    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;

    if (Combo_Demo_Flag == 0) {
        wk->wu.disp_flag = 1;
    }
}

void plmv_1020(PLW* wk, s16 step) {
    if (wk->wu.id) {
        wk->wu.rl_flag = 0;
        wk->wu.xyz[0].disp.pos = step + get_center_position();
        wk->wu.xyz[1].disp.pos = 0;
    } else {
        wk->wu.rl_flag = 1;
        wk->wu.xyz[0].disp.pos = get_center_position() - step;
        wk->wu.xyz[1].disp.pos = 0;
    }

    about_gauge_process(wk);
}

void player_mv_2000(PLW* wk) {
    if (wk->wu.routine_no[2] == 1) {
        wk->wu.routine_no[0] = 3;

        if (Combo_Demo_Flag == 0) {
            wk->wu.disp_flag = 1;
        }

        wk->wu.cg_type = 0;
    }

    Player_normal(wk);
    about_gauge_process(wk);
}

void player_mv_3000(PLW* wk) {
    if (gouki_app) {
        gouki_appear(wk);
    } else {
        Player_normal(wk);
    }

    about_gauge_process(wk);
}

void player_mv_4000(PLW* wk) {
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

        if (Timer_Freeze == 0 && wk->wu.hit_stop == 0 && wk->zuru_timer > 0) {
            wk->zuru_timer -= 2;
        }

        if (wk->zuru_timer < 0) {
            wk->zuru_flag = true;
        } else {
            wk->zuru_flag = false;
        }

        check_omop_vital(wk);
    }

    if (Timer_Freeze == 0) {
        look_after_timers(wk);
    }

    about_gauge_process(wk);
}

s16 check_hit_stop(PLW* wk) {
    s16 num;
    WORK* emwk = (WORK*)wk->wu.target_adrs;

    num = 0;

    if ((wk->wu.dm_stop != 0) && (wk->wu.hit_stop != 0)) {
        if (wk->wu.routine_no[3]) {
            wk->wu.hit_stop = select_hit_stop(wk->wu.hit_stop, wk->wu.dm_stop);
            wk->wu.dm_stop = 0;
        } else {
            wk->wu.dm_stop = select_hit_stop(wk->wu.dm_stop, wk->wu.hit_stop);
            wk->wu.hit_stop = 0;
            return 0;
        }
    }

    if (wk->wu.hit_stop) {
        num = 1;

        if (wk->wu.hit_stop > 0) {
            wk->wu.hit_stop--;

            if (wk->sa_stop_flag == 2) {
                if (wk->just_sa_stop_timer == Game_timer) {
                    wk->wu.hit_stop++;
                }

                if (wk->wu.hit_stop <= wk->sa_stop_sai) {
                    wk->sa_stop_lvdir = wk->cp->sw_lvbt;
                    wk->sa_stop_flag = 1;
                }
            }
        } else {
            wk->wu.hit_stop++;
            char_move(&wk->wu);
        }

        if ((wk->wu.routine_no[3] == 0) && ((wk->wu.routine_no[1] == 1) || (wk->wu.routine_no[1] == 3)) &&
            (emwk->routine_no[1] != 1) && (emwk->routine_no[1] != 3)) {
            num = 0;
        }

        if ((wk->wu.hit_stop == 0) && (wk->hsjp_ok != 0)) {
            char_move_cmhs(wk);
        }
    }

    if (wk->sa_stop_flag) {
        Timer_Freeze = 1;
    }

    return num;
}

s16 select_hit_stop(s16 ms, s16 sb) {
    s16 maf = 1;

    if (sb < 0) {
        sb = -sb;
    }

    if (ms < 0) {
        ms = -ms;
        maf = -1;
    }

    if (ms < sb) {
        ms = sb;
    }

    return ms * maf;
}

void look_after_timers(PLW* wk) {
    if (wk->tsukamarenai_flag) {
        wk->tsukamarenai_flag--;
    }

    if (wk->cat_break_ok_timer) {
        wk->cat_break_ok_timer--;
    }

    if (wk->uot_cd_ok_flag) {
        wk->ukemi_ok_timer--;

        if (wk->ukemi_ok_timer <= 0) {
            wk->ukemi_ok_timer = 0;
            wk->uot_cd_ok_flag = 0;
            wk->ukemi_success = 0;
        } else if (check_ukemi_flag(wk)) {
            wk->ukemi_ok_timer = 0;
            wk->uot_cd_ok_flag = 0;
            wk->ukemi_success = 1;
        }
    }

    if (wk->bullet_hcnt) {
        if (--wk->bhcnt_timer <= 0) {
            wk->bullet_hcnt = 0;
        }
    }

    if (wk->py->now.quantity.h && (wk->wu.hit_stop == 0)) {
        wk->py->now.timer -= (wk->py->recover * stun_gauge_r_omake[omop_stun_gauge_rcv[wk->wu.id]]) / 32;

        if (wk->py->now.quantity.h <= 0) {
            wk->py->now.timer = 0;
        }
    }

    if (Debug_w[9]) {
        if (wk->sa->nmsa_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->nmsa_g_ix] = 9;
        }

        if (wk->sa->exsa_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->exsa_g_ix] = 9;
        }

        if (wk->sa->exs2_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->exs2_g_ix] = 9;
        }

        if (wk->sa->nmsa_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->nmsa_a_ix] = 9;
        }

        if (wk->sa->exsa_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->exsa_a_ix] = 9;
        }

        if (wk->sa->exs2_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->exs2_a_ix] = 9;
        }
    }
}

void about_gauge_process(PLW* wk) {
    eag_union(wk);
    sag_union(wk);
    mpg_union(wk);
    add_sp_arts_gauge_maxbit(wk);
}

void mpg_union(PLW* wk) {
    switch (wk->sa->mp_rno) {
    case 0:
        if (wk->sa->store == wk->sa->store_max) {
            wk->sa->mp_rno = 1;
            wk->sa->mp = 1;
        }

        wk->sa->saeff_mp = 0;
        break;

    case 1:
        if (wk->sa->store < wk->sa->store_max) {
            wk->sa->mp_rno = 0;
            wk->sa->mp = 0;
            break;
        }

        if (wk->sa->mp == -1) {
            wk->sa->mp_rno = 2;
            wk->sa->saeff_mp = 1;
        }
        break;

    case 2:
        switch (wk->sa->saeff_mp) {
        case -1:
            if (pcon_dp_flag == 0) {
                wk->sa->store = 0;
                wk->sa->gauge.i = 0;
            }

            sag_bug_fix(wk->wu.id);
            wk->sa->saeff_mp = 0;
            wk->sa->mp_rno = 0;
            wk->sa->mp = 0;
            sag_inc_timer[(wk->wu.id)] = 20;
            break;

        case 1:
            if (wk->wu.routine_no[1] != 4) {
            default:
                wk->sa->saeff_mp = 0;
                wk->sa->mp_rno = 0;
                wk->sa->mp = 0;
            }
        }

        break;

    default:
        wk->sa->mp_rno = 0;
        wk->sa->mp = 0;
        wk->sa->store = 0;
        wk->sa->gauge.i = 0;
        wk->sa->saeff_mp = 0;
        break;
    }
}

void eag_union(PLW* wk) {
    switch (wk->sa->ex_rno) {
    case 0:
        if (wk->player_number == 14 || wk->player_number == 0) {
            if (wk->sa->store != 0) {
                wk->sa->ex_rno = 1;
                wk->sa->ex = 1;
            }

            break;
        }

        if ((wk->sa->store != 0) || (wk->sa->gauge.s.h >= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]])) {
            wk->sa->ex_rno = 1;
            wk->sa->ex = 1;
            break;
        }

        break;

    case 1:
        if (wk->player_number == 14 || wk->player_number == 0) {
            if (wk->sa->store == 0) {
                wk->sa->ex_rno = 0;
                wk->sa->ex = 0;
                break;
            }
        } else if ((wk->sa->store == 0) && (wk->sa->gauge.s.h < use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]])) {
            wk->sa->ex_rno = 0;
            wk->sa->ex = 0;
            break;
        }

        if (wk->sa->ex == -1) {
            wk->sa->ex_rno = 2;
            sa_gauge_flash[wk->wu.id] |= 2;
        }

        break;

    case 2:
        if (pcon_dp_flag == 0) {
            if (wk->sa->gauge_type == 1 && wk->sa->store == wk->sa->store_max) {
                wk->sa->gauge.i = 0;
            }

            if (wk->sa->gauge.s.h >= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]]) {
                wk->sa->gauge.s.h -= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]];
            } else {
                wk->sa->store--;
                wk->sa->gauge.s.h += wk->sa->gauge_len - use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]];
            }
        }

        sag_bug_fix(wk->wu.id);
        wk->sa->ex_rno = 0;
        wk->sa->ex = 0;
        sag_inc_timer[wk->wu.id] = 20;
        break;

    default:
        wk->sa->ex_rno = 0;
        wk->sa->ex = 0;
        wk->sa->store = 0;
        wk->sa->gauge.i = 0;
        break;
    }
}

void sag_union(PLW* wk) {
    switch (wk->sa->sa_rno) {
    case 0:
        if (wk->sa->store) {
            wk->sa->sa_rno = 1;
            wk->sa->ok = 1;
            wk->sa->id_arts++;
        }

        wk->sa->saeff_ok = 0;
        break;

    case 1:
        if (wk->sa->store == 0) {
            wk->sa->sa_rno = 0;
            wk->sa->ok = 0;
            break;
        }

        if (wk->sa->ok == -1) {
            wk->sa->sa_rno = 2;
            wk->sa->sa_rno2 = 0;
            wk->sa->saeff_ok = 1;

            if (wk->sa->gt2 == 0) {
                wk->sa->bacckup_g_h = 0;
                break;
            }
        }

        break;

    case 2:
        switch (wk->sa->gt2) {
        case 0:
            switch (wk->sa->saeff_ok) {
            case -1:
                if (pcon_dp_flag == 0) {
                    if (wk->sa->ex4th_exec) {
                        wk->sa->store = 0;
                    } else {
                        wk->sa->store--;
                    }
                }

                sag_bug_fix(wk->wu.id);
                wk->sa->saeff_ok = 0;
                wk->sa->sa_rno = 0;
                wk->sa->ok = 0;
                sag_inc_timer[wk->wu.id] = 20;
                break;

            case 1:
                if (wk->wu.routine_no[1] != 4) {
                default:
                    wk->sa->saeff_ok = 0;
                    wk->sa->sa_rno = 0;
                    wk->sa->ok = 0;
                }
            }

            break;

        case 1:
            switch (wk->sa->sa_rno2) {
            case 0:
                switch (wk->sa->saeff_ok) {
                case -1:
                    if (pcon_dp_flag == 0) {
                        if (wk->sa->ex4th_exec) {
                            wk->sa->store = 0;
                        } else {
                            wk->sa->store--;
                        }
                    }

                    sag_bug_fix(wk->wu.id);

                    if (wk->sa->mp == 1) {
                        wk->sa->bacckup_g_h = 0;
                    } else {
                        wk->sa->bacckup_g_h = wk->sa->gauge.s.h;
                    }

                    wk->sa->gauge.s.h = wk->sa->gauge_len;
                    wk->sa->gauge.s.l = -1;
                    wk->sa->sa_rno2 = 1;
                    wk->sa->saeff_ok = 0;
                    break;

                case 1:
                    if (wk->wu.routine_no[1] != 4) {
                    default:
                        wk->sa->saeff_ok = 0;
                        wk->sa->sa_rno = 0;
                        wk->sa->ok = 0;
                        wk->sa->dtm_mul = 1;
                    }
                }

                break;

            case 1:
                if (Timer_Freeze != 0) {
                    break;
                }

                wk->sa->sa_rno2 = 2;
                /* fallthrough */

            case 2:
                if ((wk->sa_stop_flag != 1) && (((PLW*)wk->wu.target_adrs)->sa_stop_flag != 1)) {
                    wk->sa->gauge.i -= wk->sa->dtm * wk->sa->dtm_mul;
                }

                if (wk->sa->gauge.s.h <= 0 || Suicide[6] != 0) {
                    wk->sa->gauge.i = 0;
                    wk->sa->ok = 0;
                    wk->sa->sa_rno = 0;
                    wk->sa->dtm_mul = 1;
                    wk->sa->gauge.s.h = wk->sa->bacckup_g_h;
                    sag_inc_timer[wk->wu.id] = 20;
                    break;
                }

                if (My_char[wk->wu.id] == 3) {
                    addSAAttribute(&wk->wu.kind_of_waza, &wk->wu.at_koa);
                }

                if (My_char[wk->wu.id] == 10) {
                    wk->wu.kind_of_waza |= 32;
                    wk->wu.at_koa = 128;
                }

                if (My_char[wk->wu.id] == 16) {
                    wk->wu.kind_of_waza |= 32;
                    wk->wu.at_koa = 128;
                }

                if (My_char[wk->wu.id] == 18) {
                    wk->wu.kind_of_waza |= 32;
                    wk->wu.at_koa = 128;
                }

                if ((My_char[wk->wu.id] == 9) && (wk->sa->kind_of_arts == 2)) {
                    wk->wu.att.dipsw |= 0x10;
                }

                break;
            }

            break;

        case 3:
            switch (wk->sa->sa_rno2) {
            case 0:
                switch (wk->sa->saeff_ok) {
                case -1:
                    sag_bug_fix(wk->wu.id);
                    wk->sa->store--;
                    wk->sa->saeff_ok = 0;
                    wk->sa->sa_rno2 = 1;
                    break;

                case 1:
                    break;

                default:
                    wk->sa->saeff_ok = 0;
                    wk->sa->sa_rno = 0;
                    wk->sa->ok = 0;
                }

                break;

            default:
                break;
            }

            break;

        default:
            wk->sa->sa_rno = 0;
            wk->sa->ok = 0;
            wk->sa->store = 0;
            wk->sa->saeff_ok = 0;
            break;
        }

        break;
    }
}

void addSAAttribute(u8* kow, u16* koa) {
    switch (*kow & 0x78) {
    case 0:
    case 8:
        *kow = 0x20;
        *koa = 0x80;
        break;

    case 16:
    case 24:
        *kow = 0x28;
        *koa = 0x100;
        break;
    }
}

void demo_set_sa_full(SA_WORK* sa) {
    sa->sa_rno = 1;
    sa->ok = 1;
    sa->store = sa->store_max;
    sa->id_arts++;
    sa->gauge.s.h = 0;
    sa->gauge.s.l = 0;
    sa->dtm_mul = 1;
}

void get_saikinnno_idouryou(PLW* wk) {
    s16 i;

    for (i = 0; i < 7; i++) {
        wk->old_pos_data[i] = wk->old_pos_data[i + 1];
    }

    wk->old_pos_data[i] = wk->wu.xyz[0].disp.pos;
    wk->move_distance = wk->old_pos_data[7] - wk->old_pos_data[0];
    wk->move_power = cal_move_quantity2(wk->old_pos_data[0], 0, wk->old_pos_data[7], 0);
    wk->move_power >>= 3;
}

void clear_attack_num(WORK* wk) {
    s16 i;

    for (i = 0; i < 4; i++) {
        wk->uketa_att[i] = 0;
    }

    wk->attack_num = 0;
}

void clear_tk_flags(PLW* wk) {
    wk->tk_success = 0;
    wk->tk_dageki = 0;
    wk->tk_nage = 0;
    wk->tk_kizetsu = 0;
    wk->tk_konjyou = 0;
    wk->att_plus = 8;
    wk->def_plus = 8;
}

void (*const plmain_lv_00[5])(PLW* wk) = {
    player_mv_0000, player_mv_1000, player_mv_2000, player_mv_3000, player_mv_4000
};

void (*const plmain_lv_02[5])(PLW* wk) = { Player_normal, Player_damage, Player_catch, Player_caught, Player_attack };

const u8 plpnm_mvkind[59] = { 0, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3,
                              3, 2, 2, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const u8 plpdm_mvkind[32] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };

const u8 plpxx_kind[5] = { 0, 1, 0, 1, 0 };

void check_omop_vital(PLW* wk) {
    if (pcon_dp_flag) {
        return;
    }

    if (wk->dead_flag) {
        return;
    }

    if (sa_stop_check()) {
        return;
    }

    if (wk->resurrection_resv) {
        wk->wu.vital_new = -1;
        return;
    }

    switch (omop_vital_ix[wk->wu.id]) {
    case 0:
        if (vital_dec_timer) {
            break;
        }

        if ((wk->wu.routine_no[1] == 0) && !(plpnm_mvkind[wk->wu.routine_no[2]] & 1)) {
            break;
        }

        if ((wk->wu.routine_no[1] == 1) && !(plpdm_mvkind[wk->wu.routine_no[2]] & 1)) {
            break;
        }

        if (wk->wu.routine_no[1] == 3) {
            break;
        }

        if (wk->player_number == 0) {
            if ((wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 21)) {
                if (ca_check_flag == 0) {
                    ca_check_flag = 1;
                }

                break;
            }

            if ((wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 22) && (wk->wu.pat_status == 23)) {
                break;
            }
        }

        wk->wu.vital_new--;

        if (wk->wu.vital_new < 0) {
            wk->wu.vital_new = -1;
            wk->wu.dm_koa = 4;
            wk->dead_flag = 1;
            wk->guard_flag = 3;
            ca_check_flag = 0;
            break;
        }

        break;

    case 2:
        if (vital_inc_timer) {
            break;
        }

        if (wk->wu.routine_no[1] != 0) {
            break;
        }

        if (!(plpnm_mvkind[wk->wu.routine_no[2]] & 2)) {
            break;
        }

        wk->wu.vital_new++;

        if (wk->wu.vital_new > 160) {
            wk->wu.vital_new = 160;
            break;
        }

        break;

    case 3:
        if (plpxx_kind[wk->wu.routine_no[1]]) {
            break;
        }

        if (plpxx_kind[wk->wu.old_rno[1]]) {
            wk->omop_vital_timer = 40;
        }

        if (wk->omop_vital_timer) {
            wk->omop_vital_timer--;
            break;
        }

        /* fallthrough */

    case 4:
        wk->wu.vital_new++;

        if (wk->wu.vital_new > 160) {
            wk->wu.vital_new = 160;
        }

        break;
    }
}
