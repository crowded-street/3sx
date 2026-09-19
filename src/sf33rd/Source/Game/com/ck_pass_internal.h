/*
 * Shared between the ck_pass files.
 *
 * These three are this campaign's own: the one-, two- and three-check answers
 * the per-character option lists were folded onto. They are defined in
 * ck_pass_units_a.c and called from all four unit files, which is the only
 * reason they are not static. Nothing the decompilation wrote is widened.
 */

#ifndef CK_PASS_INTERNAL_H
#define CK_PASS_INTERNAL_H

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "structs.h"
#include "types.h"

s32 check_one_special(PLW* wk, const SP_Tech_Args* p0);
s32 check_two_specials(PLW* wk, const SP_Tech_Args* p0, const SP_Tech_Args* p1);
s32 check_three_specials(PLW* wk, const SP_Tech_Args* p0, const SP_Tech_Args* p1, const SP_Tech_Args* p2);

#endif
