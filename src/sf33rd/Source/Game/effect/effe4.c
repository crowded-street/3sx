/**
 * @file effe4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe4.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static s32 should_disable_vibration(const PLW* mwk) {
    return Training->contents[1][1][3] == 0 || mwk->wu.id == New_Challenger;
}


/* Non-zero when this effect should not run at all: the master has adopted a
 * different one, this one is already dead, or the game is not in either
 * training mode. */
static s32 e4_effect_not_wanted(const WORK_Other* ewk, const PLW* mwk) {
    return mwk->wu.E4_work_index != ewk->wu.myself || ewk->wu.dead_f != 0 ||
           (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING);
}

static void e4_apply_sa_setting(PLW* mwk, s16 num) {
    switch (num) {
    case 1:
        mwk->spmv_ng_flag2 &= 0xFFFBFFFF;
        mwk->spmv_ng_flag2 |= 0x90000;
        demo_set_sa_full(mwk->sa);
        tr_spgauge_cont_init2(mwk->wu.id);
        break;

    case 3:
        mwk->spmv_ng_flag2 &= 0xFFF7FFFF;
        mwk->spmv_ng_flag2 |= 0x50000;
        demo_set_sa_full(mwk->sa);
        tr_spgauge_cont_init2(mwk->wu.id);
        break;

    case 2:
        mwk->spmv_ng_flag2 &= 0xFFFEFFFF;
        mwk->spmv_ng_flag2 |= 0xC0000;
        clear_super_arts_point(mwk);
        tr_spgauge_cont_init(mwk->wu.id);
        break;

    case 0:
        mwk->spmv_ng_flag2 |= 0xD0000;
        clear_super_arts_point(mwk);
        tr_spgauge_cont_init(mwk->wu.id);
        break;
    }
}

static void e4_apply_move_lock(PLW* mwk) {
    if (mwk->wu.id == New_Challenger || Training->contents[1][0][2] == 0) {
        mwk->spmv_ng_flag |= 0x80;
    } else {
        mwk->spmv_ng_flag &= ~0x80;
        mwk->spmv_ng_flag &= ~0xF00;
    }
}

static void e4_setup_training(PLW* mwk) {
    s16 num;

    if (should_disable_vibration(mwk)) {
        vib_sel[mwk->wu.id] = 0;
    }

    omop_vital_ix[mwk->wu.id] = 1;
    mwk->spmv_ng_flag &= 0xFFFEFFFF;
    num = 0;

    if (New_Challenger == mwk->wu.id) {
        num = Training->contents[1][0][1];
    } else {
        num = Training->contents[1][0][3];
    }

    e4_apply_sa_setting(mwk, num);
    e4_apply_move_lock(mwk);
    omop_spmv_ng_table[mwk->wu.id] = mwk->spmv_ng_flag;
    omop_spmv_ng_table2[mwk->wu.id] = mwk->spmv_ng_flag2;
}

static void e4_start(WORK_Other* ewk, PLW* mwk) {
    if (e4_effect_not_wanted(ewk, mwk)) {
        ewk->wu.routine_no[0] = 2;
        return;
    }

    if (mwk->init_E4_flag == 0) {
        return;
    }

    mwk->init_E4_flag = 0;

    if (Mode_Type != MODE_PARRY_TRAINING) {
        return;
    }

    e4_setup_training(mwk);
}

void effect_E4_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        e4_start(ewk, mwk);
        break;
    case 1:
    case 2:
    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_E4_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 144;
    ewk->wu.work_id = 16;
    ewk->my_master = wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->master_player = wk->player_number;
    ewk->master_ng_flag = wk->spmv_ng_flag;
    ewk->master_ng_flag2 = wk->spmv_ng_flag2;
    wk->init_E4_flag = 1;
    wk->wu.E4_work_index = ewk->wu.myself;
    return 0;
}
