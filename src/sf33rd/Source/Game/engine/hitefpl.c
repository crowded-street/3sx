/**
 * @file hitefpl.c
 * Hit Effect vs Player
 */

#include "sf33rd/Source/Game/engine/hitefpl.h"
#include "bin2obj/gauge.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/system/sysdir.h"

void setup_dm_rl_pldm(WORK* as, WORK* ds);

void effect_at_vs_player_dm(s16 ix2, s16 ix) {
    WORK_Other* as = (WORK_Other*)q_hit_push[ix2];
    PLW* ds = (PLW*)q_hit_push[ix];
    PLW* ms;
    s8 gddir;

    ds->dm_point = hs[ix].dm_body;
    gddir = get_guard_direction(&as->wu, &ds->wu);
    setup_saishin_lvdir(ds, gddir);
    setup_dm_rl_pldm(&as->wu, &ds->wu);
    cal_hit_mark_pos(&as->wu, &ds->wu, ix2, ix);
    cal_damage_vitality_eff(as, ds);
    ds->wu.dm_piyo = _add_piyo_gauge[as->master_player][as->wu.att.piyo];
    ds->wu.dm_piyo = ds->wu.dm_piyo * stun_gauge_omake[omop_stun_gauge_add[(ds->wu.id + 1) & 1]] / 32;

    if ((ds->wu.pat_status == 32 || ds->wu.pat_status == 3) || ds->wu.pat_status == 25) {
        ds->wu.dm_vital = (ds->wu.dm_vital * 125) / 100;
    } else if (ds->wu.pat_status == 7 || ds->wu.pat_status == 23 || ds->wu.pat_status == 35) {
        ds->wu.dm_vital = (ds->wu.dm_vital * 150) / 100;
    } else if (ds->wu.pat_status == 1 || ds->wu.pat_status == 21 || ds->wu.pat_status == 37) {
        ds->wu.dm_vital *= 2;
    }

    ms = (PLW*)as->my_master;

    if (ms->wu.work_id == 1) {
        if (as->wu.olc_work_ix[3] == 2) {
            ds->wu.dm_vital = ds->wu.dm_vital * (as->wu.olc_work_ix[1] + 32) / 32;
        }

        if (as->wu.olc_work_ix[3] == 4) {
            ds->wu.dm_vital = ds->wu.dm_vital * (as->wu.olc_work_ix[0] + 32) / 32;
        }

        ds->utk_dageki = as->wu.olc_work_ix[0];
        ds->utk_nage = as->wu.olc_work_ix[1];
        ds->wu.dm_piyo = ds->wu.dm_piyo * (as->wu.olc_work_ix[2] + 32) / 32;
        ds->utk_kizetsu = as->wu.olc_work_ix[2];
    }

    as->wu.at_ten_ix = remake_score_index(ds->wu.dm_vital);
    cal_combo_waribiki((PLW*)as, ds);
    cal_dm_vital_gauge_hosei(ds);
    cal_combo_waribiki2(ds);
    as->wu.dm_vital = 256;
    ds->atemi_flag = 0;
    plef_at_vs_player_damage_union((PLW*)as, ds, gddir);
}

void setup_dm_rl_pldm(WORK* as, WORK* ds) {
    s16 pw;

    ds->dm_rl = as->rl_flag;

    if (ds->xyz[1].disp.pos <= 0) {
        return;
    }

    if (!(as->att.dipsw & 2)) {
        return;
    }

    pw = ds->xyz[0].disp.pos - as->xyz[0].disp.pos;

    if (pw) {
        if (pw > 0) {
            ds->dm_rl = 1;
        } else {
            ds->dm_rl = 0;
        }
    } else {
        ds->dm_rl = as->rl_flag;
    }
}
