#include "sf33rd/Source/Game/PLPCA.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/PLS03.h"
#include "sf33rd/Source/Game/Pow_Pow.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/pulpul.h"

void check_nagenuke(PLW* wk, PLW* tk);
static s32 cat07_running_check(WORK* wk);
void catch_cg_type_check(PLW* wk);
void set_char_move_init_ca(PLW* wk, s16 koc, s16 index);

void (*const plpca_lv_00[9])();

void Player_catch(PLW* wk) {
    wk->wu.next_z = wk->wu.my_priority;
    wk->running_f = 0;
    wk->py->flag = 0;
    wk->guard_flag = 3;
    wk->guard_chuu = 0;
    wk->tsukami_f = 1;
    wk->tsukamare_f = 0;
    wk->scr_pos_set_flag = 1;
    wk->dm_hos_flag = 0;
    wk->ukemi_success = 0;
    wk->zuru_timer = 0;
    wk->zuru_ix_counter = 0;
    wk->sa_stop_flag = 0;
    wk->atemi_flag = 0;
    wk->caution_flag = 0;
    wk->sa->saeff_ok = 0;
    wk->sa->saeff_mp = 0;
    wk->ukemi_success = 0;
    wk->ukemi_ok_timer = 0;
    wk->uot_cd_ok_flag = 0;
    wk->cancel_timer = 0;
    wk->hazusenai_flag = 0;
    wk->cat_break_reserve = 0;
    wk->hsjp_ok = 0;
    wk->high_jump_flag = 0;
    wk->wu.swallow_no_effect = 0;
    check_em_tk_power_off(wk, (PLW*)wk->wu.target_adrs);

    if (wk->wu.routine_no[3] == 0) {
        pp_pulpara_catch(&wk->wu);
    }

    plpca_lv_00[wk->wu.routine_no[2]](wk);
    check_nagenuke(wk, (PLW*)wk->wu.hit_adrs);

    if (((WORK*)wk->wu.target_adrs)->routine_no[2] == 3) {
        return;
    }

    if (!wk->wu.cg_prio) {
        return;
    }

    wk->wu.next_z = ((WORK*)wk->wu.target_adrs)->my_priority;

    if (wk->wu.cg_prio == 1) {
        wk->wu.next_z++;
        return;
    }

    wk->wu.next_z -= 3;
}

void check_nagenuke(PLW* wk, PLW* tk) {
    if (tk->wu.work_id != 1) {
        return;
    }

    if (!tk->cat_break_reserve && tk->hazusenai_flag) {
        return;
    }

    if (!wk->cat_break_ok_timer) {
        return;
    }

    if (wk->wu.routine_no[1] != 2) {
        return;
    }

    if (!check_nagenuke_cmd(tk)) {
        return;
    }

    if (wk->wu.xyz[1].disp.pos > 8) {
        wk->wu.routine_no[2] = 50;
    } else {
        wk->wu.routine_no[2] = 48;
    }

    wk->wu.routine_no[1] = 0;
    wk->wu.routine_no[3] = 0;
    wk->wu.hit_stop = 0;
    wk->wu.dm_stop = 0;

    if (tk->wu.xyz[1].disp.pos > 8) {
        tk->wu.routine_no[2] = 49;
    } else {
        tk->wu.routine_no[2] = 47;
    }

    tk->wu.routine_no[1] = 0;
    tk->wu.routine_no[3] = 0;
    tk->wu.hit_stop = 1;
    tk->wu.dm_stop = 0;
}

void Catch_00000(PLW* /* unused */) {}

void Catch_01000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        break;

    case 1:
        char_move(&wk->wu);
        catch_cg_type_check(wk);
        break;
    }
}

void Catch_02000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index + ((WORK*)wk->wu.hit_adrs)->weight_level);
        break;

    case 1:
        char_move(&wk->wu);
        catch_cg_type_check(wk);
        break;
    }
}

void Catch_03000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 7) {
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
        }

        catch_cg_type_check(wk);
        break;
    }
}

void Catch_04000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->data_ix;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

        catch_cg_type_check(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
        }

        catch_cg_type_check(wk);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        catch_cg_type_check(wk);
        break;

    case 4:
        char_move(&wk->wu);
        catch_cg_type_check(wk);
        break;
    }
}

void Catch_05000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
    void add_to_mvxy_data(WORK * wk, u32 ix);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        break;

    case 1:
        char_move(&wk->wu);
        switch (wk->wu.cg_type) {
        case 1:
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;

        case 20:
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 22:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;
        }

        catch_cg_type_check(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        switch (wk->wu.cg_type) {
        case 1:
            wk->wu.routine_no[3] = 1;
            wk->wu.cg_type = 0;
            break;

        case 20:
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;
        }

        catch_cg_type_check(wk);
        break;

    case 3:
        char_move(&wk->wu);
        catch_cg_type_check(wk);
        break;
    }
}

void Catch_06000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dir_timer = 10;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        break;

    case 1:
        if (wk->wu.dir_timer > 0 && (--wk->wu.dir_timer == 0)) {
            pp_pulpara_shungokusatsu(&wk->wu);
        }

        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            nise_combo_work(wk, (PLW*)wk->wu.target_adrs, 14);
            wk->wu.cg_type = 0;
        }

        catch_cg_type_check(wk);
        break;
    }
}

void Catch_07000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
    void add_to_mvxy_data(WORK * wk, u32 ix);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->data_ix;
        wk->wu.dir_timer = 10;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            cat07_running_check(&wk->wu);
        }

        catch_cg_type_check(wk);
        break;

    case 2:
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        char_move(&wk->wu);

        if (cat07_running_check(&wk->wu) == 0) {
            catch_cg_type_check(wk);
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 6);

        if (--wk->wu.dir_timer <= 0) {
            wk->wu.routine_no[3] = 4;
        }

        break;

    case 4:
        jumping_union_process(&wk->wu, 6);

        if (((PLW*)wk->wu.target_adrs)->micchaku_flag) {
            char_move_z(&wk->wu);
            wk->wu.routine_no[3] = 5;
        }

        catch_cg_type_check(wk);
        break;

    case 5:
        jumping_union_process(&wk->wu, 6);
        break;

    case 6:
        char_move(&wk->wu);
        break;
    }
}

s32 cat07_running_check(WORK* wk) {
#if defined(TARGET_PS2)
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

    if (wk->xyz[0].disp.pos < (bg_w.bgw[1].l_limit2 - 64) || wk->xyz[0].disp.pos > (bg_w.bgw[1].r_limit2 + 64)) {
        char_move_cmja(wk);
        setup_mvxy_data(wk, wk->mvxy.index);
        wk->mvxy.index++;
        wk->routine_no[3] = 3;
        return 1;
    }

    return 0;
}

void Catch_08000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init_ca(PLW * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init_ca(wk, 2, wk->wu.char_index);
        break;

    case 1:
        char_move(&wk->wu);
        catch_cg_type_check(wk);

        if (wk->wu.cg_type == 50) {
            wk->wu.routine_no[1] = 4;
            wk->wu.routine_no[2] = 23;
            wk->wu.routine_no[3] = 1;
        }

        break;
    }
}

void subtract_cu_vital(PLW* wk) {
    if (wk->wu.dm_vital != 0) {
        if (wk->dead_flag == 0) {
            if (wk->wu.dm_vital) {
                Additinal_Score_DM((WORK_Other*)wk->wu.dmg_adrs, wk->wu.dm_ten_ix);
                add_sp_arts_gauge_hit_dm(wk);
            }

            if (omop_vital_ix[wk->wu.id] == 5) {
                wk->wu.dm_vital = 0;
            }

            wk->wu.vital_new -= wk->wu.dm_vital;

            if (wk->wu.dm_nodeathattack && wk->wu.vital_new < 0) {
                wk->wu.vital_new = 0;
            }

            if (wk->wu.vital_new < 0) {
                wk->wu.vital_new = -1;
                wk->dead_flag = 1;
                dead_voice_flag = 1;

                if (round_slow_flag == 0) {
                    set_conclusion_slow();
                    round_slow_flag = 1;
                }
            } else if (wk->py->flag == 0) {
                wk->py->now.quantity.h += wk->wu.dm_piyo;

                if (wk->py->now.quantity.h >= wk->py->genkai) {
                    wk->py->now.timer = 0;
                    wk->py->flag = 1;
                }
            }
        }

        pp_pulpara_remake_dm_all(&wk->wu);
    }

    if (Mode_Type == MODE_NORMAL_TRAINING && (Training_ID != wk->wu.id)) {
        Training_Damage_Set(wk->wu.dm_vital, wk->wu.dm_piyo, wk->wu.kezurare_flag);
    }

    wk->wu.dm_vital = 0;
    wk->wu.dm_piyo = 0;
}

void catch_cg_type_check(PLW* wk) {
#if defined(TARGET_PS2)
    s32 effect_02_init(WORK * wk, s8 dmgp, s8 mkst, s32 dmrl);
#endif

    PLW* emwk = (PLW*)wk->wu.hit_adrs;

    switch (wk->wu.cg_type) {
    case 2:
        wk->wu.cg_type = 0;
        setup_catch_atthit(&wk->wu, &emwk->wu);
        add_combo_work(wk, emwk);
        grade_add_clean_hits((WORK_Other*)wk);
        break;

    case 3:
        wk->wu.cg_type = 4;
        subtract_cu_vital(emwk);
        set_catch_hit_mark_pos(&wk->wu, &emwk->wu);
        effect_02_init(&wk->wu, 0, 1, wk->wu.rl_flag);
        pp_pulpara_remake_at_hit(wk);
        pp_pulpara_hit(&wk->wu);
        pp_pulpara_remake_dm_all(&emwk->wu);

        if (emwk->backup_ok_timer) {
            emwk->uot_cd_ok_flag = 1;
            emwk->ukemi_ok_timer = emwk->backup_ok_timer;
        } else {
            emwk->uot_cd_ok_flag = 0;
            emwk->ukemi_ok_timer = 0;
        }

        emwk->ukemi_success = 0;
        break;

    case 5:
        subtract_cu_vital(emwk);
        wk->wu.cg_type = 0;
        emwk->ukemi_ok_timer = 0;
        emwk->uot_cd_ok_flag = 0;
        emwk->ukemi_success = 0;
        break;

    case 6:
        wk->wu.cg_type = 0;
        wk->wu.rl_flag ^= 1;
        break;

    case 7:
        wk->wu.cg_type = 0;
        wk->wu.routine_no[1] = wk->wu.cmmd.koc;
        wk->wu.routine_no[2] = wk->wu.cmmd.ix;
        wk->wu.routine_no[3] = wk->wu.cmmd.pat;
        break;

    case 8:
        wk->wu.cg_type = 0;
        emwk->wu.routine_no[1] = wk->wu.cmyd.koc;
        emwk->wu.routine_no[2] = wk->wu.cmyd.ix;
        emwk->wu.routine_no[3] = wk->wu.cmyd.pat;

    case 9:
        grade_add_nml_nage(&wk->wu);
        wk->wu.cg_type = 0;
    }
}

void set_char_move_init_ca(PLW* wk, s16 koc, s16 index) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s32 koc, s32 index);
#endif

    set_char_move_init(&wk->wu, koc, index);
    wk->cat_break_ok_timer = wk->wu.cmyd.koc >> 8;
    wk->wu.cmyd.koc &= 0xFF;
}

void (*const plpca_lv_00[9])(PLW*) = { Catch_00000, Catch_01000, Catch_02000, Catch_03000, Catch_04000,
                                       Catch_05000, Catch_06000, Catch_07000, Catch_08000 };
