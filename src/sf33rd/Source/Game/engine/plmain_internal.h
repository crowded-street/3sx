#ifndef PLMAIN_INTERNAL_H
#define PLMAIN_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between plmain.c and the PS2 super-art states split out of it.
 * Nothing here is meant to be called from outside that pair.
 *
 * abandon_super_art and clear_super_art_state were `static` in plmain.c. The
 * split is the only reason they are not: both state machines end an art the
 * same way, and the port's states moved to their own file. Recipe S allows the
 * widening and asks for this note. */
void abandon_super_art(PLW* wk);
void clear_super_art_state(PLW* wk);

/* Defined in plmain_ps2_arts.c; plmain.c's sag_union picks it when Arcade
 * Balance is off. */
void sag_union_ps2(PLW* wk);

/* Defined in plmain.c, and the port's timed art adds its attribute the same
 * way the CPS3 one does. */
void addSAAttribute(u8* kow, u16* koa);

/* Defined in plmain.c, next to the state machines that index them; the OMOP
 * vitality drain asks the same two questions of a state. The tables themselves
 * are untouched - only a declaration moved. */
extern const u8 plpnm_mvkind[59];
extern const u8 plpdm_mvkind[32];
extern const u8 plpxx_kind[5];

/* Defined in plmain_vital.c; plmain.c's player_mv_4000 runs it each frame. */
void check_omop_vital(PLW* wk);

#endif // PLMAIN_INTERNAL_H
