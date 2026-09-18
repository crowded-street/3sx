/*
 * Internal declarations for the pass12 pattern scripts.
 *
 * pass12.c holds the dispatcher and Passive12_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS12_INTERNAL_H
#define PASS12_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive12_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive12_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive12_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive12_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive12_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive12_pattern_normal_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void passive12_pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void passive12_pattern_approach_walk_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive12_pattern_wait_get_up_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive12_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive12_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive12_pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, u16 lever_data_b);
void passive12_pattern_wait_lie_approach_walk(PLW* wk, s16 target_pos);
void passive12_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive12_pattern_approach_walk(PLW* wk, s16 target_pos);
void passive12_pattern_normal_attack_com_random_select(PLW* wk, u16 lever_data, const Branch_Menu_Args* p, s16 rnd_type);
void passive12_pattern_keep_away_wait_get_up(PLW* wk, u16 lever_data);
void passive12_pattern_command_attack_com_random_select(PLW* wk, const Command_Attack_Args* p);
void Passive12_0063(PLW* wk);
void Passive12_0064(PLW* wk);
void Passive12_0065(PLW* wk);
void Passive12_0066(PLW* wk);
void Passive12_0067(PLW* wk);
void Passive12_0068(PLW* wk);
void Passive12_0069(PLW* wk);
void Passive12_0070(PLW* wk);
void Passive12_0071(PLW* wk);
void Passive12_0072(PLW* wk);
void Passive12_0073(PLW* wk);
void Passive12_0074(PLW* wk);
void Passive12_0075(PLW* wk);
void Passive12_0076(PLW* wk);
void Passive12_0077(PLW* wk);
void Passive12_0078(PLW* wk);
void Passive12_0079(PLW* wk);
void Passive12_0080(PLW* wk);
void Passive12_0081(PLW* wk);
void Passive12_0082(PLW* wk);
void Passive12_0083(PLW* wk);
void Passive12_0084(PLW* wk);
void Passive12_0085(PLW* wk);
void Passive12_0086(PLW* wk);
void Passive12_0087(PLW* wk);
void Passive12_0088(PLW* wk);
void Passive12_0089(PLW* wk);
void Passive12_0090(PLW* wk);
void Passive12_0091(PLW* wk);
void Passive12_0092(PLW* wk);
void Passive12_0093(PLW* wk);
void Passive12_0094(PLW* wk);
void Passive12_0095(PLW* wk);
void Passive12_0096(PLW* wk);
void Passive12_0097(PLW* wk);
void Passive12_0098(PLW* wk);
void Passive12_0099(PLW* wk);
void Passive12_0100(PLW* wk);
void Passive12_0101(PLW* wk);
void Passive12_0102(PLW* wk);
void Passive12_0103(PLW* wk);
void Passive12_0104(PLW* wk);
void Passive12_0105(PLW* wk);
void Passive12_0106(PLW* wk);
void Passive12_0107(PLW* wk);
void Passive12_0108(PLW* wk);
void Passive12_0109(PLW* wk);
void Passive12_0110(PLW* wk);
void Passive12_0111(PLW* wk);
void Passive12_0112(PLW* wk);
void Passive12_0113(PLW* wk);
void Passive12_0114(PLW* wk);
void Passive12_0115(PLW* wk);
void Passive12_0116(PLW* wk);
void Passive12_0117(PLW* wk);
void Passive12_0118(PLW* wk);
void Passive12_0119(PLW* wk);
void Passive12_0120(PLW* wk);
void Passive12_0121(PLW* wk);
void Passive12_0122(PLW* wk);
void Passive12_0123(PLW* wk);
void Passive12_0124(PLW* wk);
void Passive12_0125(PLW* wk);
void Passive12_0126(PLW* wk);
void Passive12_0127(PLW* wk);
void Passive12_0128(PLW* wk);
void Passive12_0129(PLW* wk);
void Passive12_0130(PLW* wk);
void Passive12_0131(PLW* wk);
void Passive12_0132(PLW* wk);
void Passive12_0133(PLW* wk);
void Passive12_0134(PLW* wk);
void Passive12_0135(PLW* wk);
void Passive12_0136(PLW* wk);
void Passive12_0137(PLW* wk);
void Passive12_0138(PLW* wk);
void Passive12_0139(PLW* wk);
void Passive12_0140(PLW* wk);
void Passive12_0141(PLW* wk);
void Passive12_0142(PLW* wk);
void Passive12_0143(PLW* wk);
void Passive12_0144(PLW* wk);
void Passive12_0145(PLW* wk);
void Passive12_0146(PLW* wk);
void Passive12_0147(PLW* wk);
void Passive12_0148(PLW* wk);
void Passive12_0149(PLW* wk);
void Passive12_0150(PLW* wk);
void Passive12_0151(PLW* wk);
void Passive12_0152(PLW* wk);
void Passive12_0153(PLW* wk);
void Passive12_0154(PLW* wk);
void Passive12_0155(PLW* wk);
void Passive12_0156(PLW* wk);
void Passive12_0157(PLW* wk);
void Passive12_0158(PLW* wk);
void Passive12_0159(PLW* wk);
void Passive12_0160(PLW* wk);
void Passive12_0161(PLW* wk);
void Passive12_0162(PLW* wk);
void Passive12_0163(PLW* wk);
void Passive12_0164(PLW* wk);
void Passive12_0165(PLW* wk);
void Passive12_0166(PLW* wk);
void Passive12_0167(PLW* wk);
void Passive12_0168(PLW* wk);
void Passive12_0169(PLW* wk);
void Passive12_0170(PLW* wk);
void Passive12_0171(PLW* wk);
void Passive12_0172(PLW* wk);
void Passive12_0173(PLW* wk);
void Passive12_0174(PLW* wk);
void Passive12_0175(PLW* wk);
void Passive12_0176(PLW* wk);
void Passive12_0177(PLW* wk);
void Passive12_0178(PLW* wk);
void Passive12_0179(PLW* wk);
void Passive12_0180(PLW* wk);
void Passive12_0181(PLW* wk);
void Passive12_0182(PLW* wk);
void Passive12_0183(PLW* wk);
void Passive12_0184(PLW* wk);
void Passive12_0185(PLW* wk);
void Passive12_0186(PLW* wk);
void Passive12_0187(PLW* wk);
void Passive12_0188(PLW* wk);
void Passive12_0189(PLW* wk);
void Passive12_0190(PLW* wk);
void Passive12_0191(PLW* wk);
void Passive12_0192(PLW* wk);
void Passive12_0193(PLW* wk);
void Passive12_0194(PLW* wk);
void Passive12_0195(PLW* wk);
void Passive12_0196(PLW* wk);
void Passive12_0197(PLW* wk);
void Passive12_0198(PLW* wk);
void Passive12_0199(PLW* wk);
void Passive12_0200(PLW* wk);
void Passive12_0201(PLW* wk);
void Passive12_0202(PLW* wk);
void Passive12_0203(PLW* wk);
void Passive12_0204(PLW* wk);
void Passive12_0205(PLW* wk);
void Passive12_0206(PLW* wk);
void Passive12_0207(PLW* wk);
void Passive12_0208(PLW* wk);
void Passive12_0209(PLW* wk);
void Passive12_0210(PLW* wk);
void Passive12_0211(PLW* wk);
void Passive12_0212(PLW* wk);
void Passive12_0213(PLW* wk);
void Passive12_0214(PLW* wk);
void Passive12_0215(PLW* wk);
void Passive12_0216(PLW* wk);
void Passive12_0217(PLW* wk);
void Passive12_0218(PLW* wk);
void Passive12_0219(PLW* wk);
void Passive12_0220(PLW* wk);
void Passive12_0221(PLW* wk);
void Passive12_0222(PLW* wk);
void Passive12_0223(PLW* wk);
void Passive12_0224(PLW* wk);
void Passive12_0225(PLW* wk);
void Passive12_0226(PLW* wk);
void Passive12_0227(PLW* wk);
void Passive12_0228(PLW* wk);
void Passive12_0229(PLW* wk);
void Passive12_0230(PLW* wk);

#endif
