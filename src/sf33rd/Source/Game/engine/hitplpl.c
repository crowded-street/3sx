/**
 * @file hitplpl.c
 * Hit Player vs Player
 */

#include "sf33rd/Source/Game/engine/hitplpl.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/io/pulpul.h"

/* The defender's own attack is at least as strong as the attacker's, so
 * neither lands. */
static s32 defender_outranks_attacker(PLW* ds) {
    if (ds->wu.att.dipsw & 0x20) {
        return 1;
    }

    if (ds->wu.kind_of_waza & 4) {
        return 1;
    }

    return 0;
}

/* The defender's attack carries no priority of its own. */
static s32 defender_has_no_priority(PLW* ds) {
    return !(ds->wu.att.dipsw & 0x60) && !(ds->wu.kind_of_waza & 4);
}

/* Neither attack carries priority, which is the last case the trade has. */
static s32 neither_side_has_priority(PLW* as, PLW* ds) {
    return !(as->wu.kind_of_waza & 6) && !(ds->wu.att.dipsw & 0x60) && !(ds->wu.kind_of_waza & 4);
}

void player_at_vs_player_dm(s16 ix2, s16 ix) {
    PLW* as = (PLW*)q_hit_push[ix2];
    PLW* ds = (PLW*)q_hit_push[ix];
    s8 gddir;

    while (1) {
        if (ix != hs[ix2].my_hit) {
            continue;
        };

        if (!(hs[ix2].flag.results & 1)) {
            break;
        }

        if (ix != hs[ix2].dm_me) {
            break;
        }

        if (as->wu.att.dipsw & 0x40) {
            if (!(ds->wu.att.dipsw & 0x40)) {
                goto two;
            }

            break;
        }

        /* These two attacker cases ran the same block in the original; they
         * share it here, with both tests kept as they were written. */
        if ((as->wu.att.dipsw & 0x20) || (as->wu.kind_of_waza & 4)) {
            if (!(ds->wu.att.dipsw & 0x40)) {
                if (defender_outranks_attacker(ds)) {
                    break;
                }

                goto two;
            }
        } else if (as->wu.kind_of_waza & 2) {
            if (defender_has_no_priority(ds)) {
                if (ds->wu.kind_of_waza & 2) {
                    break;
                }

                goto two;
            }
        } else if (neither_side_has_priority(as, ds)) {
            if (!(ds->wu.kind_of_waza & 2)) {
                break;
            }
        }

        hs[ix2].flag.results &= 0x1101;
        hs[ix].flag.results &= 0x1110;
        return;

    two:
        hs[ix2].flag.results &= 0x1110;
        hs[ix].flag.results &= 0x1101;
        break;
    }

    pp_pulpara_hit(&as->wu);
    ds->dm_point = hs[ix].dm_body;
    gddir = get_guard_direction(&as->wu, &ds->wu);
    setup_saishin_lvdir(ds, gddir);
    setup_dm_rl(&as->wu, &ds->wu);
    cal_hit_mark_pos(&as->wu, &ds->wu, ix2, ix);
    set_damage_and_piyo(as, ds);
    plef_at_vs_player_damage_union(as, ds, gddir);
}
