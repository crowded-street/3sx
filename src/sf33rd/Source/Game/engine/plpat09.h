#ifndef PLPAT09_H
#define PLPAT09_H

#include "structs.h"
#include "types.h"

/* One row of a tenguiwa position table: three old_pos components, the two screen
 * deltas and the direction. Named here so the shared placement can take either
 * table without the row extent becoming a literal in the .c file. */
typedef const s16 TenguiwaPosRow[6];

/* What distinguishes one tenguiwa set from the other, in the order the shared
 * placement used to take them as arguments. */
typedef struct TenguiwaSet {
    const u8* tengu;
    s16 rock_count;
    TenguiwaPosRow* pos;
    s16 last_slot;
} TenguiwaSet;

void pl09_extra_attack(PLW* wk);
s32 set_tenguiwa(PLW* wk, u8 data);

#endif
