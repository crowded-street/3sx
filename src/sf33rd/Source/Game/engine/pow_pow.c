/**
 * @file pow_pow.c
 * Damage Calculation
 */

#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pow_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void cal_damage_vitality(PLW* as, PLW* ds) {
    u16 xx = as->wu.att.pow;
    s16 yy;
    s16 power = Power_Data[xx];

    if (Play_Type == 1) {
        yy = Pow_Control_Data_1[0][3];
    } else {
        yy = Pow_Control_Data_1[0][Round_Level];
    }

    ds->wu.dm_vital = (power * yy) / 100;

    if (as->wu.work_id == 1) {
        ds->wu.dm_vital = (ds->wu.dm_vital * as->att_plus) / 8;
    }

    if (ds->wu.work_id == 1) {
        ds->wu.dm_vital = (ds->wu.dm_vital * ds->def_plus) / 8;
    }
}

void cal_damage_vitality_eff(WORK_Other* as, PLW* ds) {
    u16 xx = as->wu.att.pow;
    s16 yy;
    s16 power = Power_Data[xx];

    if (Play_Type == 1) {
        yy = Pow_Control_Data_1[0][3];
    } else {
        yy = Pow_Control_Data_1[0][Round_Level];
    }

    ds->wu.dm_vital = (power * yy) / 100;

    if (as->wu.work_id == 1) {
        ds->wu.dm_vital = (ds->wu.dm_vital * ((PLW*)as)->att_plus) / 8;
    }

    if (ds->wu.work_id == 1) {
        ds->wu.dm_vital = (ds->wu.dm_vital * ds->def_plus) / 8;
    }
}

void Additinal_Score_DM(WORK_Other* wk, u16 ix) {
    s16 id;

    if (wk->wu.work_id == 1) {
        id = wk->wu.id;
    } else {
        if (((WORK*)wk->my_master)->work_id != 1) {
            return;
        }

        id = wk->master_id;
    }

    Score[id][2] += Score_Data[ix];

    if (Score[id][2] >= 99999900) {
        Score[id][2] = 99999900;
    }

    if ((Mode_Type != MODE_VERSUS) && (Mode_Type != MODE_REPLAY)) {
        if (!plw[id].wu.operator) {
            return;
        }

        Score[id][Play_Type] += Score_Data[ix];

        if (Score[id][Play_Type] >= 99999900) {
            Score[id][Play_Type] = 99999900;
        }
    } else {
        Score[id][Play_Type] += Score_Data[ix];

        if (Score[id][Play_Type] >= 99999900) {
            Score[id][Play_Type] = 99999900;
        }
    }
}
