/*
 * Internal declarations for the pass18 pattern scripts.
 *
 * pass18.c holds the dispatcher and Passive18_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS18_INTERNAL_H
#define PASS18_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive18_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive18_pattern_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void passive18_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive18_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive18_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void passive18_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive18_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void passive18_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void passive18_pattern_check_ex_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void passive18_pattern_normal_attack_2(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void passive18_pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void passive18_pattern_wait_get_up_pierce_on_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void passive18_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void passive18_pattern_approach_walk_em_term_normal_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever_data
);
void passive18_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a);
void passive18_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive18_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive18_pattern_approach_walk(PLW* wk, s16 target_pos);
void passive18_pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void Passive18_0040(PLW* wk);
void Passive18_0041(PLW* wk);
void Passive18_0042(PLW* wk);
void Passive18_0043(PLW* wk);
void Passive18_0044(PLW* wk);
void Passive18_0045(PLW* wk);
void Passive18_0046(PLW* wk);
void Passive18_0047(PLW* wk);
void Passive18_0048(PLW* wk);
void Passive18_0049(PLW* wk);
void Passive18_0050(PLW* wk);
void Passive18_0051(PLW* wk);
void Passive18_0052(PLW* wk);
void Passive18_0053(PLW* wk);
void Passive18_0054(PLW* wk);
void Passive18_0055(PLW* wk);
void Passive18_0056(PLW* wk);
void Passive18_0057(PLW* wk);
void Passive18_0058(PLW* wk);
void Passive18_0059(PLW* wk);
void Passive18_0060(PLW* wk);
void Passive18_0061(PLW* wk);
void Passive18_0062(PLW* wk);
void Passive18_0063(PLW* wk);
void Passive18_0064(PLW* wk);
void Passive18_0065(PLW* wk);
void Passive18_0066(PLW* wk);
void Passive18_0067(PLW* wk);
void Passive18_0068(PLW* wk);
void Passive18_0069(PLW* wk);
void Passive18_0070(PLW* wk);
void Passive18_0071(PLW* wk);
void Passive18_0072(PLW* wk);
void Passive18_0073(PLW* wk);
void Passive18_0074(PLW* wk);
void Passive18_0075(PLW* wk);
void Passive18_0076(PLW* wk);
void Passive18_0077(PLW* wk);
void Passive18_0078(PLW* wk);
void Passive18_0079(PLW* wk);
void Passive18_0080(PLW* wk);
void Passive18_0081(PLW* wk);
void Passive18_0082(PLW* wk);
void Passive18_0083(PLW* wk);
void Passive18_0084(PLW* wk);
void Passive18_0085(PLW* wk);
void Passive18_0086(PLW* wk);
void Passive18_0087(PLW* wk);
void Passive18_0088(PLW* wk);
void Passive18_0089(PLW* wk);
void Passive18_0090(PLW* wk);
void Passive18_0091(PLW* wk);
void Passive18_0092(PLW* wk);
void Passive18_0093(PLW* wk);
void Passive18_0094(PLW* wk);
void Passive18_0095(PLW* wk);
void Passive18_0096(PLW* wk);
void Passive18_0097(PLW* wk);
void Passive18_0098(PLW* wk);
void Passive18_0099(PLW* wk);
void Passive18_0100(PLW* wk);
void Passive18_0101(PLW* wk);
void Passive18_0102(PLW* wk);
void Passive18_0103(PLW* wk);
void Passive18_0104(PLW* wk);
void Passive18_0105(PLW* wk);
void Passive18_0106(PLW* wk);
void Passive18_0107(PLW* wk);
void Passive18_0108(PLW* wk);
void Passive18_0109(PLW* wk);
void Passive18_0110(PLW* wk);
void Passive18_0111(PLW* wk);
void Passive18_0112(PLW* wk);
void Passive18_0113(PLW* wk);
void Passive18_0114(PLW* wk);
void Passive18_0115(PLW* wk);
void Passive18_0116(PLW* wk);
void Passive18_0117(PLW* wk);
void Passive18_0118(PLW* wk);
void Passive18_0119(PLW* wk);
void Passive18_0120(PLW* wk);
void Passive18_0121(PLW* wk);
void Passive18_0122(PLW* wk);
void Passive18_0123(PLW* wk);
void Passive18_0124(PLW* wk);
void Passive18_0125(PLW* wk);
void Passive18_0126(PLW* wk);
void Passive18_0127(PLW* wk);
void Passive18_0128(PLW* wk);
void Passive18_0129(PLW* wk);
void Passive18_0130(PLW* wk);
void Passive18_0131(PLW* wk);
void Passive18_0132(PLW* wk);
void Passive18_0133(PLW* wk);
void Passive18_0134(PLW* wk);
void Passive18_0135(PLW* wk);
void Passive18_0136(PLW* wk);
void Passive18_0137(PLW* wk);
void Passive18_0138(PLW* wk);
void Passive18_0139(PLW* wk);
void Passive18_0140(PLW* wk);
void Passive18_0141(PLW* wk);
void Passive18_0142(PLW* wk);
void Passive18_0143(PLW* wk);
void Passive18_0144(PLW* wk);
void Passive18_0145(PLW* wk);
void Passive18_0146(PLW* wk);
void Passive18_0147(PLW* wk);
void Passive18_0148(PLW* wk);
void Passive18_0149(PLW* wk);
void Passive18_0150(PLW* wk);
void Passive18_0151(PLW* wk);
void Passive18_0152(PLW* wk);
void Passive18_0153(PLW* wk);
void Passive18_0154(PLW* wk);
void Passive18_0155(PLW* wk);
void Passive18_0156(PLW* wk);
void Passive18_0157(PLW* wk);
void Passive18_0158(PLW* wk);
void Passive18_0159(PLW* wk);
void Passive18_0160(PLW* wk);
void Passive18_0161(PLW* wk);
void Passive18_0162(PLW* wk);
void Passive18_0163(PLW* wk);
void Passive18_0164(PLW* wk);
void Passive18_0165(PLW* wk);
void Passive18_0166(PLW* wk);
void Passive18_0167(PLW* wk);
void Passive18_0168(PLW* wk);
void Passive18_0169(PLW* wk);
void Passive18_0170(PLW* wk);
void Passive18_0171(PLW* wk);
void Passive18_0172(PLW* wk);
void Passive18_0173(PLW* wk);
void Passive18_0174(PLW* wk);
void Passive18_0175(PLW* wk);
void Passive18_0176(PLW* wk);
void Passive18_0177(PLW* wk);
void Passive18_0178(PLW* wk);
void Passive18_0179(PLW* wk);
void Passive18_0180(PLW* wk);
void Passive18_0181(PLW* wk);
void Passive18_0182(PLW* wk);
void Passive18_0183(PLW* wk);
void Passive18_0184(PLW* wk);
void Passive18_0185(PLW* wk);
void Passive18_0186(PLW* wk);
void Passive18_0187(PLW* wk);
void Passive18_0188(PLW* wk);
void Passive18_0189(PLW* wk);
void Passive18_0190(PLW* wk);
void Passive18_0191(PLW* wk);
void Passive18_0192(PLW* wk);
void Passive18_0193(PLW* wk);
void Passive18_0194(PLW* wk);
void Passive18_0195(PLW* wk);
void Passive18_0196(PLW* wk);
void Passive18_0197(PLW* wk);
void Passive18_0198(PLW* wk);
void Passive18_0199(PLW* wk);
void Passive18_0200(PLW* wk);
void Passive18_0201(PLW* wk);
void Passive18_0202(PLW* wk);
void Passive18_0203(PLW* wk);

#endif
