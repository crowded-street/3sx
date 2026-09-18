/*
 * Internal declarations for the pass15 pattern scripts.
 *
 * pass15.c holds the dispatcher and Passive15_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS15_INTERNAL_H
#define PASS15_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive15_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive15_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive15_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive15_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive15_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive15_pattern_normal_attack_2(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void passive15_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive15_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive15_pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void passive15_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void passive15_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void Passive15_0060(PLW* wk);
void Passive15_0061(PLW* wk);
void Passive15_0062(PLW* wk);
void Passive15_0063(PLW* wk);
void Passive15_0064(PLW* wk);
void Passive15_0065(PLW* wk);
void Passive15_0066(PLW* wk);
void Passive15_0067(PLW* wk);
void Passive15_0068(PLW* wk);
void Passive15_0069(PLW* wk);
void Passive15_0070(PLW* wk);
void Passive15_0071(PLW* wk);
void Passive15_0072(PLW* wk);
void Passive15_0073(PLW* wk);
void Passive15_0074(PLW* wk);
void Passive15_0075(PLW* wk);
void Passive15_0076(PLW* wk);
void Passive15_0077(PLW* wk);
void Passive15_0078(PLW* wk);
void Passive15_0079(PLW* wk);
void Passive15_0080(PLW* wk);
void Passive15_0081(PLW* wk);
void Passive15_0082(PLW* wk);
void Passive15_0083(PLW* wk);
void Passive15_0084(PLW* wk);
void Passive15_0085(PLW* wk);
void Passive15_0086(PLW* wk);
void Passive15_0087(PLW* wk);
void Passive15_0088(PLW* wk);
void Passive15_0089(PLW* wk);
void Passive15_0090(PLW* wk);
void Passive15_0091(PLW* wk);
void Passive15_0092(PLW* wk);
void Passive15_0093(PLW* wk);
void Passive15_0094(PLW* wk);
void Passive15_0095(PLW* wk);
void Passive15_0096(PLW* wk);
void Passive15_0097(PLW* wk);
void Passive15_0098(PLW* wk);
void Passive15_0099(PLW* wk);
void Passive15_0100(PLW* wk);
void Passive15_0101(PLW* wk);
void Passive15_0102(PLW* wk);
void Passive15_0103(PLW* wk);
void Passive15_0104(PLW* wk);
void Passive15_0105(PLW* wk);
void Passive15_0106(PLW* wk);
void Passive15_0107(PLW* wk);
void Passive15_0108(PLW* wk);
void Passive15_0109(PLW* wk);
void Passive15_0110(PLW* wk);
void Passive15_0111(PLW* wk);
void Passive15_0112(PLW* wk);
void Passive15_0113(PLW* wk);
void Passive15_0114(PLW* wk);
void Passive15_0115(PLW* wk);
void Passive15_0116(PLW* wk);
void Passive15_0117(PLW* wk);
void Passive15_0118(PLW* wk);
void Passive15_0119(PLW* wk);
void Passive15_0120(PLW* wk);
void Passive15_0121(PLW* wk);
void Passive15_0122(PLW* wk);
void Passive15_0123(PLW* wk);
void Passive15_0124(PLW* wk);
void Passive15_0125(PLW* wk);
void Passive15_0126(PLW* wk);
void Passive15_0127(PLW* wk);
void Passive15_0128(PLW* wk);
void Passive15_0129(PLW* wk);
void Passive15_0130(PLW* wk);
void Passive15_0131(PLW* wk);
void Passive15_0132(PLW* wk);
void Passive15_0133(PLW* wk);
void Passive15_0134(PLW* wk);
void Passive15_0135(PLW* wk);
void Passive15_0136(PLW* wk);
void Passive15_0137(PLW* wk);
void Passive15_0138(PLW* wk);
void Passive15_0139(PLW* wk);
void Passive15_0140(PLW* wk);
void Passive15_0141(PLW* wk);
void Passive15_0142(PLW* wk);
void Passive15_0143(PLW* wk);
void Passive15_0144(PLW* wk);
void Passive15_0145(PLW* wk);
void Passive15_0146(PLW* wk);
void Passive15_0147(PLW* wk);
void Passive15_0148(PLW* wk);
void Passive15_0149(PLW* wk);
void Passive15_0150(PLW* wk);
void Passive15_0151(PLW* wk);
void Passive15_0152(PLW* wk);
void Passive15_0153(PLW* wk);
void Passive15_0154(PLW* wk);
void Passive15_0155(PLW* wk);
void Passive15_0156(PLW* wk);
void Passive15_0157(PLW* wk);
void Passive15_0158(PLW* wk);
void Passive15_0159(PLW* wk);
void Passive15_0160(PLW* wk);
void Passive15_0161(PLW* wk);
void Passive15_0162(PLW* wk);
void Passive15_0163(PLW* wk);

#endif
