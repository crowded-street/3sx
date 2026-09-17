#ifndef PLPDM_INTERNAL_H
#define PLPDM_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between plpdm.c and plpdm_states.c, which were one 1666-line file.
 * Every name here already had external linkage - the dispatch table in plpdm.c
 * reaches the states by name - so the split widened nothing. */

/* The const tables the states index into, all defined in plpdm.c and all
 * already non-static. Declared here so the split did not have to move or touch
 * a single table. */
extern const s16 dir32_guard_air[32];
extern const s16 ris_data_table[4][4];
extern const s16 dm17_to_nm23_change[20];
extern const s16 hok_table[8];
extern const AS dm_reaction_table[115];
extern const u16 exdm_ix_data[2][20][5];
extern void (*const plpdm_lv_00[32])(PLW* wk);

/* Defined in plpdm_states.c, named by the dispatch table in plpdm.c. */
void Damage_00000(PLW* wk);
void Damage_01000(PLW* wk);
void Damage_04000(PLW* wk);
void Damage_07000(PLW* wk);
void Damage_12000(PLW* wk);
void Damage_14000(PLW* wk);
void Damage_16000(PLW* wk);
void Damage_17000(PLW* wk);
void Damage_18000(PLW* wk);
void Damage_19000(PLW* wk);
void Damage_20000(PLW* wk);
void Damage_21000(PLW* wk);
void Damage_23000(PLW* wk);
void Damage_24000(PLW* wk);
void Damage_25000(PLW* wk);
void Damage_26000(PLW* wk);
void Damage_27000(PLW* wk);
void Damage_28000(PLW* wk);
void Damage_29000(PLW* wk);
void Damage_30000(PLW* wk);
void Damage_31000(PLW* wk);
s32 remake_initial_speeds(WORK* wk);

/* Defined in plpdm.c; the damage states are all built out of these. */
void first_flight_union(PLW* wk, s16 num, s16 dv);
void first_TtktV_union(PLW* wk, s16 num, s16 dv);
void buttobi_chakuchi_cg_type_check(PLW* wk);
void buttobi_add_y_check(PLW* wk);
void setup_smoke_type(PLW* wk);
void add_dm_step_tbl(PLW* wk, s8 flag);
void check_dmpat_to_dmpat(PLW* wk);
void set_dm_hos_flag_sky(PLW* wk);
void set_dm_hos_flag_grd(PLW* wk);
void get_sky_dm_timer(PLW* wk);
s32 setup_kuuchuu_nmdm(PLW* wk);

#endif // PLPDM_INTERNAL_H
