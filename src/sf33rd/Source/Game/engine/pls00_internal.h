#ifndef PLS00_INTERNAL_H
#define PLS00_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between pls00.c and the two state files split out of it. These were
 * all file-scope in pls00.c, which was 1535 lines; nothing here is meant to be
 * called from outside that group. Every one already had external linkage - the
 * split widened nothing. */

/* Defined in pls00.c. The dispatch tables at the foot of that file are indexed
 * by routine_no and call into both state files. */
extern const s8 lvdir_conv[4];
extern void (*const plpdm_xxxxx[32])(PLW* wk);
extern void (*const plpdm_xxxxx_arcade[39])(PLW* wk);

/* Defined in pls00.c; the normal and damage states both enter states through
 * these. */
void TO_nm_01000(WORK* wk);
void TO_nm_09000(WORK* wk);
void TO_nm_18000_01(WORK* wk);
void TO_nm_36000(WORK* wk);
void TO_nm_37000(WORK* wk);
void TO_nm_38000(WORK* wk);

/* Defined in pls00.c. Every normal state offers the player a cancel. */
s32 check_cg_cancel_data(PLW* wk);

/* Defined in pls00_normal_states.c; pls00.c's table and the damage states
 * both reach it. */
void jumping_cg_type_check(PLW* wk);

/* Defined in pls00_normal_states.c, named by plpnm_xxxxx in pls00.c. */
void nm_00000(PLW* wk);
void nm_01000(PLW* wk);
void nm_02000(PLW* wk);
void nm_03000(PLW* wk);
void nm_05000(PLW* wk);
void nm_07000(PLW* wk);
void nm_08000(PLW* wk);
void nm_09000(PLW* wk);
void nm_10000(PLW* wk);
void nm_11000(PLW* wk);
void nm_13000(PLW* wk);
void nm_16000(PLW* wk);
void nm_17000(PLW* wk);
void nm_18000(PLW* wk);
void nm_27000(PLW* wk);
void nm_29000(PLW* wk);
void nm_31000(PLW* wk);
void nm_34000(PLW* wk);
void nm_36000(PLW* wk);
void nm_37000(PLW* wk);
void nm_38000(PLW* wk);
void nm_39000(PLW* wk);
void nm_40000(PLW* wk);
void nm_42000(PLW* wk);
void nm_45000(PLW* wk);
void nm_47000(PLW* wk);
void nm_48000(PLW* wk);
void nm_49000(PLW* wk);
void nm_51000(PLW* wk);
void nm_52000(PLW* wk);
void nm_55000(PLW* wk);
void nm_57000(PLW* wk);

/* Defined in pls00_damage_states.c, named by plpdm_xxxxx in pls00.c. */
void process_damage(PLW* wk);
void process_catch(PLW* wk);
void process_caught(PLW* wk);
void dm_00000(PLW* wk);
void dm_04000(PLW* wk);
void dm_08000(PLW* wk);
void dm_17000(PLW* wk);
void dm_18000(PLW* wk);
void dm_25000(PLW* wk);
void dm_32000(PLW* wk);
void dm_33000(PLW* wk);

#endif // PLS00_INTERNAL_H
