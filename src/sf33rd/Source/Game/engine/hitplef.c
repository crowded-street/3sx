/**
 * @file hitplef.c
 * Hit Player vs Effect
 */

#include "sf33rd/Source/Game/engine/hitplef.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/io/pulpul.h"

void player_at_vs_effect_dm(s16 ix2, s16 ix) {
    PLW* as = (PLW*)q_hit_push[ix2];
    WORK_Other* ds = (WORK_Other*)q_hit_push[ix];

    pp_pulpara_hit(&as->wu);
    ds->wu.dm_rl = as->wu.rl_flag;
    cal_hit_mark_pos(&as->wu, &ds->wu, ix2, ix);

    if (ds->wu.id == 122 || ds->wu.id == 123) {
        cal_damage_vitality(as, (PLW*)ds);
    } else {
        ds->wu.dm_vital = 256;
    }

    if (ds->wu.work_id == 2 && (ds->wu.id == 122 || ds->wu.id == 123)) {
        if (ds->wu.xyz[1].disp.pos <= 0) {
            as->wu.hf.hit.player = 2;
        } else {
            as->wu.hf.hit.player = 1;
        }
    } else if (ds->wu.xyz[1].disp.pos <= 0) {
        as->wu.hf.hit.player = 32;
    } else {
        as->wu.hf.hit.player = 16;
    }

    ds->wu.routine_no[1] = 1;
    ds->wu.routine_no[2] = 0;

    if (ds->wu.work_id != 2 || ds->wu.id != 0x87) {
        if (ds->wu.att.dipsw & 2) {
            effect_02_init(&as->wu, 2, 2, ds->wu.dm_rl);
        } else if (ds->wu.id != 13) {
            effect_02_init(&as->wu, 2, 1, ds->wu.dm_rl);
        } else if (ds->wu.charset_id == 2) {
            effect_02_init(&as->wu, 2, 2, ds->wu.dm_rl);
        }
    }

    dm_status_copy(&as->wu, &ds->wu);

    if (ds->wu.work_id == 2 && ds->wu.id != 122 && ds->wu.id != 123) {
        as->wu.att_hit_ok = 1;
        as->wu.hit_stop /= 2;
        ds->wu.dm_stop /= 2;
    }

    hit_pattern_extdat_check(&as->wu);
}
