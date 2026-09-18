/*
 * Internal declarations for the pass17 pattern scripts.
 *
 * pass17.c holds the dispatcher and Passive17_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS17_INTERNAL_H
#define PASS17_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive17_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive17_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive17_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive17_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive17_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive17_pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void passive17_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive17_pattern_normal_attack_2(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void passive17_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive17_pattern_lever_on_normal_attack_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
);
void passive17_pattern_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void passive17_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void passive17_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void Passive17_0049(PLW* wk);
void Passive17_0050(PLW* wk);
void Passive17_0051(PLW* wk);
void Passive17_0052(PLW* wk);
void Passive17_0053(PLW* wk);
void Passive17_0054(PLW* wk);
void Passive17_0055(PLW* wk);
void Passive17_0056(PLW* wk);
void Passive17_0057(PLW* wk);
void Passive17_0058(PLW* wk);
void Passive17_0059(PLW* wk);
void Passive17_0060(PLW* wk);
void Passive17_0061(PLW* wk);
void Passive17_0062(PLW* wk);
void Passive17_0063(PLW* wk);
void Passive17_0064(PLW* wk);
void Passive17_0065(PLW* wk);
void Passive17_0066(PLW* wk);
void Passive17_0067(PLW* wk);
void Passive17_0068(PLW* wk);
void Passive17_0069(PLW* wk);
void Passive17_0070(PLW* wk);
void Passive17_0071(PLW* wk);
void Passive17_0072(PLW* wk);
void Passive17_0073(PLW* wk);
void Passive17_0074(PLW* wk);
void Passive17_0075(PLW* wk);
void Passive17_0076(PLW* wk);
void Passive17_0077(PLW* wk);
void Passive17_0078(PLW* wk);
void Passive17_0079(PLW* wk);
void Passive17_0080(PLW* wk);
void Passive17_0081(PLW* wk);
void Passive17_0082(PLW* wk);
void Passive17_0083(PLW* wk);
void Passive17_0084(PLW* wk);
void Passive17_0085(PLW* wk);
void Passive17_0086(PLW* wk);
void Passive17_0087(PLW* wk);
void Passive17_0088(PLW* wk);
void Passive17_0089(PLW* wk);
void Passive17_0090(PLW* wk);
void Passive17_0091(PLW* wk);
void Passive17_0092(PLW* wk);
void Passive17_0093(PLW* wk);
void Passive17_0094(PLW* wk);
void Passive17_0095(PLW* wk);
void Passive17_0096(PLW* wk);
void Passive17_0097(PLW* wk);
void Passive17_0098(PLW* wk);
void Passive17_0099(PLW* wk);
void Passive17_0100(PLW* wk);
void Passive17_0101(PLW* wk);
void Passive17_0102(PLW* wk);
void Passive17_0103(PLW* wk);
void Passive17_0104(PLW* wk);
void Passive17_0105(PLW* wk);
void Passive17_0106(PLW* wk);
void Passive17_0107(PLW* wk);
void Passive17_0108(PLW* wk);
void Passive17_0109(PLW* wk);
void Passive17_0110(PLW* wk);
void Passive17_0111(PLW* wk);
void Passive17_0112(PLW* wk);
void Passive17_0113(PLW* wk);
void Passive17_0114(PLW* wk);
void Passive17_0115(PLW* wk);
void Passive17_0116(PLW* wk);
void Passive17_0117(PLW* wk);
void Passive17_0118(PLW* wk);
void Passive17_0119(PLW* wk);
void Passive17_0120(PLW* wk);
void Passive17_0121(PLW* wk);
void Passive17_0122(PLW* wk);
void Passive17_0123(PLW* wk);
void Passive17_0124(PLW* wk);
void Passive17_0125(PLW* wk);
void Passive17_0126(PLW* wk);
void Passive17_0127(PLW* wk);
void Passive17_0128(PLW* wk);
void Passive17_0129(PLW* wk);
void Passive17_0130(PLW* wk);
void Passive17_0131(PLW* wk);
void Passive17_0132(PLW* wk);
void Passive17_0133(PLW* wk);
void Passive17_0134(PLW* wk);
void Passive17_0135(PLW* wk);
void Passive17_0136(PLW* wk);
void Passive17_0137(PLW* wk);
void Passive17_0138(PLW* wk);
void Passive17_0139(PLW* wk);
void Passive17_0140(PLW* wk);
void Passive17_0141(PLW* wk);
void Passive17_0142(PLW* wk);
void Passive17_0143(PLW* wk);
void Passive17_0144(PLW* wk);
void Passive17_0145(PLW* wk);
void Passive17_0146(PLW* wk);
void Passive17_0147(PLW* wk);
void Passive17_0148(PLW* wk);
void Passive17_0149(PLW* wk);
void Passive17_0150(PLW* wk);
void Passive17_0151(PLW* wk);
void Passive17_0152(PLW* wk);
void Passive17_0153(PLW* wk);
void Passive17_0154(PLW* wk);
void Passive17_0155(PLW* wk);
void Passive17_0156(PLW* wk);
void Passive17_0157(PLW* wk);
void Passive17_0158(PLW* wk);
void Passive17_0159(PLW* wk);
void Passive17_0160(PLW* wk);
void Passive17_0161(PLW* wk);
void Passive17_0162(PLW* wk);
void Passive17_0163(PLW* wk);
void Passive17_0164(PLW* wk);
void Passive17_0165(PLW* wk);
void Passive17_0166(PLW* wk);
void Passive17_0167(PLW* wk);
void Passive17_0168(PLW* wk);
void Passive17_0169(PLW* wk);
void Passive17_0170(PLW* wk);
void Passive17_0171(PLW* wk);
void Passive17_0172(PLW* wk);
void Passive17_0173(PLW* wk);
void Passive17_0174(PLW* wk);
void Passive17_0175(PLW* wk);
void Passive17_0176(PLW* wk);
void Passive17_0177(PLW* wk);
void Passive17_0178(PLW* wk);
void Passive17_0179(PLW* wk);
void Passive17_0180(PLW* wk);
void Passive17_0181(PLW* wk);

#endif
