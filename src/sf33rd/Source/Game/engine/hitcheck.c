/**
 * @file hitcheck.c
 * Hitcheck functions
 */

#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "arcade/arcade_balance.h"
#include "bin2obj/exchange.h"
#include "bin2obj/gauge.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitefef.h"
#include "sf33rd/Source/Game/engine/hitefpl.h"
#include "sf33rd/Source/Game/engine/hitplef.h"
#include "sf33rd/Source/Game/engine/hitplpl.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/system/sysdir.h"

#include <SDL3/SDL.h>

HS hs[32];
s16 grdb[2][2][2];
s16 grdb2[2][2];
s16* dmdat_adrs[16];
WORK* q_hit_push[32];
s16 mkm_wk[32];
s16 hpq_in;
s8 ca_check_flag;

void make_red_blocking_time(s16 id, s16 ix, s16 num) {
    switch (ix) {
    case 3:
        grdb[id][0][0] = num - (blok_r_omake[omop_r_block_ix[id]] + 2);
        grdb[id][1][0] = num - (blok_r_omake[omop_r_block_ix[id]] + 3);
        break;

    case 4:
        grdb[id][0][1] = num - (blok_r_omake[omop_r_block_ix[id]] + 2);
        grdb[id][1][1] = num - (blok_r_omake[omop_r_block_ix[id]] + 3);
        break;

    case 5:
        grdb2[id][0] = num - (blok_r_omake[omop_r_block_ix[id]] + 2);
        grdb2[id][1] = num - (blok_r_omake[omop_r_block_ix[id]] + 3);
        break;
    }
}

void hit_check_main_process() { // 🟢
    aiuchi_flag = 0;

    if (hpq_in > 1) {
        if (ca_check_flag) {
            catch_hit_check();
        }

        attack_hit_check();

        if (set_judge_result()) {
            check_result_extra();
        }
    }

    clear_hit_queue();
}

s16 set_judge_result() { // 🟢
    s16 i;
    s16 rnum = 0;

    for (i = 0; i < hpq_in; i++) {
        if (hs[i].flag.results & 0x101) {
            rnum = 1;

            if (hs[i].flag.results & 0x100) {
                set_caught_status(i);
            } else {
                set_struck_status(i);
            }
        }
    }

    return rnum;
}

static s16 get_player_damage_state(PLW* player) {
    s32 assigned = 0;

    if (player->wu.routine_no[1] == 1 && player->wu.routine_no[3] == 0) {
        assigned = 1;
    }

    return assigned;
}

static void resolve_simultaneous_player_hit() {
    WORK_Other* dm1p;
    WORK_Other* dm2p;
    s16 hs1;
    s16 hs2;
    s16 qua;

    dm1p = (WORK_Other*)plw[0].wu.dmg_adrs;
    dm2p = (WORK_Other*)plw[1].wu.dmg_adrs;

    switch ((dm1p->wu.work_id == 1) + ((dm2p->wu.work_id == 1) * 2)) {
    case 3:
        aiuchi_flag = 1;

        if ((hs1 = plw[0].wu.dm_stop) < 0) {
            hs1 = -hs1;
        }

        if ((hs2 = plw[1].wu.dm_stop) < 0) {
            hs2 = -hs2;
        }

        qua = plw[0].wu.dm_quake;

        if (qua < plw[1].wu.dm_quake) {
            qua = plw[1].wu.dm_quake;
        }

        if (hs1 > hs2) {
            plw[0].wu.hit_stop = plw[1].wu.hit_stop = hs1;
            plw[0].wu.hit_quake = plw[1].wu.hit_quake = qua;
        } else if (hs2) {
            plw[0].wu.hit_stop = plw[1].wu.hit_stop = hs2;
            plw[0].wu.hit_quake = plw[1].wu.hit_quake = qua;
        }

        plw[0].wu.dm_stop = plw[1].wu.dm_stop = 0;
        plw[0].wu.dm_quake = plw[1].wu.dm_quake = 0;
        plw[0].wu.dm_nodeathattack = plw[1].wu.dm_nodeathattack = 0;
        break;
    }
}

void check_result_extra() { // 🟢
    s16 p1state = get_player_damage_state(&plw[0]);
    s16 p2state = get_player_damage_state(&plw[1]);

    if (p1state & p2state) {
        resolve_simultaneous_player_hit();
    }
}

static void set_catcher_animation(PLW* as, PLW* ds) {
    s32 var_s4;

    var_s4 = 0;

    if (ds->wu.routine_no[1] == 1 && ds->wu.cg_type == 10) {
        var_s4 = 1;
    }

    switch (var_s4 + (((as->wu.rl_flag + ds->wu.rl_flag) & 1) * 2)) {
    case 0:
    case 3:
        as->wu.routine_no[1] = as->wu.cmcr.koc;
        as->wu.routine_no[2] = as->wu.cmcr.ix;
        as->wu.char_index = as->wu.cmcr.pat;
        break;

    default:
        as->wu.routine_no[1] = as->wu.cmcf.koc;
        as->wu.routine_no[2] = as->wu.cmcf.ix;
        as->wu.char_index = as->wu.cmcf.pat;
        break;
    }
}

static void finalize_successful_catch(PLW* as, PLW* ds, s16 blocking_status, s8 gddir) {
    ds->wu.kezurare_flag = 0;
    as->wu.routine_no[3] = 0;

    if (ds->guard_flag == 3 || blocking_status & 1) {
        ds->hazusenai_flag = 1;
    }

    as->tsukami_num = ds->player_number;
    as->tsukami_f = true;
    ds->tsukamare_f = true;
    ds->wu.routine_no[1] = 3;
    ds->wu.routine_no[2] = as->wu.att.ng_type;
    ds->wu.routine_no[3] = 0;
    grade_add_clean_hits((WORK_Other*)as);
    check_guard_miss(&as->wu, ds, gddir);

    if (as->wu.att.ng_type == 2) {
        ds->wu.xyz[1].disp.pos = as->wu.xyz[1].disp.pos;
    }

    effect_02_init(&as->wu, ds->dm_point, 1, ds->wu.dm_rl);
    dm_status_copy(&as->wu, &ds->wu);
    ds->wu.dm_vital = 0;
    as->wu.hit_stop = ds->wu.dm_stop = 0;
    as->wu.cmwk[8]++;
    as->wu.cmwk[0xF]++;
    ds->wu.dm_count_up++;
    hit_pattern_extdat_check(&as->wu);
    paring_ctr_vs[Play_Type][ds->wu.id] = 0;
    paring_counter[ds->wu.id] = 0;
    paring_bonus_r[ds->wu.id] = 0;
    pp_pulpara_hit(&as->wu);
}

typedef enum {
    CATCH_RESULT_UNDECIDED,
    CATCH_RESULT_CANCEL_CURRENT,
    CATCH_RESULT_KEEP_CURRENT,
    CATCH_RESULT_USE_BLOCKING_STATUS,
} CatchResolution;

typedef struct {
    s16 current_index;
    s16 competing_index;
    PLW* attacker;
    PLW* defender;
    s16 blocking_status;
} CatchContest;

static CatchResolution resolve_priority_catch_dipswitch(PLW* ds) {
    if (ds->wu.att.dipsw & 0x40) {
        return CATCH_RESULT_UNDECIDED;
    }

    return CATCH_RESULT_KEEP_CURRENT;
}

static CatchResolution resolve_secondary_catch_dipswitch(PLW* ds) {
    if (ds->wu.att.dipsw & 0x40) {
        return CATCH_RESULT_CANCEL_CURRENT;
    }

    if (ds->wu.att.dipsw & 0x20) {
        return CATCH_RESULT_UNDECIDED;
    }

    return CATCH_RESULT_KEEP_CURRENT;
}

static CatchResolution choose_dipswitch_catch_resolution(PLW* as, PLW* ds) {
    if (as->wu.att.dipsw & 0x40) {
        return resolve_priority_catch_dipswitch(ds);
    }

    if (as->wu.att.dipsw & 0x20) {
        return resolve_secondary_catch_dipswitch(ds);
    }

    if (ds->wu.att.dipsw & 0x60) {
        return CATCH_RESULT_CANCEL_CURRENT;
    }

    return CATCH_RESULT_USE_BLOCKING_STATUS;
}

static CatchResolution choose_catch_resolution(const CatchContest* contest) {
    CatchResolution resolution = choose_dipswitch_catch_resolution(contest->attacker, contest->defender);

    if (resolution != CATCH_RESULT_USE_BLOCKING_STATUS) {
        return resolution;
    }

    switch (contest->blocking_status) {
    case 1:
        contest->defender->hazusenai_flag = 1;
        return CATCH_RESULT_KEEP_CURRENT;

    case 2:
        contest->attacker->hazusenai_flag = 1;
        return CATCH_RESULT_CANCEL_CURRENT;

    case 3:
        contest->defender->hazusenai_flag = 1;
        contest->attacker->hazusenai_flag = 1;
        break;

    default:
        contest->attacker->cat_break_reserve = contest->defender->cat_break_reserve = 1;
        break;
    }

    return CATCH_RESULT_UNDECIDED;
}

static CatchResolution resolve_catch_tie(CatchResolution resolution) {
    if (resolution != CATCH_RESULT_UNDECIDED) {
        return resolution;
    }

    if (!(Game_timer & 1)) {
        return CATCH_RESULT_CANCEL_CURRENT;
    }

    return CATCH_RESULT_KEEP_CURRENT;
}

static bool should_cancel_competing_catch(const CatchContest* contest) {
    CatchResolution resolution;

    while (1) {
        if (!(hs[contest->competing_index].flag.results & 0x100)) {
            break;
        }

        if (contest->current_index != hs[contest->competing_index].dm_me) {
            break;
        }

        resolution = resolve_catch_tie(choose_catch_resolution(contest));

        if (resolution == CATCH_RESULT_CANCEL_CURRENT) {
            hs[contest->competing_index].flag.results &= 0x111;
            hs[contest->current_index].flag.results &= 0x1011;
            return true;
        }

        hs[contest->competing_index].flag.results &= 0x1011;
        hs[contest->current_index].flag.results &= 0x111;
        break;
    }

    return false;
}

typedef enum {
    CATCH_DEFENSE_DAMAGE,
    CATCH_DEFENSE_GUARD,
    CATCH_DEFENSE_PARRY,
} CatchDefenseResult;

typedef enum {
    CATCH_HIT_GROUND = 1,
    CATCH_HIT_SKY = 2,
} CatchHitType;

static void set_caught_damage_reaction(PLW* as, PLW* ds, s16 hit_type) {
    as->wu.hf.hit.player = hit_type;
    ds->wu.routine_no[2] = as->wu.att.reaction;
}

static bool is_forced_catch_damage(PLW* as, PLW* ds) {
    return ds->guard_flag == 3 || as->wu.att.guard == 0 || ds->py->flag != 0;
}

static bool should_use_ground_catch_damage(PLW* ds) {
    return ds->wu.xyz[1].disp.pos <= 0 && check_pat_status(&ds->wu) == 0;
}

static CatchDefenseResult resolve_catch_defense_variant(PLW* as, PLW* ds, s8 gddir, CatchHitType hit_type) {
    s32 defense_result;

    if (hit_type == CATCH_HIT_SKY) {
        defense_result = defense_sky(as, ds, gddir);
    } else {
        defense_result = defense_ground(as, ds, gddir);
    }

    switch (defense_result) {
    case 0:
        return CATCH_DEFENSE_PARRY;

    case 1:
        return CATCH_DEFENSE_GUARD;

    default:
        break;
    }

    set_caught_damage_reaction(as, ds, hit_type);
    return CATCH_DEFENSE_DAMAGE;
}

static CatchDefenseResult resolve_catch_defense(PLW* as, PLW* ds, s8 gddir) {
    if (is_forced_catch_damage(as, ds)) {
        if (should_use_ground_catch_damage(ds)) {
            set_caught_damage_reaction(as, ds, CATCH_HIT_GROUND);
        } else {
            set_caught_damage_reaction(as, ds, CATCH_HIT_SKY);
        }

        return CATCH_DEFENSE_DAMAGE;
    }

    if (ds->wu.xyz[1].disp.pos > 0) {
        return resolve_catch_defense_variant(as, ds, gddir, CATCH_HIT_SKY);
    }

    return resolve_catch_defense_variant(as, ds, gddir, CATCH_HIT_GROUND);
}

static bool apply_catch_defense_result(CatchDefenseResult result, PLW* as, PLW* ds) {
    switch (result) {
    case CATCH_DEFENSE_GUARD:
        set_guard_status(as, ds);
        pp_pulpara_hit(&as->wu);
        return true;

    case CATCH_DEFENSE_PARRY:
        set_paring_status(as, ds);
        return true;

    case CATCH_DEFENSE_DAMAGE:
        return false;
    }

    return false;
}

void set_caught_status(s16 ix) { // 🟡
    // CPS3 lacks the port-side vibration and training chip-damage metadata kept below.
    s16 ix2 = hs[ix].dm_me;
    PLW* as = (PLW*)q_hit_push[ix2];
    PLW* ds = (PLW*)q_hit_push[ix];
    s16 blocking_status = check_blocking_flag(as, ds);
    CatchContest contest;
    s8 gddir;

    while (1) {
        if (ix == hs[ix2].my_hit) {
            break;
        }
    }

    contest.current_index = ix;
    contest.competing_index = ix2;
    contest.attacker = as;
    contest.defender = ds;
    contest.blocking_status = blocking_status;

    if (should_cancel_competing_catch(&contest)) {
        return;
    }

    as->wu.hit_adrs = ds;
    ds->wu.dmg_adrs = as;
    as->wu.hit_work_id = ds->wu.work_id;
    ds->wu.dmg_work_id = as->wu.work_id;
    ds->dm_point = 1;
    gddir = get_guard_direction(&as->wu, &ds->wu);
    setup_saishin_lvdir(ds, gddir);
    setup_dm_rl(&as->wu, &ds->wu);
    set_catch_hit_mark_pos(&as->wu, &ds->wu);
    set_damage_and_piyo(as, ds);
    ds->wu.dm_guard_success = -1;

    if (apply_catch_defense_result(resolve_catch_defense(as, ds, gddir), as, ds)) {
        return;
    }

    set_catcher_animation(as, ds);
    finalize_successful_catch(as, ds, blocking_status, gddir);
}

s32 check_pat_status(WORK* wk) { // 🟢
    if (wk->pat_status >= 14 && wk->pat_status < 31) {
        return 1;
    }

    return 0;
}

s16 check_blocking_flag(PLW* as, PLW* ds) { // 🟢
    WORK_CP* wp;
    s16 num;

    wp = ds->cp;
    num = (wp->waza_flag[3] + wp->waza_flag[4]) != 0;
    wp = as->cp;
    num += (wp->waza_flag[3] + wp->waza_flag[4] != 0) << 1;
    return num;
}

void setup_catch_atthit(WORK* as, WORK* ds) { // 🟢
    set_damage_and_piyo((PLW*)as, (PLW*)ds);
    dm_status_copy(as, ds);
    as->hit_stop = ds->dm_stop = 0;
}

static bool set_configured_hit_mark_pos(WORK* attacker) {
    if (!attacker->att.mkh_ix) {
        return false;
    }

    if (attacker->rl_flag) {
        attacker->hit_mark_x = attacker->xyz[0].disp.pos - hit_mark_hosei_table[attacker->att.mkh_ix][0];
    } else {
        attacker->hit_mark_x = attacker->xyz[0].disp.pos + hit_mark_hosei_table[attacker->att.mkh_ix][0];
    }

    attacker->hit_mark_y = attacker->xyz[1].disp.pos + hit_mark_hosei_table[attacker->att.mkh_ix][1];
    return true;
}

void set_catch_hit_mark_pos(WORK* as, WORK* ds) { // 🟢
    if (!set_configured_hit_mark_pos(as)) {
        cal_hit_mark_position(ds, as, ds->h_cau->cau_box, as->h_cat->cat_box);
    }
}

void set_struck_status(s16 ix) { // 🟢
    WORK* as;
    WORK* ds;
    s16 ix2;

    ix2 = hs[ix].dm_me;

    do {

    } while (ix != hs[ix2].my_hit);

    as = q_hit_push[ix2];
    ds = q_hit_push[ix];
    as->hit_adrs = ds;
    ds->dmg_adrs = as;
    as->hit_work_id = ds->work_id;
    ds->dmg_work_id = as->work_id;

    switch ((as->work_id == 1) + ((ds->work_id == 1) * 2)) {
    case 3:
        player_at_vs_player_dm(ix2, ix);
        break;

    case 2:
        if (hs[ix].flag.results & 0x10 && ix2 == hs[ix].my_hit) {
            as->att_hit_ok = 1;
            break;
        }

        effect_at_vs_player_dm(ix2, ix);
        break;

    case 1:
        player_at_vs_effect_dm(ix2, ix);
        break;

    default:
        effect_at_vs_effect_dm(ix2, ix);
        break;
    }
}

void cal_hit_mark_pos(WORK* as, WORK* ds, s16 ix2, s16 ix) {
    if (!set_configured_hit_mark_pos(as)) {
        cal_hit_mark_position(ds, as, hs[ix].dh, hs[ix2].ah);
    }

    as->hit_mark_z = as->position_z - 8;
}

const s16 Dsas_dir_table[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 };



static bool preserves_attack_direction(WORK* as, WORK* ds) {
    if (as->work_id != 1) {
        return true;
    }

    if (check_ttk_damage_request(as->att.reaction)) {
        return true;
    }

    switch ((ds->xyz[1].disp.pos > 0) + ((as->xyz[1].disp.pos > 0) * 2)) {
    case 0:
    case 2:
        if (!(as->att.dipsw & 0x60)) {
            return true;
        }

        break;
    }

    return false;
}

void setup_dm_rl(WORK* as, WORK* ds) { // 🟢
    s16 pw;

    if (preserves_attack_direction(as, ds)) {
        ds->dm_rl = as->rl_flag;
        return;
    }

    pw = ds->xyz[0].disp.pos - as->xyz[0].disp.pos;

    if (!pw) {
        ds->dm_rl = as->rl_flag;
        return;
    }

    if (pw > 0) {
        ds->dm_rl = 1;
    } else {
        ds->dm_rl = 0;
    }
}

void dm_status_copy(WORK* as, WORK* ds) { // 🟡
    // CPS3 lacks the port-side vibration feedback kept below.
    ds->dm_attlv = as->att.level;
    ds->dm_impact = as->att.impact;
    ds->dm_dir = as->dir_atthit;
    ds->dm_stop = as->att.hs_you;
    ds->dm_quake = as->att.hs_you;
    ds->dm_weight = as->weight_level;
    ds->dm_butt_type = as->att.but_ix;
    ds->dm_zuru = as->att_zuru;
    ds->dm_attribute = as->at_attribute;
    ds->dm_ten_ix = as->at_ten_ix;
    ds->dm_koa = as->at_koa;
    ds->hm_dm_side = as->att.dmg_mark;
    ds->dm_work_id = as->work_id;
    as->hit_stop = as->att.hs_me;
    ds->dm_arts_point = as->add_arts_point;
    ds->dm_kind_of_waza = as->kind_of_waza;
    ds->dm_nodeathattack = as->no_death_attack;
    ds->dm_jump_att_flag = as->jump_att_flag;

    if (ds->dm_quake < 0) {
        ds->dm_quake = -ds->dm_quake;
    }

    if (as->work_id == 1) {
        ds->dm_exdm_ix = ((PLW*)as)->exdm_ix;
        ds->dm_plnum = ((PLW*)as)->player_number;
        pp_pulpara_remake_at_hit(as);
    } else {
        ds->dm_plnum = ((PLW*)((WORK_Other*)as)->my_master)->player_number;
    }

    as->meoshi_hit_flag = 1;
}

static void apply_combo_work(PLW* as, PLW* ds) {
    s16* kow;
    s16* cal;

    ds->kizetsu_kow = ds->cb->new_dm = as->wu.kind_of_waza;
    kow = ds->cb->kind_of[0][0];
    cal = calc_hit[ds->wu.id];
    kow[as->wu.kind_of_waza]++;
    cal[(as->wu.kind_of_waza & 120) / 8]++;
    ds->cb->total++;
    kow = ds->rp->kind_of[0][0];
    kow[as->wu.kind_of_waza]++;
    ds->rp->total++;
}

void add_combo_work(PLW* as, PLW* ds) { // 🟢
    if (ds->kezurijini_flag) {
        return;
    }

    apply_combo_work(as, ds);
}

void nise_combo_work(PLW* as, PLW* ds, s16 num) { // 🟢
    s16 i;

    for (i = 0; i < num; i++) {
        apply_combo_work(as, ds);
    }
}

static TBL calculate_combo_scaling_index(PLW* ds, KOATT* koatt) {
    s16 i;
    s16 j;
    s16 k;
    TBL tbl;

    tbl.ixl = 0;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 4; j++) {
            k = ds->rp->kind_of[i][j][0];
            k += ds->rp->kind_of[i][j][1];

            if (k) {
                tbl.ixl += k * koatt->step[i][j] * 256;
            }
        }
    }

    return tbl;
}

static bool uses_special_combo_power(PLW* as) {
    if (as->player_number != CHAR_YUN) {
        if (as->player_number != CHAR_YANG) {
            return false;
        }
    }

    if (as->sa->kind_of_arts != 2) {
        return false;
    }

    return as->sa->ok == -1;
}

static POWER* select_combo_power(PLW* as) {
    if (uses_special_combo_power(as)) {
        return (POWER*)_exchange_pow_pl03_sa3[as->wu.kind_of_waza >> 1];
    }

    return (POWER*)_exchange_pow[as->wu.kind_of_waza >> 1];
}

void cal_combo_waribiki(PLW* as, PLW* ds) { // 🟢
    POWER* power;
    KOATT* koatt;
    TBL tbl;

    if (ds->wu.dm_vital == 0) {
        return;
    }

    if (ds->rp->total == 0) {
        return;
    }

    koatt = (KOATT*)_exchange_koa[(as->wu.kind_of_waza) >> 1];
    tbl = calculate_combo_scaling_index(ds, koatt);

    if (tbl.ixs.l) {
        tbl.ixs.h++;
    }

    power = select_combo_power(as);

    if (tbl.ixs.h > 31) {
        tbl.ixs.h = 31;
    }

    ds->wu.dm_vital *= power[0].data[tbl.ixs.h];
    ds->wu.dm_vital >>= 5;

    if (ds->wu.dm_vital <= 0) {
        ds->wu.dm_vital = 1;
    }
}

void cal_combo_waribiki2(PLW* ds) { // 🟢
    s16 num;

    if (ds->wu.dm_piyo == 0) {
        return;
    }

    if (ds->cb->total == 0) {
        return;
    }

    num = 32 - (ds->cb->total * 2);

    if (num <= 0) {
        num = 1;
    }

    if (num > 32) {
        num = 32;
    }

    ds->wu.dm_piyo = (ds->wu.dm_piyo * num) / 32;

    if (ds->wu.dm_piyo == 0) {
        ds->wu.dm_piyo = 1;
    }
}


const u16 chain_normal_ground_table[8] = { 0x760, 0x660, 0x640, 0x440, 0x400, 0x0, 0x0, 0x0 };
const u16 chain_hidou_nm_ground_table[8] = { 0x770, 0x770, 0x740, 0x470, 0x600, 0x60, 0x0, 0x0 };
const u16 chain_normal_air_table[8] = { 0x660, 0x660, 0x440, 0x440, 0x0, 0x0, 0x0, 0x0 };
const u16 chain_hidou_nm_air_table[8] = { 0x320, 0x220, 0x640, 0x440, 0x510, 0x110, 0x0, 0x0 };
const u8 plpat_rno_filter[16] = { 1, 9, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const s16 rsix_r_table[17][2] = { { 61, 1 },   { 121, 2 },  { 181, 3 },  { 241, 4 },  { 301, 5 },  { 361, 6 },
                                  { 421, 7 },  { 481, 8 },  { 541, 9 },  { 601, 10 }, { 661, 11 }, { 721, 12 },
                                  { 781, 13 }, { 841, 14 }, { 901, 15 }, { 961, 16 }, { 999, 17 } };

const s16 attr_flame_tbl[83] = { 42,  42,  42,  42,  42,  42,  42,  42,  42,  42,  42,  42,  42,  0,   0,   0,   0,
                                 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   67,  67,
                                 67,  67,  67,  67,  67,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                 0,   0,   0,   0,   0,   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
                                 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103 };

const s16 attr_thunder_tbl[83] = { 43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  43,  0,   0,   0,   0,
                                   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   68,  68,
                                   68,  68,  68,  68,  68,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                   0,   0,   0,   0,   0,   104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104,
                                   104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104 };

const s16 attr_freeze_tbl[83] = { 44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,  0,   0,   0,   0,
                                  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   69,  69,
                                  69,  69,  69,  69,  69,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  0,   0,   0,   0,   0,   105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
                                  105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105 };

const s16 sky_nm_damage_tbl[83] = { 88,  88,  88,  88,  88,  88,  88,  97,  98, 88,  103, 104, 105, 0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  88, 97,
                                    98,  103, 104, 105, 98,  0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   88,  89,  90,  91, 92,  93,  94,  95,  96,  97, 98, 99,
                                    100, 101, 102, 103, 104, 105, 106, 107, 91, 109, 110, 111, 112, 113, 114 };

const s16 sky_sp_damage_tbl[83] = { 91,  91,  91,  95,  91,  91,  96,  97,  98, 91,  103, 104, 105, 0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  91, 97,
                                    98,  103, 104, 105, 98,  0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   88,  89,  90,  91, 92,  93,  94,  95,  96,  97, 98, 90,
                                    100, 101, 102, 103, 104, 105, 106, 107, 91, 109, 110, 111, 97,  113, 114 };

const s16 kagami_damage_tbl[83] = { 64,  64,  64,  64,  64,  64,  64,  65,  66, 64,  67,  68,  69,  0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  64, 65,
                                    66,  67,  68,  69,  70,  0,   0,   0,   0,  0,   0,   0,   0,   0,   0,  0,  0,
                                    0,   0,   0,   0,   0,   88,  89,  90,  91, 92,  93,  94,  95,  96,  65, 66, 90,
                                    100, 101, 102, 103, 104, 105, 106, 107, 65, 109, 110, 111, 112, 113, 114 };

const s16 grd_hand_damage_tbl[83] = { 41,  41,  41,  41,  41,  41,  41,  41, 41,  41, 42, 43,  44,  0,   0,  0,  0,
                                      0,   0,   0,   0,   0,   0,   0,   0,  0,   0,  0,  0,   0,   0,   0,  41, 41,
                                      41,  42,  43,  44,  41,  0,   0,   0,  0,   0,  0,  0,   0,   0,   0,  0,  0,
                                      0,   0,   0,   0,   0,   88,  89,  90, 91,  91, 93, 94,  100, 91,  39, 40, 99,
                                      100, 101, 102, 103, 104, 105, 106, 96, 108, 91, 91, 111, 112, 113, 114 };

const u8 hddm_damage_tbl[83] = { 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const u8 trdm_damage_tbl[83] = { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const u8 ttk_damage_req_tbl[83] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1 };

const u8 parisucc_pts[2][40] = {
    { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30,  32,  34,  36,  38,
      40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 100, 100, 100, 100 },
    { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
      20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 }
};

const u8 dm_oiuchi_catch[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1 };
