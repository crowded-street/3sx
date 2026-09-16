#ifndef PLS03_INTERNAL_H
#define PLS03_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between pls03.c and pls03_super_arts.c, which were one file until the
 * super arts were split out. Both were `static` in pls03.c and are not meant to
 * be called from anywhere else. */

/* The command-to-shot conversion table both halves index into. Declared here
 * rather than in each .c so the split did not duplicate the declaration. */
extern const s16 cmdshot_conv_tbl[32];

/* Defined in pls03.c. The super arts and the specials both gate on it. */
s32 player_is_grounded_or_on_car(const PLW* wk);

/* Defined in pls03.c. Every super-art and special slot is checked against the
 * arcade-balance switch before it can be taken. */
bool is_blocked_by_arcade_switch(PLW* wk, s16 ix);

#endif // PLS03_INTERNAL_H
