/*
 * Internal declarations for the pass02 pattern scripts.
 *
 * pass02.c holds the dispatcher and Passive02_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS02_INTERNAL_H
#define PASS02_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive02_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive02_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive02_pattern_sa_term(PLW* wk, const SA_Term_Args* p);
void passive02_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive02_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive02_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive02_pattern_check_boss_next_another_menu(PLW* wk, u16 next_menu);
void passive02_pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void passive02_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void Passive02_0065(PLW* wk);
void Passive02_0066(PLW* wk);
void Passive02_0067(PLW* wk);
void Passive02_0068(PLW* wk);
void Passive02_0069(PLW* wk);
void Passive02_0070(PLW* wk);
void Passive02_0071(PLW* wk);
void Passive02_0072(PLW* wk);
void Passive02_0073(PLW* wk);
void Passive02_0074(PLW* wk);
void Passive02_0075(PLW* wk);
void Passive02_0076(PLW* wk);
void Passive02_0077(PLW* wk);
void Passive02_0078(PLW* wk);
void Passive02_0079(PLW* wk);
void Passive02_0080(PLW* wk);
void Passive02_0081(PLW* wk);
void Passive02_0082(PLW* wk);
void Passive02_0083(PLW* wk);
void Passive02_0084(PLW* wk);
void Passive02_0085(PLW* wk);
void Passive02_0086(PLW* wk);
void Passive02_0087(PLW* wk);
void Passive02_0088(PLW* wk);
void Passive02_0089(PLW* wk);
void Passive02_0090(PLW* wk);
void Passive02_0091(PLW* wk);
void Passive02_0092(PLW* wk);
void Passive02_0093(PLW* wk);
void Passive02_0094(PLW* wk);
void Passive02_0095(PLW* wk);
void Passive02_0096(PLW* wk);
void Passive02_0097(PLW* wk);
void Passive02_0098(PLW* wk);
void Passive02_0099(PLW* wk);
void Passive02_0100(PLW* wk);
void Passive02_0101(PLW* wk);
void Passive02_0102(PLW* wk);
void Passive02_0103(PLW* wk);
void Passive02_0104(PLW* wk);
void Passive02_0105(PLW* wk);
void Passive02_0106(PLW* wk);
void Passive02_0107(PLW* wk);
void Passive02_0108(PLW* wk);
void Passive02_0109(PLW* wk);
void Passive02_0110(PLW* wk);
void Passive02_0111(PLW* wk);
void Passive02_0112(PLW* wk);
void Passive02_0113(PLW* wk);
void Passive02_0114(PLW* wk);
void Passive02_0115(PLW* wk);
void Passive02_0116(PLW* wk);
void Passive02_0117(PLW* wk);
void Passive02_0118(PLW* wk);
void Passive02_0119(PLW* wk);
void Passive02_0120(PLW* wk);
void Passive02_0121(PLW* wk);
void Passive02_0122(PLW* wk);
void Passive02_0123(PLW* wk);
void Passive02_0124(PLW* wk);
void Passive02_0125(PLW* wk);
void Passive02_0126(PLW* wk);
void Passive02_0127(PLW* wk);
void Passive02_0128(PLW* wk);
void Passive02_0129(PLW* wk);
void Passive02_0130(PLW* wk);
void Passive02_0131(PLW* wk);
void Passive02_0132(PLW* wk);

#endif
