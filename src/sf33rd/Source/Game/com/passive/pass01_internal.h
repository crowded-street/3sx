/*
 * Internal declarations for the pass01 pattern scripts.
 *
 * pass01.c holds the dispatcher and Passive01_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS01_INTERNAL_H
#define PASS01_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive01_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive01_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive01_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive01_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive01_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive01_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive01_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive01_pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void passive01_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive01_pattern_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void passive01_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void passive01_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void passive01_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void Passive01_0048(PLW* wk);
void Passive01_0049(PLW* wk);
void Passive01_0050(PLW* wk);
void Passive01_0051(PLW* wk);
void Passive01_0052(PLW* wk);
void Passive01_0053(PLW* wk);
void Passive01_0054(PLW* wk);
void Passive01_0055(PLW* wk);
void Passive01_0056(PLW* wk);
void Passive01_0057(PLW* wk);
void Passive01_0058(PLW* wk);
void Passive01_0059(PLW* wk);
void Passive01_0060(PLW* wk);
void Passive01_0061(PLW* wk);
void Passive01_0062(PLW* wk);
void Passive01_0063(PLW* wk);
void Passive01_0064(PLW* wk);
void Passive01_0065(PLW* wk);
void Passive01_0066(PLW* wk);
void Passive01_0067(PLW* wk);
void Passive01_0068(PLW* wk);
void Passive01_0069(PLW* wk);
void Passive01_0070(PLW* wk);
void Passive01_0071(PLW* wk);
void Passive01_0072(PLW* wk);
void Passive01_0073(PLW* wk);
void Passive01_0074(PLW* wk);
void Passive01_0075(PLW* wk);
void Passive01_0076(PLW* wk);
void Passive01_0077(PLW* wk);
void Passive01_0078(PLW* wk);
void Passive01_0079(PLW* wk);
void Passive01_0080(PLW* wk);
void Passive01_0081(PLW* wk);
void Passive01_0082(PLW* wk);
void Passive01_0083(PLW* wk);
void Passive01_0084(PLW* wk);
void Passive01_0085(PLW* wk);
void Passive01_0086(PLW* wk);
void Passive01_0087(PLW* wk);
void Passive01_0088(PLW* wk);
void Passive01_0089(PLW* wk);
void Passive01_0090(PLW* wk);
void Passive01_0091(PLW* wk);
void Passive01_0092(PLW* wk);
void Passive01_0093(PLW* wk);
void Passive01_0094(PLW* wk);
void Passive01_0095(PLW* wk);
void Passive01_0096(PLW* wk);
void Passive01_0097(PLW* wk);
void Passive01_0098(PLW* wk);
void Passive01_0099(PLW* wk);
void Passive01_0100(PLW* wk);
void Passive01_0101(PLW* wk);
void Passive01_0102(PLW* wk);
void Passive01_0103(PLW* wk);
void Passive01_0104(PLW* wk);
void Passive01_0105(PLW* wk);
void Passive01_0106(PLW* wk);
void Passive01_0107(PLW* wk);
void Passive01_0108(PLW* wk);
void Passive01_0109(PLW* wk);
void Passive01_0110(PLW* wk);
void Passive01_0111(PLW* wk);
void Passive01_0112(PLW* wk);
void Passive01_0113(PLW* wk);
void Passive01_0114(PLW* wk);
void Passive01_0115(PLW* wk);
void Passive01_0116(PLW* wk);
void Passive01_0117(PLW* wk);
void Passive01_0118(PLW* wk);
void Passive01_0119(PLW* wk);
void Passive01_0120(PLW* wk);
void Passive01_0121(PLW* wk);
void Passive01_0122(PLW* wk);
void Passive01_0123(PLW* wk);
void Passive01_0124(PLW* wk);
void Passive01_0125(PLW* wk);
void Passive01_0126(PLW* wk);
void Passive01_0127(PLW* wk);
void Passive01_0128(PLW* wk);
void Passive01_0129(PLW* wk);
void Passive01_0130(PLW* wk);
void Passive01_0131(PLW* wk);
void Passive01_0132(PLW* wk);
void Passive01_0133(PLW* wk);
void Passive01_0134(PLW* wk);
void Passive01_0135(PLW* wk);
void Passive01_0136(PLW* wk);
void Passive01_0137(PLW* wk);
void Passive01_0138(PLW* wk);
void Passive01_0139(PLW* wk);
void Passive01_0140(PLW* wk);
void Passive01_0141(PLW* wk);
void Passive01_0142(PLW* wk);
void Passive01_0143(PLW* wk);
void Passive01_0144(PLW* wk);
void Passive01_0145(PLW* wk);
void Passive01_0146(PLW* wk);
void Passive01_0147(PLW* wk);
void Passive01_0148(PLW* wk);
void Passive01_0149(PLW* wk);
void Passive01_0150(PLW* wk);
void Passive01_0151(PLW* wk);
void Passive01_0152(PLW* wk);
void Passive01_0153(PLW* wk);
void Passive01_0154(PLW* wk);
void Passive01_0155(PLW* wk);
void Passive01_0156(PLW* wk);
void Passive01_0157(PLW* wk);
void Passive01_0158(PLW* wk);
void Passive01_0159(PLW* wk);
void Passive01_0160(PLW* wk);
void Passive01_0161(PLW* wk);
void Passive01_0162(PLW* wk);
void Passive01_0163(PLW* wk);
void Passive01_0164(PLW* wk);
void Passive01_0165(PLW* wk);
void Passive01_0166(PLW* wk);
void Passive01_0167(PLW* wk);
void Passive01_0168(PLW* wk);
void Passive01_0169(PLW* wk);
void Passive01_0170(PLW* wk);
void Passive01_0171(PLW* wk);
void Passive01_0172(PLW* wk);
void Passive01_0173(PLW* wk);
void Passive01_0174(PLW* wk);
void Passive01_0175(PLW* wk);

#endif
