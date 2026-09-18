/*
 * Internal declarations for the pass05 pattern scripts.
 *
 * pass05.c holds the dispatcher and Passive05_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS05_INTERNAL_H
#define PASS05_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive05_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive05_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive05_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive05_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void passive05_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive05_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive05_pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void Passive05_0070(PLW* wk);
void Passive05_0071(PLW* wk);
void Passive05_0072(PLW* wk);
void Passive05_0073(PLW* wk);
void Passive05_0074(PLW* wk);
void Passive05_0075(PLW* wk);
void Passive05_0076(PLW* wk);
void Passive05_0077(PLW* wk);
void Passive05_0078(PLW* wk);
void Passive05_0079(PLW* wk);
void Passive05_0080(PLW* wk);
void Passive05_0081(PLW* wk);
void Passive05_0082(PLW* wk);
void Passive05_0083(PLW* wk);
void Passive05_0084(PLW* wk);
void Passive05_0085(PLW* wk);
void Passive05_0086(PLW* wk);
void Passive05_0087(PLW* wk);
void Passive05_0088(PLW* wk);
void Passive05_0089(PLW* wk);
void Passive05_0090(PLW* wk);
void Passive05_0091(PLW* wk);
void Passive05_0092(PLW* wk);
void Passive05_0093(PLW* wk);
void Passive05_0094(PLW* wk);
void Passive05_0095(PLW* wk);
void Passive05_0096(PLW* wk);
void Passive05_0097(PLW* wk);
void Passive05_0098(PLW* wk);
void Passive05_0099(PLW* wk);
void Passive05_0100(PLW* wk);
void Passive05_0101(PLW* wk);

#endif
