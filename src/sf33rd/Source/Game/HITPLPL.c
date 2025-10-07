#include "sf33rd/Source/Game/HITPLPL.h"
#include "common.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/PulPul.h"

void player_at_vs_player_dm(s16 ix2, s16 ix) {
#if defined(TARGET_PS2)
    void setup_saishin_lvdir(PLW * ds, s32 gddir);
    void cal_hit_mark_pos(WORK * as, WORK * ds, s32 ix2, s32 ix);
    void plef_at_vs_player_damage_union(PLW * as, PLW * ds, s32 gddir);
#endif

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

        if (as->wu.att.dipsw & 0x20) {
            if (!(ds->wu.att.dipsw & 0x40)) {
                if (ds->wu.att.dipsw & 0x20) {
                    break;
                }

                if (ds->wu.kind_of_waza & 4) {
                    break;
                }

                goto two;
            }
        } else if (as->wu.kind_of_waza & 4) {
            if (!(ds->wu.att.dipsw & 0x40)) {
                if (ds->wu.att.dipsw & 0x20) {
                    break;
                }

                if (ds->wu.kind_of_waza & 4) {
                    break;
                }

                goto two;
            }
        } else if (as->wu.kind_of_waza & 2) {
            if (!(ds->wu.att.dipsw & 0x60) && !(ds->wu.kind_of_waza & 4)) {
                if (ds->wu.kind_of_waza & 2) {
                    break;
                }

                goto two;
            }
        } else if (!(as->wu.kind_of_waza & 6) && !(ds->wu.att.dipsw & 0x60) && !(ds->wu.kind_of_waza & 4)) {
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
