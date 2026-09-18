/*
 * Internal declarations for the pass16 pattern scripts.
 *
 * pass16.c holds the dispatcher and Passive16_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS16_INTERNAL_H
#define PASS16_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive16_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive16_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive16_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive16_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive16_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive16_pattern_check_ex_jump_command_attack_term(PLW* wk, s16 next_menu, const JCA_Term_Args* a);
void passive16_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive16_pattern_normal_attack_2(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void passive16_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void passive16_pattern_wait_branch_unit_area(PLW* wk, s16 time, const Branch_Menu_Args* p);
void passive16_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive16_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void passive16_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void Passive16_0047(PLW* wk);
void Passive16_0048(PLW* wk);
void Passive16_0049(PLW* wk);
void Passive16_0050(PLW* wk);
void Passive16_0051(PLW* wk);
void Passive16_0052(PLW* wk);
void Passive16_0053(PLW* wk);
void Passive16_0054(PLW* wk);
void Passive16_0055(PLW* wk);
void Passive16_0056(PLW* wk);
void Passive16_0057(PLW* wk);
void Passive16_0058(PLW* wk);
void Passive16_0059(PLW* wk);
void Passive16_0060(PLW* wk);
void Passive16_0061(PLW* wk);
void Passive16_0062(PLW* wk);
void Passive16_0063(PLW* wk);
void Passive16_0064(PLW* wk);
void Passive16_0065(PLW* wk);
void Passive16_0066(PLW* wk);
void Passive16_0067(PLW* wk);
void Passive16_0068(PLW* wk);
void Passive16_0069(PLW* wk);
void Passive16_0070(PLW* wk);
void Passive16_0071(PLW* wk);
void Passive16_0072(PLW* wk);
void Passive16_0073(PLW* wk);
void Passive16_0074(PLW* wk);
void Passive16_0075(PLW* wk);
void Passive16_0076(PLW* wk);
void Passive16_0077(PLW* wk);
void Passive16_0078(PLW* wk);
void Passive16_0079(PLW* wk);
void Passive16_0080(PLW* wk);
void Passive16_0081(PLW* wk);
void Passive16_0082(PLW* wk);
void Passive16_0083(PLW* wk);
void Passive16_0084(PLW* wk);
void Passive16_0085(PLW* wk);
void Passive16_0086(PLW* wk);
void Passive16_0087(PLW* wk);
void Passive16_0088(PLW* wk);
void Passive16_0089(PLW* wk);
void Passive16_0090(PLW* wk);
void Passive16_0091(PLW* wk);
void Passive16_0092(PLW* wk);
void Passive16_0093(PLW* wk);
void Passive16_0094(PLW* wk);
void Passive16_0095(PLW* wk);
void Passive16_0096(PLW* wk);
void Passive16_0097(PLW* wk);
void Passive16_0098(PLW* wk);
void Passive16_0099(PLW* wk);
void Passive16_0100(PLW* wk);
void Passive16_0101(PLW* wk);
void Passive16_0102(PLW* wk);
void Passive16_0103(PLW* wk);
void Passive16_0104(PLW* wk);
void Passive16_0105(PLW* wk);
void Passive16_0106(PLW* wk);
void Passive16_0107(PLW* wk);
void Passive16_0108(PLW* wk);
void Passive16_0109(PLW* wk);
void Passive16_0110(PLW* wk);
void Passive16_0111(PLW* wk);
void Passive16_0112(PLW* wk);
void Passive16_0113(PLW* wk);
void Passive16_0114(PLW* wk);
void Passive16_0115(PLW* wk);
void Passive16_0116(PLW* wk);
void Passive16_0117(PLW* wk);
void Passive16_0118(PLW* wk);
void Passive16_0119(PLW* wk);
void Passive16_0120(PLW* wk);
void Passive16_0121(PLW* wk);
void Passive16_0122(PLW* wk);
void Passive16_0123(PLW* wk);
void Passive16_0124(PLW* wk);
void Passive16_0125(PLW* wk);
void Passive16_0126(PLW* wk);
void Passive16_0127(PLW* wk);
void Passive16_0128(PLW* wk);
void Passive16_0129(PLW* wk);
void Passive16_0130(PLW* wk);
void Passive16_0131(PLW* wk);
void Passive16_0132(PLW* wk);
void Passive16_0133(PLW* wk);
void Passive16_0134(PLW* wk);
void Passive16_0135(PLW* wk);
void Passive16_0136(PLW* wk);
void Passive16_0137(PLW* wk);
void Passive16_0138(PLW* wk);
void Passive16_0139(PLW* wk);
void Passive16_0140(PLW* wk);
void Passive16_0141(PLW* wk);
void Passive16_0142(PLW* wk);
void Passive16_0143(PLW* wk);
void Passive16_0144(PLW* wk);
void Passive16_0145(PLW* wk);
void Passive16_0146(PLW* wk);
void Passive16_0147(PLW* wk);
void Passive16_0148(PLW* wk);
void Passive16_0149(PLW* wk);
void Passive16_0150(PLW* wk);
void Passive16_0151(PLW* wk);
void Passive16_0152(PLW* wk);
void Passive16_0153(PLW* wk);
void Passive16_0154(PLW* wk);
void Passive16_0155(PLW* wk);
void Passive16_0156(PLW* wk);
void Passive16_0157(PLW* wk);
void Passive16_0158(PLW* wk);
void Passive16_0159(PLW* wk);
void Passive16_0160(PLW* wk);
void Passive16_0161(PLW* wk);
void Passive16_0162(PLW* wk);
void Passive16_0163(PLW* wk);
void Passive16_0164(PLW* wk);
void Passive16_0165(PLW* wk);
void Passive16_0166(PLW* wk);

#endif
