/*
 * Internal declarations for the pass11 pattern scripts.
 *
 * pass11.c holds the dispatcher and Passive11_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS11_INTERNAL_H
#define PASS11_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive11_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive11_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive11_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void passive11_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive11_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive11_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive11_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive11_pattern_em_term_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive11_pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void passive11_pattern_normal_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void passive11_pattern_wait_get_up_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void passive11_pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void passive11_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void passive11_pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void passive11_pattern_approach_walk_em_term_normal_attack(PLW* wk, const Command_Attack_Args* p);
void passive11_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive11_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive11_pattern_approach_walk(PLW* wk, s16 target_pos);
void passive11_pattern_normal_attack_com_random_select(PLW* wk, u16 lever_data, const Branch_Menu_Args* p, s16 rnd_type);
void passive11_pattern_keep_away_wait_get_up(PLW* wk, u16 lever_data);
void passive11_pattern_command_attack_com_random_select(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
);
void passive11_pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void passive11_pattern_approach_walk_em_term_sa_term(PLW* wk, u16 lever);
void passive11_pattern_em_term_sa_term_normal_attack(PLW* wk, u16 lever_data);
void Passive11_0052(PLW* wk);
void Passive11_0053(PLW* wk);
void Passive11_0054(PLW* wk);
void Passive11_0055(PLW* wk);
void Passive11_0056(PLW* wk);
void Passive11_0057(PLW* wk);
void Passive11_0058(PLW* wk);
void Passive11_0059(PLW* wk);
void Passive11_0060(PLW* wk);
void Passive11_0061(PLW* wk);
void Passive11_0062(PLW* wk);
void Passive11_0063(PLW* wk);
void Passive11_0064(PLW* wk);
void Passive11_0065(PLW* wk);
void Passive11_0066(PLW* wk);
void Passive11_0067(PLW* wk);
void Passive11_0068(PLW* wk);
void Passive11_0069(PLW* wk);
void Passive11_0070(PLW* wk);
void Passive11_0071(PLW* wk);
void Passive11_0072(PLW* wk);
void Passive11_0073(PLW* wk);
void Passive11_0074(PLW* wk);
void Passive11_0075(PLW* wk);
void Passive11_0076(PLW* wk);
void Passive11_0077(PLW* wk);
void Passive11_0078(PLW* wk);
void Passive11_0079(PLW* wk);
void Passive11_0080(PLW* wk);
void Passive11_0081(PLW* wk);
void Passive11_0082(PLW* wk);
void Passive11_0083(PLW* wk);
void Passive11_0084(PLW* wk);
void Passive11_0085(PLW* wk);
void Passive11_0086(PLW* wk);
void Passive11_0087(PLW* wk);
void Passive11_0088(PLW* wk);
void Passive11_0089(PLW* wk);
void Passive11_0090(PLW* wk);
void Passive11_0091(PLW* wk);
void Passive11_0092(PLW* wk);
void Passive11_0093(PLW* wk);
void Passive11_0094(PLW* wk);
void Passive11_0095(PLW* wk);
void Passive11_0096(PLW* wk);
void Passive11_0097(PLW* wk);
void Passive11_0098(PLW* wk);
void Passive11_0099(PLW* wk);
void Passive11_0100(PLW* wk);
void Passive11_0101(PLW* wk);
void Passive11_0102(PLW* wk);
void Passive11_0103(PLW* wk);
void Passive11_0104(PLW* wk);
void Passive11_0105(PLW* wk);
void Passive11_0106(PLW* wk);
void Passive11_0107(PLW* wk);
void Passive11_0108(PLW* wk);
void Passive11_0109(PLW* wk);
void Passive11_0110(PLW* wk);
void Passive11_0111(PLW* wk);
void Passive11_0112(PLW* wk);
void Passive11_0113(PLW* wk);
void Passive11_0114(PLW* wk);
void Passive11_0115(PLW* wk);
void Passive11_0116(PLW* wk);
void Passive11_0117(PLW* wk);
void Passive11_0118(PLW* wk);
void Passive11_0119(PLW* wk);
void Passive11_0120(PLW* wk);
void Passive11_0121(PLW* wk);
void Passive11_0122(PLW* wk);
void Passive11_0123(PLW* wk);
void Passive11_0124(PLW* wk);
void Passive11_0125(PLW* wk);
void Passive11_0126(PLW* wk);
void Passive11_0127(PLW* wk);
void Passive11_0128(PLW* wk);
void Passive11_0129(PLW* wk);
void Passive11_0130(PLW* wk);
void Passive11_0131(PLW* wk);
void Passive11_0132(PLW* wk);
void Passive11_0133(PLW* wk);
void Passive11_0134(PLW* wk);
void Passive11_0135(PLW* wk);
void Passive11_0136(PLW* wk);
void Passive11_0137(PLW* wk);
void Passive11_0138(PLW* wk);
void Passive11_0139(PLW* wk);
void Passive11_0140(PLW* wk);
void Passive11_0141(PLW* wk);
void Passive11_0142(PLW* wk);
void Passive11_0143(PLW* wk);
void Passive11_0144(PLW* wk);
void Passive11_0145(PLW* wk);
void Passive11_0146(PLW* wk);
void Passive11_0147(PLW* wk);
void Passive11_0148(PLW* wk);
void Passive11_0149(PLW* wk);
void Passive11_0150(PLW* wk);
void Passive11_0151(PLW* wk);
void Passive11_0152(PLW* wk);
void Passive11_0153(PLW* wk);
void Passive11_0154(PLW* wk);
void Passive11_0155(PLW* wk);
void Passive11_0156(PLW* wk);
void Passive11_0157(PLW* wk);
void Passive11_0158(PLW* wk);
void Passive11_0159(PLW* wk);
void Passive11_0160(PLW* wk);
void Passive11_0161(PLW* wk);
void Passive11_0162(PLW* wk);
void Passive11_0163(PLW* wk);
void Passive11_0164(PLW* wk);
void Passive11_0165(PLW* wk);
void Passive11_0166(PLW* wk);
void Passive11_0167(PLW* wk);
void Passive11_0168(PLW* wk);
void Passive11_0169(PLW* wk);
void Passive11_0170(PLW* wk);
void Passive11_0171(PLW* wk);
void Passive11_0172(PLW* wk);
void Passive11_0173(PLW* wk);
void Passive11_0174(PLW* wk);
void Passive11_0175(PLW* wk);
void Passive11_0176(PLW* wk);
void Passive11_0177(PLW* wk);
void Passive11_0178(PLW* wk);
void Passive11_0179(PLW* wk);
void Passive11_0180(PLW* wk);
void Passive11_0181(PLW* wk);
void Passive11_0182(PLW* wk);
void Passive11_0183(PLW* wk);
void Passive11_0184(PLW* wk);
void Passive11_0185(PLW* wk);
void Passive11_0186(PLW* wk);
void Passive11_0187(PLW* wk);
void Passive11_0188(PLW* wk);
void Passive11_0189(PLW* wk);
void Passive11_0190(PLW* wk);
void Passive11_0191(PLW* wk);
void Passive11_0192(PLW* wk);
void Passive11_0193(PLW* wk);
void Passive11_0194(PLW* wk);
void Passive11_0195(PLW* wk);
void Passive11_0196(PLW* wk);
void Passive11_0197(PLW* wk);
void Passive11_0198(PLW* wk);
void Passive11_0199(PLW* wk);
void Passive11_0200(PLW* wk);
void Passive11_0201(PLW* wk);
void Passive11_0202(PLW* wk);
void Passive11_0203(PLW* wk);
void Passive11_0204(PLW* wk);
void Passive11_0205(PLW* wk);
void Passive11_0206(PLW* wk);
void Passive11_0207(PLW* wk);
void Passive11_0208(PLW* wk);
void Passive11_0209(PLW* wk);
void Passive11_0210(PLW* wk);
void Passive11_0211(PLW* wk);
void Passive11_0212(PLW* wk);
void Passive11_0213(PLW* wk);
void Passive11_0214(PLW* wk);
void Passive11_0215(PLW* wk);
void Passive11_0216(PLW* wk);
void Passive11_0217(PLW* wk);
void Passive11_0218(PLW* wk);
void Passive11_0219(PLW* wk);
void Passive11_0220(PLW* wk);
void Passive11_0221(PLW* wk);
void Passive11_0222(PLW* wk);
void Passive11_0223(PLW* wk);
void Passive11_0224(PLW* wk);
void Passive11_0225(PLW* wk);
void Passive11_0226(PLW* wk);
void Passive11_0227(PLW* wk);
void Passive11_0228(PLW* wk);
void Passive11_0229(PLW* wk);
void Passive11_0230(PLW* wk);
void Passive11_0231(PLW* wk);
void Passive11_0232(PLW* wk);
void Passive11_0233(PLW* wk);
void Passive11_0234(PLW* wk);
void Passive11_0235(PLW* wk);
void Passive11_0236(PLW* wk);
void Passive11_0237(PLW* wk);
void Passive11_0238(PLW* wk);
void Passive11_0239(PLW* wk);
void Passive11_0240(PLW* wk);
void Passive11_0241(PLW* wk);
void Passive11_0242(PLW* wk);
void Passive11_0243(PLW* wk);
void Passive11_0244(PLW* wk);
void Passive11_0245(PLW* wk);
void Passive11_0246(PLW* wk);
void Passive11_0247(PLW* wk);
void Passive11_0248(PLW* wk);
void Passive11_0249(PLW* wk);
void Passive11_0250(PLW* wk);
void Passive11_0251(PLW* wk);
void Passive11_0252(PLW* wk);
void Passive11_0253(PLW* wk);
void Passive11_0254(PLW* wk);

#endif
