/*
 * Internal declarations for the pass13 pattern scripts.
 *
 * pass13.c holds the dispatcher and Passive13_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS13_INTERNAL_H
#define PASS13_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive13_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive13_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive13_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive13_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive13_pattern_lever_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive13_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive13_pattern_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b);
void passive13_pattern_pierce_on_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void passive13_pattern_hi_jump_attack_term_normal_attack(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
);
void Passive13_0059(PLW* wk);
void Passive13_0060(PLW* wk);
void Passive13_0061(PLW* wk);
void Passive13_0062(PLW* wk);
void Passive13_0063(PLW* wk);
void Passive13_0064(PLW* wk);
void Passive13_0065(PLW* wk);
void Passive13_0066(PLW* wk);
void Passive13_0067(PLW* wk);
void Passive13_0068(PLW* wk);
void Passive13_0069(PLW* wk);
void Passive13_0070(PLW* wk);
void Passive13_0071(PLW* wk);
void Passive13_0072(PLW* wk);
void Passive13_0073(PLW* wk);
void Passive13_0074(PLW* wk);
void Passive13_0075(PLW* wk);
void Passive13_0076(PLW* wk);
void Passive13_0077(PLW* wk);
void Passive13_0078(PLW* wk);
void Passive13_0079(PLW* wk);
void Passive13_0080(PLW* wk);
void Passive13_0081(PLW* wk);
void Passive13_0082(PLW* wk);
void Passive13_0083(PLW* wk);
void Passive13_0084(PLW* wk);
void Passive13_0085(PLW* wk);
void Passive13_0086(PLW* wk);
void Passive13_0087(PLW* wk);
void Passive13_0088(PLW* wk);
void Passive13_0089(PLW* wk);
void Passive13_0090(PLW* wk);
void Passive13_0091(PLW* wk);
void Passive13_0092(PLW* wk);
void Passive13_0093(PLW* wk);
void Passive13_0094(PLW* wk);
void Passive13_0095(PLW* wk);
void Passive13_0096(PLW* wk);
void Passive13_0097(PLW* wk);
void Passive13_0098(PLW* wk);
void Passive13_0099(PLW* wk);
void Passive13_0100(PLW* wk);
void Passive13_0101(PLW* wk);
void Passive13_0102(PLW* wk);
void Passive13_0103(PLW* wk);
void Passive13_0104(PLW* wk);
void Passive13_0105(PLW* wk);
void Passive13_0106(PLW* wk);
void Passive13_0107(PLW* wk);
void Passive13_0108(PLW* wk);
void Passive13_0109(PLW* wk);
void Passive13_0110(PLW* wk);
void Passive13_0111(PLW* wk);
void Passive13_0112(PLW* wk);
void passive13_0113_from_step_6(PLW* wk);
void Passive13_0113(PLW* wk);
void Passive13_0114(PLW* wk);
void Passive13_0115(PLW* wk);
void Passive13_0116(PLW* wk);
void Passive13_0117(PLW* wk);
void Passive13_0118(PLW* wk);
void Passive13_0119(PLW* wk);
void Passive13_0120(PLW* wk);
void Passive13_0121(PLW* wk);
void Passive13_0122(PLW* wk);
void Passive13_0123(PLW* wk);
void Passive13_0124(PLW* wk);
void Passive13_0125(PLW* wk);
void Passive13_0126(PLW* wk);
void Passive13_0127(PLW* wk);
void Passive13_0128(PLW* wk);
void Passive13_0129(PLW* wk);
void Passive13_0130(PLW* wk);
void Passive13_0131(PLW* wk);
void Passive13_0132(PLW* wk);
void Passive13_0133(PLW* wk);
void Passive13_0134(PLW* wk);
void Passive13_0135(PLW* wk);
void Passive13_0136(PLW* wk);
void Passive13_0137(PLW* wk);
void Passive13_0138(PLW* wk);
void Passive13_0139(PLW* wk);
void Passive13_0140(PLW* wk);
void Passive13_0141(PLW* wk);
void Passive13_0142(PLW* wk);
void Passive13_0143(PLW* wk);
void Passive13_0144(PLW* wk);
void Passive13_0145(PLW* wk);
void Passive13_0146(PLW* wk);
void Passive13_0147(PLW* wk);
void Passive13_0148(PLW* wk);
void Passive13_0149(PLW* wk);
void Passive13_0150(PLW* wk);
void Passive13_0151(PLW* wk);
void Passive13_0152(PLW* wk);
void Passive13_0153(PLW* wk);
void Passive13_0154(PLW* wk);
void Passive13_0155(PLW* wk);
void Passive13_0156(PLW* wk);
void Passive13_0157(PLW* wk);
void Passive13_0158(PLW* wk);
void Passive13_0159(PLW* wk);
void Passive13_0160(PLW* wk);
void Passive13_0161(PLW* wk);
void Passive13_0162(PLW* wk);
void Passive13_0163(PLW* wk);

#endif
