#include "sf33rd/Source/Game/HITEFEF.h"
#include "common.h"
#include "sf33rd/Source/Game/EFF02.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/Pow_Pow.h"

void effect_at_vs_effect_dm(s16 ix2, s16 ix) {
#if defined(TARGET_PS2)
    void cal_hit_mark_pos(WORK * as, WORK * ds, s32 ix2, s32 ix);
    s32 effect_02_init(WORK * wk, s32 dmgp, s32 mkst, s32 dmrl);
#endif

    WORK_Other* as = (WORK_Other*)q_hit_push[ix2];
    WORK_Other* ds = (WORK_Other*)q_hit_push[ix];

    ds->wu.dm_rl = as->wu.rl_flag;
    as->wu.dm_rl = ds->wu.rl_flag;
    cal_hit_mark_pos(&as->wu, &ds->wu, ix2, ix);

    if (ds->wu.att.dipsw & 2) {
        if (as->wu.att.dipsw & 2) {
            ds->wu.dm_vital = 128;
            as->wu.dm_vital = 128;
        } else {
            ds->wu.dm_vital = 128;
            as->wu.dm_vital = 128;

            if (as->wu.shell_vs_refrect == 0) {
                as->dm_refrect = 1;
                as->refrected = 1;
                as->wu.att_hit_ok = 1;
            }
        }
    } else if (ds->wu.id == 13) {
        switch (as->wu.work_id) {
        case 4:
            ds->wu.dm_vital = as->wu.vital_new;
            as->wu.dm_vital = ds->wu.vital_new;

            if (ds->wu.dm_vital > ds->wu.vital_new) {
                ds->wu.dm_vital = ds->wu.vital_new;
            }

            if (as->wu.dm_vital > as->wu.vital_new) {
                as->wu.dm_vital = as->wu.vital_new;
            }

            break;

        default:
            break;
        }
    } else if (ds->wu.id == 122 || ds->wu.id == 123) {
        cal_damage_vitality((PLW*)as, (PLW*)ds);
        as->wu.dm_vital = 256;
    } else {
        ds->wu.dm_vital = 256;
    }

    if (ds->wu.xyz[1].disp.pos > 0) {
        as->wu.hf.hit.effect = 2;
    } else {
        as->wu.hf.hit.effect = 1;
    }

    ds->wu.routine_no[1] = 1;
    ds->wu.routine_no[2] = 0;
    as->wu.hit_stop = ds->wu.hit_stop = 6;
    ds->wu.dm_dir = as->wu.dir_atthit;
    ds->wu.dm_kind_of_waza = as->wu.kind_of_waza;

    if (ds->wu.id == 122 || ds->wu.id == 123) {
        effect_02_init(&as->wu, 2, 1, ds->wu.dm_rl);
    }

    hit_pattern_extdat_check(&as->wu);
}
