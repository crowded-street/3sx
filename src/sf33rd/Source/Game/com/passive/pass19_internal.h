/*
 * Internal declarations for the pass19 pattern scripts.
 *
 * pass19.c holds the dispatcher and Passive19_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS19_INTERNAL_H
#define PASS19_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive19_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive19_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive19_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive19_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive19_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive19_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive19_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive19_pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void passive19_pattern_em_term_branch_unit_area(PLW* wk, const EM_Term_Params* p, const Branch_Menu_Args* p_b);
void passive19_pattern_normal_attack_2(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void passive19_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive19_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void Passive19_0058(PLW* wk);
void Passive19_0059(PLW* wk);
void Passive19_0060(PLW* wk);
void Passive19_0061(PLW* wk);
void Passive19_0062(PLW* wk);
void Passive19_0063(PLW* wk);
void Passive19_0064(PLW* wk);
void Passive19_0065(PLW* wk);
void Passive19_0066(PLW* wk);
void Passive19_0067(PLW* wk);
void Passive19_0068(PLW* wk);
void Passive19_0069(PLW* wk);
void Passive19_0070(PLW* wk);
void Passive19_0071(PLW* wk);
void Passive19_0072(PLW* wk);
void Passive19_0073(PLW* wk);
void Passive19_0074(PLW* wk);
void Passive19_0075(PLW* wk);
void Passive19_0076(PLW* wk);
void Passive19_0077(PLW* wk);
void Passive19_0078(PLW* wk);
void Passive19_0079(PLW* wk);
void Passive19_0080(PLW* wk);
void Passive19_0081(PLW* wk);
void Passive19_0082(PLW* wk);
void Passive19_0083(PLW* wk);
void Passive19_0084(PLW* wk);
void Passive19_0085(PLW* wk);
void Passive19_0086(PLW* wk);
void Passive19_0087(PLW* wk);
void Passive19_0088(PLW* wk);
void Passive19_0089(PLW* wk);
void Passive19_0090(PLW* wk);
void Passive19_0091(PLW* wk);
void Passive19_0092(PLW* wk);
void Passive19_0093(PLW* wk);
void Passive19_0094(PLW* wk);
void Passive19_0095(PLW* wk);
void Passive19_0096(PLW* wk);
void Passive19_0097(PLW* wk);
void Passive19_0098(PLW* wk);
void Passive19_0099(PLW* wk);
void Passive19_0100(PLW* wk);
void Passive19_0101(PLW* wk);
void Passive19_0102(PLW* wk);
void Passive19_0103(PLW* wk);
void Passive19_0104(PLW* wk);
void Passive19_0105(PLW* wk);
void Passive19_0106(PLW* wk);
void Passive19_0107(PLW* wk);
void Passive19_0108(PLW* wk);
void Passive19_0109(PLW* wk);
void Passive19_0110(PLW* wk);
void Passive19_0111(PLW* wk);
void Passive19_0112(PLW* wk);
void Passive19_0113(PLW* wk);
void Passive19_0114(PLW* wk);
void Passive19_0115(PLW* wk);
void Passive19_0116(PLW* wk);
void Passive19_0117(PLW* wk);
void Passive19_0118(PLW* wk);
void Passive19_0119(PLW* wk);
void Passive19_0120(PLW* wk);
void Passive19_0121(PLW* wk);
void Passive19_0122(PLW* wk);
void Passive19_0123(PLW* wk);
void Passive19_0124(PLW* wk);
void Passive19_0125(PLW* wk);
void Passive19_0126(PLW* wk);
void Passive19_0127(PLW* wk);
void Passive19_0128(PLW* wk);
void Passive19_0129(PLW* wk);
void Passive19_0130(PLW* wk);
void Passive19_0131(PLW* wk);
void Passive19_0132(PLW* wk);
void Passive19_0133(PLW* wk);
void Passive19_0134(PLW* wk);
void Passive19_0135(PLW* wk);
void Passive19_0136(PLW* wk);
void Passive19_0137(PLW* wk);
void Passive19_0138(PLW* wk);
void Passive19_0139(PLW* wk);
void Passive19_0140(PLW* wk);
void Passive19_0141(PLW* wk);
void Passive19_0142(PLW* wk);
void Passive19_0143(PLW* wk);
void Passive19_0144(PLW* wk);
void Passive19_0145(PLW* wk);
void Passive19_0146(PLW* wk);
void Passive19_0147(PLW* wk);
void Passive19_0148(PLW* wk);
void Passive19_0149(PLW* wk);
void Passive19_0150(PLW* wk);
void Passive19_0151(PLW* wk);
void Passive19_0152(PLW* wk);
void Passive19_0153(PLW* wk);
void Passive19_0154(PLW* wk);
void Passive19_0155(PLW* wk);
void Passive19_0156(PLW* wk);
void Passive19_0157(PLW* wk);
void Passive19_0158(PLW* wk);
void Passive19_0159(PLW* wk);
void Passive19_0160(PLW* wk);
void Passive19_0161(PLW* wk);
void Passive19_0162(PLW* wk);
void Passive19_0163(PLW* wk);
void Passive19_0164(PLW* wk);
void Passive19_0165(PLW* wk);
void Passive19_0166(PLW* wk);
void Passive19_0167(PLW* wk);
void Passive19_0168(PLW* wk);
void Passive19_0169(PLW* wk);
void Passive19_0170(PLW* wk);
void Passive19_0171(PLW* wk);
void Passive19_0172(PLW* wk);
void Passive19_0173(PLW* wk);
void Passive19_0174(PLW* wk);
void Passive19_0175(PLW* wk);

#endif
