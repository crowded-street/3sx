/*
 * Internal declarations for the pass10 pattern scripts.
 *
 * pass10.c holds the dispatcher and Passive10_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS10_INTERNAL_H
#define PASS10_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive10_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive10_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive10_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive10_pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void passive10_pattern_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b);
void passive10_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive10_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void Passive10_0050(PLW* wk);
void Passive10_0051(PLW* wk);
void Passive10_0052(PLW* wk);
void Passive10_0053(PLW* wk);
void Passive10_0054(PLW* wk);
void Passive10_0055(PLW* wk);
void Passive10_0056(PLW* wk);
void Passive10_0057(PLW* wk);
void Passive10_0058(PLW* wk);
void Passive10_0059(PLW* wk);
void Passive10_0060(PLW* wk);
void Passive10_0061(PLW* wk);
void Passive10_0062(PLW* wk);
void Passive10_0063(PLW* wk);
void Passive10_0064(PLW* wk);
void Passive10_0065(PLW* wk);
void Passive10_0066(PLW* wk);
void Passive10_0067(PLW* wk);
void Passive10_0068(PLW* wk);
void Passive10_0069(PLW* wk);
void Passive10_0070(PLW* wk);
void Passive10_0071(PLW* wk);
void Passive10_0072(PLW* wk);
void Passive10_0073(PLW* wk);
void Passive10_0074(PLW* wk);
void Passive10_0075(PLW* wk);
void Passive10_0076(PLW* wk);
void Passive10_0077(PLW* wk);
void Passive10_0078(PLW* wk);
void Passive10_0079(PLW* wk);
void Passive10_0080(PLW* wk);
void Passive10_0081(PLW* wk);
void Passive10_0082(PLW* wk);
void Passive10_0083(PLW* wk);
void Passive10_0084(PLW* wk);
void Passive10_0085(PLW* wk);
void Passive10_0086(PLW* wk);
void Passive10_0087(PLW* wk);
void Passive10_0088(PLW* wk);
void Passive10_0089(PLW* wk);
void Passive10_0090(PLW* wk);
void Passive10_0091(PLW* wk);
void Passive10_0092(PLW* wk);
void Passive10_0093(PLW* wk);
void Passive10_0094(PLW* wk);
void Passive10_0095(PLW* wk);
void Passive10_0096(PLW* wk);
void Passive10_0097(PLW* wk);
void Passive10_0098(PLW* wk);
void Passive10_0099(PLW* wk);
void Passive10_0100(PLW* wk);
void Passive10_0101(PLW* wk);
void Passive10_0102(PLW* wk);
void Passive10_0103(PLW* wk);
void Passive10_0104(PLW* wk);
void Passive10_0105(PLW* wk);
void Passive10_0106(PLW* wk);
void Passive10_0107(PLW* wk);
void Passive10_0108(PLW* wk);
void Passive10_0109(PLW* wk);
void Passive10_0110(PLW* wk);
void Passive10_0111(PLW* wk);
void Passive10_0112(PLW* wk);
void Passive10_0113(PLW* wk);
void Passive10_0114(PLW* wk);
void Passive10_0115(PLW* wk);
void Passive10_0116(PLW* wk);
void Passive10_0117(PLW* wk);
void Passive10_0118(PLW* wk);
void Passive10_0119(PLW* wk);
void Passive10_0120(PLW* wk);
void Passive10_0121(PLW* wk);
void Passive10_0122(PLW* wk);
void Passive10_0123(PLW* wk);
void Passive10_0124(PLW* wk);
void Passive10_0125(PLW* wk);
void Passive10_0126(PLW* wk);
void Passive10_0127(PLW* wk);
void Passive10_0128(PLW* wk);
void Passive10_0129(PLW* wk);
void Passive10_0130(PLW* wk);
void Passive10_0131(PLW* wk);
void Passive10_0132(PLW* wk);
void Passive10_0133(PLW* wk);
void Passive10_0134(PLW* wk);
void Passive10_0135(PLW* wk);
void Passive10_0136(PLW* wk);
void Passive10_0137(PLW* wk);

#endif
