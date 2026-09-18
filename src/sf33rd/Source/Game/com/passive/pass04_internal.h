/*
 * Internal declarations for the pass04 pattern scripts.
 *
 * pass04.c holds the dispatcher and Passive04_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS04_INTERNAL_H
#define PASS04_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive04_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive04_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive04_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive04_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive04_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive04_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive04_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive04_pattern_approach_walk_wait_get_up_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void passive04_pattern_wait_get_up_command_attack(PLW* wk, const Command_Attack_Args* p);
void Passive04_0062(PLW* wk);
void Passive04_0063(PLW* wk);
void Passive04_0064(PLW* wk);
void Passive04_0065(PLW* wk);
void Passive04_0066(PLW* wk);
void Passive04_0067(PLW* wk);
void Passive04_0068(PLW* wk);
void Passive04_0069(PLW* wk);
void Passive04_0070(PLW* wk);
void Passive04_0071(PLW* wk);
void Passive04_0072(PLW* wk);
void Passive04_0073(PLW* wk);
void Passive04_0074(PLW* wk);
void Passive04_0075(PLW* wk);
void Passive04_0076(PLW* wk);
void Passive04_0077(PLW* wk);
void Passive04_0078(PLW* wk);
void Passive04_0079(PLW* wk);
void Passive04_0080(PLW* wk);
void Passive04_0081(PLW* wk);
void Passive04_0082(PLW* wk);
void Passive04_0083(PLW* wk);
void Passive04_0084(PLW* wk);
void Passive04_0085(PLW* wk);
void Passive04_0086(PLW* wk);
void Passive04_0087(PLW* wk);
void Passive04_0088(PLW* wk);
void Passive04_0089(PLW* wk);
void Passive04_0090(PLW* wk);
void Passive04_0091(PLW* wk);
void Passive04_0092(PLW* wk);
void Passive04_0093(PLW* wk);
void Passive04_0094(PLW* wk);
void Passive04_0095(PLW* wk);
void Passive04_0096(PLW* wk);
void Passive04_0097(PLW* wk);
void Passive04_0098(PLW* wk);
void Passive04_0099(PLW* wk);
void Passive04_0100(PLW* wk);
void Passive04_0101(PLW* wk);
void Passive04_0102(PLW* wk);
void Passive04_0103(PLW* wk);
void Passive04_0104(PLW* wk);
void Passive04_0105(PLW* wk);
void Passive04_0106(PLW* wk);
void Passive04_0107(PLW* wk);
void Passive04_0108(PLW* wk);
void Passive04_0109(PLW* wk);
void Passive04_0110(PLW* wk);
void Passive04_0111(PLW* wk);
void Passive04_0112(PLW* wk);
void Passive04_0113(PLW* wk);
void Passive04_0114(PLW* wk);
void Passive04_0115(PLW* wk);
void Passive04_0116(PLW* wk);
void Passive04_0117(PLW* wk);
void Passive04_0118(PLW* wk);
void Passive04_0119(PLW* wk);
void Passive04_0120(PLW* wk);
void Passive04_0121(PLW* wk);
void Passive04_0122(PLW* wk);
void Passive04_0123(PLW* wk);
void Passive04_0124(PLW* wk);
void Passive04_0125(PLW* wk);
void Passive04_0126(PLW* wk);
void Passive04_0127(PLW* wk);
void Passive04_0128(PLW* wk);
void Passive04_0129(PLW* wk);
void Passive04_0130(PLW* wk);
void Passive04_0131(PLW* wk);
void Passive04_0132(PLW* wk);
void Passive04_0133(PLW* wk);
void Passive04_0134(PLW* wk);
void Passive04_0135(PLW* wk);
void Passive04_0136(PLW* wk);
void Passive04_0137(PLW* wk);
void Passive04_0138(PLW* wk);
void Passive04_0139(PLW* wk);
void Passive04_0140(PLW* wk);
void Passive04_0141(PLW* wk);

#endif
