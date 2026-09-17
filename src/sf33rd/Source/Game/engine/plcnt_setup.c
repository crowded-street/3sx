/**
 * @file plcnt_setup.c
 * Per-player data setup: base work, shadows, stun and super-art status.
 *
 * Split out of plcnt.c, which was 1008 lines of code and tripped Lines of Code
 * in a Single File. Every function here already had external linkage and is
 * declared in plcnt.h, so the split widened nothing, and none of them called a
 * `static` that stays behind.
 */

#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plcnt_internal.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "main.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/effect/eff00.h"
#include "sf33rd/Source/Game/effect/eff01.h"
#include "sf33rd/Source/Game/effect/eff33.h"
#include "sf33rd/Source/Game/effect/effc9.h"
#include "sf33rd/Source/Game/effect/effd3.h"
#include "sf33rd/Source/Game/effect/effe3.h"
#include "sf33rd/Source/Game/effect/effe4.h"
#include "sf33rd/Source/Game/effect/effe5.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effj7.h"
#include "sf33rd/Source/Game/effect/effk5.h"
#include "sf33rd/Source/Game/effect/effk7.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include <SDL3/SDL.h>

void setup_base_and_other_data() {
    make_texcash_work(3);
    make_texcash_work(4);
    make_texcash_work(6);
    plw[0].wu.my_mts = 3;
    plw[1].wu.my_mts = 4;
    set_base_data(&plw[0], 0);
    set_base_data(&plw[1], 1);
    plw[0].sa = &super_arts[0];
    plw[1].sa = &super_arts[1];
    plw[0].py = &piyori_type[0];
    plw[1].py = &piyori_type[1];
    setup_other_data(&plw[0]);
    setup_other_data(&plw[1]);
    effect_work_list_init(6, 0xC5);
    plw[0].gill_ccch_go = plw[1].gill_ccch_go = 0;
    effect_J7_init(&plw[0]);
    effect_J7_init(&plw[1]);
    effect_E5_init(&plw[0]);
    effect_E5_init(&plw[1]);

    if (plw[0].wu.my_priority == plw[1].wu.my_priority) {
        plw[0].the_same_players = plw[1].the_same_players = 1;
    }

    poison_flag[0] = 0;
    poison_flag[1] = 0;

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        effect_E3_init(&plw[0]);
        effect_E3_init(&plw[1]);
        effect_E4_init(&plw[0]);
        effect_E4_init(&plw[1]);
    }
}

void setup_any_data() {
    set_base_data_tiny(&plw[0]);
    set_base_data_tiny(&plw[1]);
    setup_other_data(&plw[0]);
    setup_other_data(&plw[1]);
    effect_work_list_init(6, 0xC5);
    plw[0].gill_ccch_go = plw[1].gill_ccch_go = 0;
    effect_J7_init(&plw[0]);
    effect_J7_init(&plw[1]);
    effect_E5_init(&plw[0]);
    effect_E5_init(&plw[1]);

    if (plw[0].wu.my_priority == plw[1].wu.my_priority) {
        plw[0].the_same_players = plw[1].the_same_players = 1;
    }
}

void set_base_data(PLW* wk, s16 ix) {
    wk->wu.be_flag = 1;
    wk->wu.disp_flag = 0;
    wk->wu.blink_timing = ix;
    wk->wu.id = ix;
    wk->wu.work_id = 1;
    wk->wu.operator = Operator_Status[ix];
    wk->wu.charset_id = plid_data[My_char[ix]];
    wk->wkey_flag = wk->dead_flag = 0;
    set_char_base_data(&wk->wu);
    wk->wu.target_adrs = &plw[(ix + 1) & 1];
    wk->player_number = My_char[ix];
    wk->wu.hit_adrs = wk->wu.target_adrs;
    wk->wu.dmg_adrs = wk->wu.target_adrs;
    cmd_init(wk);
    wk->cb = &combo_type[ix];
    wk->rp = &remake_power[ix];

    if (ix) {
        wk->wu.my_col_code |= 0x10;
    }

    wk->spmv_ng_flag = omop_spmv_ng_table[wk->wu.id];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[wk->wu.id];
    wk->wu.weight_level = weight_lv_table[wk->player_number];
    set_player_shadow(wk);
    wk->wu.cg_olc_ix = wk->wu.cg_hit_ix = 0;
    wk->wu.cg_olc = wk->wu.olc_ix_table[wk->wu.cg_olc_ix];
    wk->wu.cg_ja = wk->wu.hit_ix_table[wk->wu.cg_hit_ix];

    set_jugde_area(&wk->wu);
}

void set_base_data_metamorphose(PLW* wk, s16 dmid) {
    set_char_base_data(&wk->wu);

    if (wk->wu.id) {
        wk->wu.my_col_code |= 0x10;
    }

    cmd_init(wk);
    wk->spmv_ng_flag = omop_spmv_ng_table[dmid];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[dmid];
    set_player_shadow(wk);
}

void set_base_data_tiny(PLW* wk) {
    wk->wu.charset_id = plid_data[My_char[wk->wu.id]];
    wk->player_number = My_char[wk->wu.id];
    set_char_base_data(&wk->wu);

    if (wk->wu.id) {
        wk->wu.my_col_code |= 0x10;
    }

    wk->wu.be_flag = 1;
    wk->wu.disp_flag = 0;
    wk->wkey_flag = wk->dead_flag = 0;
    cmd_init(wk);
    wk->spmv_ng_flag = omop_spmv_ng_table[wk->wu.id];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[wk->wu.id];
    wk->wu.weight_level = weight_lv_table[wk->player_number];
    set_player_shadow(wk);
}

void set_player_shadow(PLW* wk) {
    wk->wu.kage_flag = 1;
    wk->wu.kage_prio = 68;
    wk->wu.kage_hx = kage_base[wk->player_number][0];
    wk->wu.kage_char = kage_base[wk->player_number][1];
}

void setup_other_data(PLW* wk) {
    s16 i;

    if (wk->player_number == 0) {
        setup_GILL_exsa_obj();
    }

    for (i = 0; i < 4; i++) {
        effect_01_init(&wk->wu, i);
    }

    effect_K5_init(wk);
    effect_00_init(&wk->wu);
}

void clear_chainex_check(s16 ix) {
    s16 i;

    for (i = 0; i < 36; i++) {
        chainex_check[ix][i] = 0;
    }
}

void set_kizetsu_status(s16 ix) { // 🟢
    s16 plnum = My_char[ix];

    piyori_type[ix].flag = 0;
    piyori_type[ix].time = 0;
    piyori_type[ix].now.timer = 0;
    piyori_type[ix].store = 0;
    piyori_type[ix].recover = pl_nr_piyo_tbl[plnum];
    piyori_type[ix].genkai = pl_piyo_tbl[plnum] + stun_gauge_len_omake[omop_stun_gauge_len[ix]];

    if (piyori_type[ix].genkai < 56) {
        piyori_type[ix].genkai = 56;
    }

    if (piyori_type[ix].genkai > 72) {
        piyori_type[ix].genkai = 72;
    }
}

void clear_kizetsu_point(PLW* wk) { // 🟢
    wk->py->flag = 0;
    wk->py->time = 0;
    wk->py->now.timer = 0;
    wk->py->store = 0;
    wk->py->recover = pl_nr_piyo_tbl[wk->player_number];
}

/* The run both super-art setups open with, character for character: pick the row
 * - the DC table when the player is on a command select or has no art - then
 * copy the six move indices, the fourth-EX flag and the gauge type across. The
 * row is returned because each caller reads more fields from it afterwards. */
static const SA_DATA* apply_shared_sa_status(s16 ix) {
    const SA_DATA* saptr;

    if (cmd_sel[ix] || no_sa[ix]) {
        saptr = &super_arts_DATA[My_char[ix]][Super_Arts[ix]];
    } else {
        saptr = &super_arts_data[My_char[ix]][Super_Arts[ix]];
    }

    super_arts[ix].kind_of_arts = Super_Arts[ix];
    super_arts[ix].nmsa_g_ix = saptr->nmsa_g_ix;
    super_arts[ix].exsa_g_ix = saptr->exsa_g_ix;
    super_arts[ix].exs2_g_ix = saptr->exs2_g_ix;
    super_arts[ix].nmsa_a_ix = saptr->nmsa_a_ix;
    super_arts[ix].exsa_a_ix = saptr->exsa_a_ix;
    super_arts[ix].exs2_a_ix = saptr->exs2_a_ix;
    super_arts[ix].ex4th_full = saptr->ex4th_full;
    super_arts[ix].gauge_type = saptr->gauge_type;

    return saptr;
}

void set_super_arts_status(s16 ix) { // 🟢
    const SA_DATA* saptr = apply_shared_sa_status(ix);
    super_arts[ix].gt2 = saptr->gauge_type;
    super_arts[ix].gauge_len = remake_sa_gauge_len(ix, saptr->gauge_len);
    super_arts[ix].store_max = remake_sa_store_max(ix, saptr->store_max);
    super_arts[ix].dtm = saptr->dtm;
    super_arts[ix].dtm_mul = 1;
    super_arts[ix].store = 0;
    super_arts[ix].gauge.s.h = 0;
    super_arts[ix].gauge.s.l = -1;
    super_arts[ix].sa_rno = 0;
    super_arts[ix].ok = 0;
}

s16 remake_sa_store_max(s16 ix, s16 store_max) { // 🔴
    s16 num = store_max + sag_stock_omake[omop_sag_max_ix[ix]];

    if (num <= 0) {
        num = 1;
    }

    if (num > 9) {
        num = 9;
    }

    return num;
}

s16 remake_sa_gauge_len(s16 ix, s16 gauge_len) { // 🔴
    s16 num = gauge_len + sag_length_omake[omop_sag_len_ix[ix]] * 8;

    if (num < 0x40) {
        num = 0x40;
    }

    if (num > 0x80) {
        num = 0x80;
    }

    return num;
}

void set_super_arts_status_dc(s16 ix) { // 🔴
    const SA_DATA* saptr = apply_shared_sa_status(ix);
    super_arts[ix].gauge_len = remake_sa_gauge_len(ix, saptr->gauge_len);
    super_arts[ix].store_max = remake_sa_store_max(ix, saptr->store_max);
    super_arts[ix].dtm = saptr->dtm;
    super_arts[ix].dtm_mul = 1;
}

void clear_super_arts_point(PLW* wk) { // 🟡
    if (!ArcadeBalance_IsEnabled()) {
        wk->sa->store = 0;
    }

    wk->sa->gauge.s.h = 0;
    wk->sa->gauge.s.l = -1;
    wk->sa->mp_rno = 0;

    if (!ArcadeBalance_IsEnabled()) {
        wk->sa->mp_rno2 = 0;
    }

    wk->sa->sa_rno = 0;

    if (!ArcadeBalance_IsEnabled()) {
        wk->sa->sa_rno2 = 0;
    }

    wk->sa->ex_rno = 0;
    wk->sa->mp = 0;
    wk->sa->ok = 0;
    wk->sa->ex = 0;

    if (!ArcadeBalance_IsEnabled()) {
        wk->sa->bacckup_g_h = 0;
    }
}
