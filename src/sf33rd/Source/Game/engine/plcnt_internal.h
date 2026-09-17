#ifndef PLCNT_INTERNAL_H
#define PLCNT_INTERNAL_H

#include "constants.h"
#include "structs.h"
#include "types.h"

/* Shared between plcnt.c and plcnt_setup.c, which was split out of it. Every
 * name here was already file-scope with external linkage in plcnt.c; nothing is
 * meant to be reached from outside the pair, and the split widened nothing. */

/* Defined in plcnt.c, read by the per-character setup. */
extern const s8 weight_lv_table[20];
extern const s16 kage_base[20][2];
extern const SA_DATA super_arts_data[20][4];
extern const SA_DATA super_arts_DATA[20][4];
extern const s16 pl_piyo_tbl[NUM_CHARS];
extern const s32 pl_nr_piyo_tbl[NUM_CHARS];

/* Defined in plcnt_setup.c; the setup entry points call each other, and
 * plcnt.c's own round setup calls the first two. */
void setup_base_and_other_data(void);
void setup_any_data(void);
void set_base_data(PLW* wk, s16 ix);
void set_base_data_tiny(PLW* wk);
void setup_other_data(PLW* wk);
s16 remake_sa_store_max(s16 ix, s16 store_max);
s16 remake_sa_gauge_len(s16 ix, s16 gauge_len);

#endif // PLCNT_INTERNAL_H
