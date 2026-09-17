#ifndef PLPAT_INTERNAL_H
#define PLPAT_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between plpat.c and plpat_ja.c, which was split out of it. Every name
 * here was already file-scope with external linkage in plpat.c; nothing is
 * meant to be called from outside the pair. */

/* Defined in plpat_ja.c. Attack_03000's airborne arm drives the dummy-RTNM
 * window through these two. */
void check_ja_nmj_dummy_RTNM(PLW* wk);
s16 ja_nmj_rno_change(WORK* wk);

/* Defined in plpat_ja.c, used by check_ja_nmj_dummy_RTNM to size the window. */
u8 get_cjdR(PLW*);

#endif // PLPAT_INTERNAL_H
