/**
 * @file plpdm.c
 * Player Damage and Hit Reaction Controller
 */

#include "sf33rd/Source/Game/engine/plpdm.h"
#include "arcade/arcade_balance.h"
#include "bin2obj/buttobi.h"
#include "bin2obj/etc.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effa7.h"
#include "sf33rd/Source/Game/effect/effd9.h"
#include "sf33rd/Source/Game/effect/effe2.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effg6.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpca.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>
#include "sf33rd/Source/Game/engine/plpdm_internal.h"

void setup_damage_process_flags(PLW* wk);
void Damage_00000(PLW* wk);
void Damage_01000(PLW* wk);
void Damage_04000(PLW* wk);
void Damage_07000(PLW* wk);
void Damage_12000(PLW* wk);
void Damage_14000(PLW* wk);
void Damage_16000(PLW* wk);
void Damage_17000(PLW* wk);
void Damage_18000(PLW* wk);
void Damage_19000(PLW* wk);
void Damage_20000(PLW* wk);
void Damage_21000(PLW* wk);
void Damage_23000(PLW* wk);
void Damage_24000(PLW* wk);
void Damage_25000(PLW* wk);
void Damage_26000(PLW* wk);
void Damage_27000(PLW* wk);
void Damage_28000(PLW* wk);
void Damage_29000(PLW* wk);
void Damage_30000(PLW* wk);
void Damage_31000(PLW* wk);
void first_flight_union(PLW* wk, s16 num, s16 dv);
void first_TtktV_union(PLW* wk, s16 num, s16 dv);
void buttobi_chakuchi_cg_type_check(PLW* wk);
void buttobi_add_y_check(PLW* wk);
void set_dm_hos_flag_sky(PLW* wk);
void get_sky_dm_timer(PLW* wk);
void get_damage_reaction_data(PLW* wk);
void damage_atemi_setup(PLW* wk, PLW* ek);
void check_bullet_damage(PLW* wk);
void check_dmpat_to_dmpat(PLW* /* unused */);
void add_dm_step_tbl(PLW* wk, s8 flag);
void set_dm_hos_flag_grd(PLW* wk);
void setup_smoke_type(PLW* wk);
s32 remake_initial_speeds(WORK* wk);
s32 setup_kuuchuu_nmdm(PLW* wk);

const s16 dir32_guard_air[32] = { 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,
                                  4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0 };

const s16 ris_data_table[4][4] = { { 0, 19, 23, 27 }, { 0, 15, 18, 21 }, { 0, 12, 14, 16 }, { 0, 8, 10, 18 } };

const s16 dm17_to_nm23_change[20] = {
    37, 50, 56, 103, 46, 42, 64, 78, 57, 48, 100, 45, 51, 47, 69, 65, 79, 55, 46, 51
};

const s16 hok_table[8] = { 1, 2, 3, 3, 4, 4, 5, 5 };

const s16 oki_select_table2[4] = { 13, 12, 12, 13 };

const s16 sky_dm_zuru_table[4][16] = { { 0, 161, 121, 91, 61, 41, 21, 11, 7, 3, 1, 1, 1, 1, 1, 1 },
                                       { 0, 241, 201, 161, 121, 81, 41, 21, 9, 3, 1, 1, 1, 1, 1, 1 },
                                       { 0, 321, 281, 241, 201, 171, 141, 111, 81, 61, 41, 21, 11, 7, 5, 3 },
                                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

const s16 sky_dm_zuru_ix[8] = { 1, 2, 3, 5, 7, 10, 15, 15 };

const s8 tama_select[240] = {
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const AS dm_reaction_table[115] = {
    { 0, 0, 0 },   { 1, 64, 0 },  { 2, 64, 15 }, { 3, 64, 16 }, { 4, 0, 0 },   { 5, 1, 0 },   { 6, 2, 0 },
    { 7, 3, 0 },   { 8, 4, 17 },  { 9, 5, 17 },  { 10, 6, 18 }, { 11, 7, 0 },  { 2, 71, 19 }, { 3, 72, 20 },
    { 0, 0, 0 },   { 0, 0, 0 },   { 0, 67, 0 },  { 24, 68, 0 }, { 24, 69, 0 }, { 25, 70, 0 }, { 25, 70, 0 },
    { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 },
    { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 25, 70, 0 }, { 12, 8, 0 },  { 12, 12, 0 }, { 12, 16, 0 },
    { 12, 20, 0 }, { 12, 24, 0 }, { 12, 28, 0 }, { 12, 32, 0 }, { 14, 36, 0 }, { 21, 40, 0 }, { 12, 44, 0 },
    { 12, 92, 0 }, { 12, 82, 0 }, { 12, 74, 0 }, { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },
    { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },
    { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },
    { 0, 0, 0 },   { 13, 48, 0 }, { 15, 52, 0 }, { 22, 56, 0 }, { 13, 48, 0 }, { 13, 86, 0 }, { 13, 78, 0 },
    { 27, 60, 0 }, { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },
    { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },
    { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 0, 0, 0 },   { 17, 0, 0 },  { 16, 1, 0 },  { 18, 2, 0 },
    { 18, 8, 0 },  { 18, 3, 0 },  { 19, 4, 0 },  { 19, 5, 0 },  { 18, 6, 0 },  { 18, 7, 0 },  { 20, 9, 0 },
    { 23, 10, 0 }, { 26, 11, 0 }, { 18, 12, 0 }, { 18, 13, 0 }, { 18, 14, 0 }, { 18, 8, 0 },  { 18, 15, 0 },
    { 18, 19, 0 }, { 18, 16, 0 }, { 18, 17, 0 }, { 28, 18, 0 }, { 29, 8, 18 }, { 29, 9, 20 }, { 30, 4, 32 },
    { 18, 33, 0 }, { 18, 34, 0 }, { 31, 17, 0 }
};

const s16 dd_convert[115][4] = {
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 16, 16, 16, 16 },     { 17, 17, 17, 17 },     { 18, 18, 18, 18 },     { 91, 91, 91, 91 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 17, 100, 100, 100 },  { 17, 100, 100, 100 },  { 17, 100, 94, 94 },    { 17, 100, 95, 95 },
    { 17, 36, 36, 36 },     { 17, 91, 91, 91 },     { 17, 96, 96, 96 },     { 17, 39, 39, 39 },
    { 17, 40, 40, 40 },     { 17, 91, 91, 91 },     { 103, 103, 103, 103 }, { 104, 104, 104, 104 },
    { 105, 105, 105, 105 }, { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 18, 91, 91, 91 },     { 18, 65, 65, 65 },     { 18, 66, 66, 66 },     { 103, 103, 103, 103 },
    { 104, 104, 104, 104 }, { 105, 105, 105, 105 }, { 70, 70, 70, 70 },     { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },         { 0, 0, 0, 0 },
    { 91, 91, 91, 91 },     { 101, 101, 101, 101 }, { 102, 102, 102, 102 }, { 91, 91, 91, 91 },
    { 92, 92, 92, 92 },     { 93, 93, 93, 93 },     { 94, 94, 94, 94 },     { 95, 95, 95, 95 },
    { 96, 96, 96, 96 },     { 97, 97, 97, 97 },     { 98, 98, 98, 98 },     { 99, 99, 99, 99 },
    { 100, 100, 100, 100 }, { 101, 101, 101, 101 }, { 102, 102, 102, 102 }, { 103, 103, 103, 103 },
    { 104, 104, 104, 104 }, { 105, 105, 105, 105 }, { 106, 106, 106, 106 }, { 107, 107, 107, 107 },
    { 108, 108, 108, 108 }, { 109, 109, 109, 109 }, { 110, 110, 110, 110 }, { 111, 111, 111, 111 },
    { 112, 112, 112, 112 }, { 113, 113, 113, 113 }, { 114, 114, 114, 114 }
};

const u8 guard_kind[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };

void (*const plpdm_lv_00[32])(PLW* wk) = {
    Damage_00000, Damage_01000, Damage_01000, Damage_01000, Damage_04000, Damage_04000, Damage_04000, Damage_07000,
    Damage_04000, Damage_04000, Damage_04000, Damage_07000, Damage_12000, Damage_12000, Damage_14000, Damage_14000,
    Damage_16000, Damage_17000, Damage_18000, Damage_19000, Damage_20000, Damage_21000, Damage_21000, Damage_23000,
    Damage_24000, Damage_25000, Damage_26000, Damage_27000, Damage_28000, Damage_29000, Damage_30000, Damage_31000
};

const s8 atsagct[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 17, 0, 1, 1, 0, 1, 1,
                         /* 0 */ }; // TODO: Might be 32 in length

const u16 exdm_ix_data[2][20][5] = {
    { { 15, 0, 1, 0, 359 },       { 65535, 12, 1, 0, 1801 }, { 2, 13, 1, 0, 2744 },     { 4, 2, 1, 0, 3865 },
      { 65533, 3, 1, 0, 5253 },   { 65521, 9, 1, 0, 6596 },  { 23, 65530, 1, 0, 7609 }, { 65515, 21, 1, 964, 8690 },
      { 30, 21, 1, 0, 10394 },    { 3, 23, 1, 0, 11911 },    { 4, 2, 1, 0, 13593 },     { 2, 13, 1, 0, 14808 },
      { 2, 13, 1, 0, 15864 },     { 15, 0, 1, 0, 17159 },    { 2, 13, 1, 0, 18424 },    { 65510, 9, 1, 0, 19779 },
      { 65511, 13, 1, 0, 21429 }, { 19, 0, 1, 0, 23300 },    { 4, 0, 1, 0, 25191 },     { 6, 5, 1, 0, 26103 } },
    { { 0, 0, 1, 0, 359 },      { 65535, 14, 1, 0, 1801 }, { 6, 9, 1, 0, 2744 },      { 12, 3, 1, 0, 3865 },
      { 9, 65535, 1, 0, 5253 }, { 65529, 3, 1, 0, 6596 },  { 18, 65518, 1, 0, 7609 }, { 0, 19, 1, 964, 8690 },
      { 40, 21, 1, 0, 10394 },  { 14, 22, 1, 0, 11911 },   { 12, 3, 1, 0, 13593 },    { 6, 9, 1, 0, 14808 },
      { 6, 9, 1, 0, 15864 },    { 0, 0, 1, 0, 17159 },     { 6, 9, 1, 0, 18424 },     { 25, 14, 1, 0, 19875 },
      { 16, 20, 1, 0, 21532 },  { 65530, 0, 1, 0, 23315 }, { 23, 2, 1, 0, 25264 },    { 9, 22, 1, 0, 26103 } }
};


static s32 winner_during_dramatic_pause(const PLW* wk) {
    return pcon_dp_flag && Conclusion_Type != 1 && wk->wu.id == Winner_id;
}

static s32 is_selectable_projectile(const WORK* tk) {
    return tk->work_id != 1 && tk->id == 13 && tama_select[tk->type] != 0;
}

static s32 target_is_gill_resurrecting(const PLW* twk) {
    return twk->player_number == 0 && twk->wu.now_koc == 5 && twk->wu.char_index == 59;
}

static s32 damage_interrupts_current_action(const PLW* wk) {
    return wk->wu.dm_vital && (wk->wu.routine_no[1] != 1 || wk->wu.routine_no[2] > 11 || wk->wu.routine_no[3] != 0);
}

static s32 chip_damage_would_be_fatal(const PLW* wk) {
    return wk->wu.dm_guard_success == -1 && wk->wu.vital_old > 0 && wk->wu.vital_new < 0 && wk->wu.vital_new > -3;
}

static s32 action_is_in_damage_range(const PLW* wk) {
    return wk->wu.routine_no[2] > 19 && wk->wu.routine_no[2] < 88 && wk->wu.routine_no[2] != 70;
}

void Player_damage(PLW* wk) {
    setup_damage_process_flags(wk);

    if (wk->wu.routine_no[3] == 0) {
        get_damage_reaction_data(wk);

        if (wk->wu.dm_koa & 0x980) {
            wk->ukemi_ok_timer = 0;
        } else {
            wk->ukemi_ok_timer = 6;
        }

        wk->uot_cd_ok_flag = 0;
        wk->ukemi_success = 0;
        check_bullet_damage(wk);
        clear_chainex_check(wk->wu.id);
    }

    if (wk->atemi_flag == 9) {
        wk->atemi_flag = 0;
    } else {
        plpdm_lv_00[(wk->wu.routine_no[2])](wk);
    }

    set_hit_stop_hit_quake(&wk->wu);
}

void setup_damage_process_flags(PLW* wk) { // TODO: Check this function thoroughly
    wk->wu.next_z = wk->wu.my_priority;
    wk->running_f = 0;
    wk->guard_flag = 3;
    wk->guard_chuu = 0;
    wk->tsukami_f = false;
    wk->tsukamare_f = false;
    wk->scr_pos_set_flag = 1;
    wk->dm_hos_flag = 0;

    if (ArcadeBalance_IsEnabled()) {
        wk->sa_stop_flag = 0;
    }

    wk->caution_flag = 0;
    wk->sa->saeff_ok = 0;
    wk->sa->saeff_mp = 0;
    wk->cancel_timer = 0;
    wk->hazusenai_flag = 0;
    wk->cat_break_reserve = 0;
    wk->cmd_request = 0;
    wk->hsjp_ok = 0;
    wk->high_jump_flag = 0;
    wk->wu.swallow_no_effect = 0;

    if (!ArcadeBalance_IsEnabled()) {
        if (wk->wu.routine_no[3]) {
            wk->sa_stop_flag = 0;
        }
    }
}


void first_flight_union(PLW* wk, s16 num, s16 dv) {
    jumping_union_process(&wk->wu, num);

    if (wk->wu.routine_no[3] != num) {
        return;
    }

    wk->wu.mvxy.a[0].sp /= dv;
    wk->wu.mvxy.d[0].sp = wk->wu.mvxy.kop[0] = 0;
    wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;

    if (wk->ukemi_ok_timer) {
        wk->uot_cd_ok_flag = 1;
    } else {
        wk->uot_cd_ok_flag = 0;
    }

    subtract_cu_vital(wk);
    effect_A7_init(wk);
    buttobi_chakuchi_cg_type_check(wk);

    if (wk->ukemi_ok_timer != 0 && wk->ukemi_success == 0) {
        wk->uot_cd_ok_flag = 1;
    }
}

void first_TtktV_union(PLW* wk, s16 num, s16 dv) {
    char_move(&wk->wu);

    if (wk->wu.cg_type) {
        wk->wu.routine_no[3] = num;
        wk->wu.mvxy.a[0].sp /= dv;
        wk->wu.mvxy.d[0].sp = wk->wu.mvxy.kop[0] = 0;

        if (wk->ukemi_ok_timer) {
            wk->uot_cd_ok_flag = 1;
        } else {
            wk->uot_cd_ok_flag = 0;
        }

        subtract_cu_vital(wk);
        effect_A7_init(wk);
        buttobi_chakuchi_cg_type_check(wk);

        if (wk->ukemi_ok_timer != 0 && wk->ukemi_success == 0) {
            wk->uot_cd_ok_flag = 1;
        }
    } else {
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);
    }
}

/* A quick stand needs the option enabled, the roll actually taken, and a player
 * who is alive, not dizzy and not in the dramatic pause. */
static s32 quick_stand_is_allowed(const PLW* wk) {
    return !(wk->spmv_ng_flag2 & DIP2_QUICK_STAND_DISABLED) && wk->ukemi_success && (wk->dead_flag == 0) &&
           (wk->py->flag == 0) && (wk->wu.vital_new > 0) && (pcon_dp_flag == 0);
}

/* Landing from a knockdown keeps drifting the way the player was already
 * moving. The `break` inside was the switch arm's exit with nothing after it,
 * so it is a `return` here. */
static void drift_on_landing(PLW* wk) {
    if (wk->wu.mvxy.a[0].sp > 0) {
        add_mvxy_speed_direct(&wk->wu, 128, 0);
        return;
    }

    if (wk->wu.mvxy.a[0].sp < 0) {
        add_mvxy_speed_direct(&wk->wu, -128, 0);
    }
}

/* The same drift at half the speed, preceded by the quick-stand chance. Not
 * shared with drift_on_landing: the two speeds differ in both the forward and
 * the backward literal, which is two values, not the one Recipe D allows. */
static void quick_stand_or_drift(PLW* wk) {
    if (quick_stand_is_allowed(wk)) {
        wk->wu.routine_no[2] = oki_select_table2[wk->wu.rl_waza + (wk->wu.rl_flag * 2)];
        wk->wu.routine_no[3] = 0;
        add_sp_arts_gauge_ukemi(wk);
        grade_add_quick_stand(wk->wu.id);
    }

    if (wk->wu.mvxy.a[0].sp > 0) {
        add_mvxy_speed_direct(&wk->wu, 64, 0);
        return;
    }

    if (wk->wu.mvxy.a[0].sp < 0) {
        add_mvxy_speed_direct(&wk->wu, -64, 0);
    }
}

void buttobi_chakuchi_cg_type_check(PLW* wk) {
    switch (wk->wu.cg_type) {
    case 9:
        break;

    case 1:
        add_mvxy_speed(&wk->wu);
        break;

    case 2:
        drift_on_landing(wk);
        break;

    case 5:
        quick_stand_or_drift(wk);
        break;
    }
}

void buttobi_add_y_check(PLW* wk) {
    s16 ady = _buttobi_add_y_table[wk->as->char_ix][wk->wu.dm_attlv];

    if (wk->wu.xyz[1].disp.pos < ady) {
        wk->wu.xyz[1].disp.pos = ady;
    }
}

/* Which of the four smoke effects the accumulated damage earns. The thresholds
 * and the order they are tested in are the original's; only the assignment to a
 * shared local becomes a return. */
static u8 smoke_type_for(s16 total) {
    u8 ix = 0;

    if (total >= 48) {
        ix = 1;

        if (total >= 64) {
            ix = 2;

            if (total >= 80) {
                ix = 3;
            }
        }
    }

    return ix;
}

void setup_smoke_type(PLW* wk) {
    s8* step_tbl;
    s16 i;
    s16 total;

    total = 0;
    step_tbl = wk->dm_step_tbl;

    for (i = 0; i < 32; i++) {
        total += *step_tbl++;
    }

    if (total < 0) {
        total = -total;
    }

    if (total >= 32) {
        effect_G6_init(&wk->wu, smoke_type_for(total));
    }
}

void add_dm_step_tbl(PLW* wk, s8 flag) {
    if (flag) {
        if (wk->wu.dm_rl) {
            wk->wu.xyz[0].disp.pos += *wk->dm_step_tbl++;
        } else {
            wk->wu.xyz[0].disp.pos -= *wk->dm_step_tbl++;
        }
    } else {
        wk->dm_step_tbl++;
    }
}

void check_dmpat_to_dmpat(PLW* /* unused */) {
    // Do nothing
}

/* Out of reach for the airborne damage-correction flag. Rising and falling have
 * different limits, and the comparisons keep the original's polarity. */
static s32 too_far_for_air_hosei(const PLW* wk, s16 disx) {
    if (wk->wu.mvxy.a[1].real.h <= 0) {
        return disx > 96;
    }

    return disx > 160;
}

/* The same question on the other dm_work_id path, which reaches less far. Not
 * merged with the one above: both limits differ, which is two values. */
static s32 too_far_for_ground_hosei(const PLW* wk, s16 disx) {
    if (wk->wu.mvxy.a[1].real.h <= 0) {
        return disx > 80;
    }

    return disx > 128;
}

void set_dm_hos_flag_sky(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    s16 disx = wk->wu.xyz[0].disp.pos - twk->wu.xyz[0].disp.pos;

    if (disx < 0) {
        disx = -disx;
    }

    if (wk->wu.dm_work_id & 8) {
        if (too_far_for_air_hosei(wk, disx)) {
            return;
        }

        wk->dm_hos_flag = 1;
        return;
    }

    if (!(wk->wu.dm_work_id & 1)) {
        return;
    }

    if (target_is_gill_resurrecting(twk)) {
        return;
    }

    if (too_far_for_ground_hosei(wk, disx)) {
        return;
    }

    wk->dm_hos_flag = 1;
}

void set_dm_hos_flag_grd(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    s16 disx = wk->wu.xyz[0].disp.pos - twk->wu.xyz[0].disp.pos;

    if (disx < 0) {
        disx = -disx;
    }

    if (wk->wu.dm_work_id & 8) {
        if (disx > 128) {
            return;
        }

        wk->dm_hos_flag = 1;
        return;
    }

    if (!(wk->wu.dm_work_id & 1)) {
        return;
    }

    if (target_is_gill_resurrecting(twk)) {
        return;
    }

    wk->dm_hos_flag = 1;
}

void get_sky_dm_timer(PLW* wk) {
    if (wk->wu.dm_zuru == 7) {
        wk->zuru_ix_counter = 0;
    } else {
        wk->zuru_ix_counter += sky_dm_zuru_ix[wk->wu.dm_zuru];
    }

    if (wk->zuru_ix_counter > 15) {
        wk->zuru_ix_counter = 15;
    }

    wk->zuru_timer = sky_dm_zuru_table[omop_otedama_ix[(wk->wu.id + 1) & 1]][wk->zuru_ix_counter];
}

static void apply_vital_underflow_or_piyo(PLW* wk) {
    if (wk->wu.vital_new < 0) {
        wk->wu.vital_new = -1;
        wk->dead_flag = 1;
        dead_voice_flag = true;

        if (wk->wu.dm_guard_success != -1) {
            wk->kezurijini_flag = 1;
        }

        if (!round_slow_flag) {
            set_conclusion_slow();
            round_slow_flag = true;
        }
    } else if (wk->py->flag == 0) {
        wk->py->now.quantity.h += wk->wu.dm_piyo;

        if (wk->py->now.quantity.h >= wk->py->genkai) {
            wk->py->now.timer = 0;
            wk->py->flag = 1;
        }
    }
}

/* Rumble the pad for a hit, except in the seven reaction states that do not.
 * The case labels are the reaction numbers the rest of the engine uses and are
 * unchanged, including their order. */
static void rumble_for_damage(PLW* wk) {
    switch (wk->wu.routine_no[2]) {
    case 1:
    case 2:
    case 3:
    case 12:
    case 13:
    case 19:
    case 16:
        break;

    default:
        pp_pulpara_remake_dm_all(&wk->wu);
        break;
    }
}

/* Taking the damage off the player's vitality, with the vital option that
 * zeroes it first and the two floors at zero after. subtract_dm_vital and
 * subtract_dm_vital_aiuchi ran this identically; the one statement that differs
 * between them - the super-art gauge award - stays at subtract_dm_vital's call
 * site, before the call, where it was. */
static void apply_damage_to_vital(PLW* wk) {
    if (wk->atemi_flag) {
        wk->dm_vital_backup = wk->wu.dm_vital;
    } else {
        wk->dm_vital_backup = 0;
    }

    wk->dm_vital_use = 0;

    if (omop_vital_ix[wk->wu.id] == 5) {
        wk->wu.dm_vital = 0;
    }

    wk->wu.vital_new -= wk->wu.dm_vital;

    if (chip_damage_would_be_fatal(wk)) {
        wk->wu.vital_new = 0;
    }

    if (wk->wu.dm_nodeathattack && wk->wu.vital_new < 0) {
        wk->wu.vital_new = 0;
    }

    apply_vital_underflow_or_piyo(wk);
}

void subtract_dm_vital(PLW* wk) {
    if (wk->dead_flag == 0) {
        if (damage_interrupts_current_action(wk)) {
            Additinal_Score_DM((WORK_Other*)wk->wu.dmg_adrs, wk->wu.dm_ten_ix);
        }

        add_sp_arts_gauge_hit_dm(wk);

        apply_damage_to_vital(wk);
    }

    if (wk->guard_chuu == 0) {
        rumble_for_damage(wk);
    }

    if (Mode_Type == MODE_NORMAL_TRAINING && (Training_ID != wk->wu.id)) {
        Training_Damage_Set(wk->wu.dm_vital, wk->wu.dm_piyo, wk->wu.kezurare_flag);
    }

    wk->wu.dm_vital = 0;
    wk->wu.dm_piyo = 0;
}


/* Applying a trade's damage to a player who is still alive. The order matters:
 * the vital option that zeroes the damage runs before the subtraction, and the
 * two floors at zero run after it, as in the original. */
static void take_aiuchi_damage(PLW* wk) {
    if (damage_interrupts_current_action(wk)) {
        Additinal_Score_DM((WORK_Other*)wk->wu.dmg_adrs, wk->wu.dm_ten_ix);
    }

    apply_damage_to_vital(wk);
}

void subtract_dm_vital_aiuchi(PLW* wk) {
    if (wk->dead_flag == 0) {
        take_aiuchi_damage(wk);
    }

    pp_pulpara_remake_dm_all(&wk->wu);

    if (Mode_Type == MODE_NORMAL_TRAINING && (Training_ID != wk->wu.id)) {
        Training_Damage_Set(wk->wu.dm_vital, wk->wu.dm_piyo, wk->wu.kezurare_flag);
    }

    wk->wu.dm_vital = 0;
    wk->wu.dm_piyo = 0;
}

/* Reaction 88 is "decide the knockdown type": first from the attack, then
 * overridden to 91 if the victim is dizzy or if air knockdowns are not disabled.
 * Each test re-reads routine_no[2], so an earlier override stops the later ones
 * firing - that sequencing is the original's and is why they stay three
 * separate tests. */
static void resolve_knockdown_reaction(PLW* wk) {
    if (wk->wu.routine_no[2] == 88) {
        wk->wu.routine_no[2] = check_buttobi_type(wk);
    }

    if (wk->py->flag && wk->wu.routine_no[2] == 88) {
        wk->wu.routine_no[2] = 91;
    }

    if (!(((PLW*)wk->wu.target_adrs)->spmv_ng_flag & DIP_AIR_KNOCKDOWNS_DISABLED) && wk->wu.routine_no[2] == 88) {
        wk->wu.routine_no[2] = 91;
    }
}

/* Count the damage stop one frame toward zero, from whichever side it is on. */
static void ease_damage_stop_toward_zero(PLW* wk) {
    if (wk->wu.dm_stop) {
        if (wk->wu.dm_stop > 0) {
            wk->wu.dm_stop--;
        }

        if (wk->wu.dm_stop < 0) {
            wk->wu.dm_stop++;
        }
    }
}

void get_damage_reaction_data(PLW* wk) {
    if (wk->atemi_flag == 2) {
        wk->wu.dm_vital = 0;
        damage_atemi_setup(wk, (PLW*)wk->wu.dmg_adrs);
        return;
    }

    subtract_dm_vital(wk);

    resolve_knockdown_reaction(wk);

    if (wk->dead_flag) {
        wk->wu.routine_no[2] = dd_convert[wk->wu.routine_no[2]][wk->wu.dm_attlv];
        if (action_is_in_damage_range(wk)) {
            wk->wu.routine_no[2] = check_buttobi_type2(wk);
        }
    }

    if (wk->atemi_flag == 1) {
        if (wk->py->flag) {
            wk->atemi_flag = 0;
        } else {
            damage_atemi_setup(wk, (PLW*)wk->wu.dmg_adrs);
            return;
        }
    }

    wk->as = &dm_reaction_table[wk->wu.routine_no[2]];
    wk->wu.routine_no[2] = wk->as->r_no;

    ease_damage_stop_toward_zero(wk);
}

void damage_atemi_setup(PLW* wk, PLW* ek) {
    wk->wu.routine_no[1] = wk->wu.cmmd.koc;
    wk->wu.routine_no[2] = wk->wu.cmmd.ix;
    wk->wu.routine_no[3] = wk->wu.cmmd.pat;
    char_move_cmms(&wk->wu);
    wk->atemi_flag = 9;
    wk->wu.dm_stop = wk->wu.dm_quake = 0;
    wk->wu.hit_stop = wk->wu.hit_quake = 0;
    ek->wu.dm_stop = ek->wu.dm_quake = 0;
    ek->wu.hit_stop = wk->wu.att.hs_you;
    ek->wu.hit_quake = wk->wu.att.hs_you / 2;
}

/// Setup KO state
s32 setup_kuzureochi(PLW* wk) { // 🟡
    // Player is not dead yet. Return false
    if (wk->wu.vital_new >= 0) {
        return 0;
    }

    if (!ArcadeBalance_IsEnabled()) {
        if (winner_during_dramatic_pause(wk)) {
            wk->wu.vital_new = 0;
            return 0;
        }
    }

    wk->wu.routine_no[1] = 1;
    wk->wu.routine_no[2] = 0;
    wk->wu.routine_no[3] = 1;
    wk->zuru_timer = 0;
    wk->zuru_ix_counter = 0;
    set_char_move_init(&wk->wu, 1, 73);
    wk->wu.dm_stop = wk->wu.hit_stop = 0;
    wk->wu.dm_quake = wk->wu.hit_quake = 0;
    return 1;
}

s32 setup_kuuchuu_nmdm(PLW* wk) {
    if (wk->dead_flag) {
        return 0;
    }

    if (((PLW*)wk->wu.target_adrs)->dead_flag == 0) {
        return 0;
    }

    wk->wu.routine_no[2] = 17;
    wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
    set_char_move_init(&wk->wu, 6, 0);
    check_dmpat_to_dmpat(wk);
    setup_butt_own_data(&wk->wu);
    cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
    return 1;
}

void get_catch_off_data(PLW* wk, s16 ix) {
    wk->as = &dm_reaction_table[ix];
}

void check_bullet_damage(PLW* wk) {
    WORK* tk = (WORK*)wk->wu.dmg_adrs;

    if (is_selectable_projectile(tk)) {
        wk->bullet_hcnt += tama_select[tk->type];
        wk->bhcnt_timer = 800;
    }
}
