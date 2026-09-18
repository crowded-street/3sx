/*
 * Internal declarations for the pass08 pattern scripts.
 *
 * pass08.c holds the dispatcher and Passive08_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS08_INTERNAL_H
#define PASS08_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive08_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive08_pattern_lever_attack(PLW* wk, u16 lever_data);
void passive08_pattern_wait_lie_approach_walk_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data);
void passive08_pattern_pierce_on_j_command_attack_search_back_term(PLW* wk, const Command_Attack_Args* p);
void passive08_0056_from_step_6(PLW* wk);
void Passive08_0056(PLW* wk);
void Passive08_0057(PLW* wk);
void Passive08_0058(PLW* wk);
void Passive08_0059(PLW* wk);
void Passive08_0060(PLW* wk);
void Passive08_0061(PLW* wk);
void Passive08_0062(PLW* wk);
void Passive08_0063(PLW* wk);
void Passive08_0064(PLW* wk);
void Passive08_0065(PLW* wk);
void Passive08_0066(PLW* wk);
void Passive08_0067(PLW* wk);
void Passive08_0068(PLW* wk);
void Passive08_0069(PLW* wk);
void Passive08_0070(PLW* wk);
void Passive08_0071(PLW* wk);
void Passive08_0072(PLW* wk);
void Passive08_0073(PLW* wk);
void Passive08_0074(PLW* wk);
void Passive08_0075(PLW* wk);
void Passive08_0076(PLW* wk);
void Passive08_0077(PLW* wk);
void Passive08_0078(PLW* wk);
void Passive08_0079(PLW* wk);
void Passive08_0080(PLW* wk);
void Passive08_0081(PLW* wk);
void Passive08_0082(PLW* wk);
void Passive08_0083(PLW* wk);
void Passive08_0084(PLW* wk);
void Passive08_0085(PLW* wk);
void Passive08_0086(PLW* wk);
void Passive08_0087(PLW* wk);
void Passive08_0088(PLW* wk);
void Passive08_0089(PLW* wk);
void Passive08_0090(PLW* wk);
void Passive08_0091(PLW* wk);
void Passive08_0092(PLW* wk);
void Passive08_0093(PLW* wk);
void Passive08_0094(PLW* wk);
void Passive08_0095(PLW* wk);
void Passive08_0096(PLW* wk);
void Passive08_0097(PLW* wk);
void Passive08_0098(PLW* wk);
void Passive08_0099(PLW* wk);
void Passive08_0100(PLW* wk);
void Passive08_0101(PLW* wk);
void Passive08_0102(PLW* wk);
void Passive08_0103(PLW* wk);
void Passive08_0104(PLW* wk);
void Passive08_0105(PLW* wk);
void Passive08_0106(PLW* wk);
void Passive08_0107(PLW* wk);
void Passive08_0108(PLW* wk);
void Passive08_0109(PLW* wk);
void Passive08_0110(PLW* wk);
void Passive08_0111(PLW* wk);
void Passive08_0112(PLW* wk);
void Passive08_0113(PLW* wk);
void Passive08_0114(PLW* wk);
void Passive08_0115(PLW* wk);
void Passive08_0116(PLW* wk);
void Passive08_0117(PLW* wk);
void Passive08_0118(PLW* wk);
void Passive08_0119(PLW* wk);
void Passive08_0120(PLW* wk);
void Passive08_0121(PLW* wk);
void Passive08_0122(PLW* wk);
void Passive08_0123(PLW* wk);
void Passive08_0124(PLW* wk);
void Passive08_0125(PLW* wk);
void Passive08_0126(PLW* wk);
void Passive08_0127(PLW* wk);
void Passive08_0128(PLW* wk);
void Passive08_0129(PLW* wk);
void Passive08_0130(PLW* wk);
void Passive08_0131(PLW* wk);
void Passive08_0132(PLW* wk);
void Passive08_0133(PLW* wk);
void Passive08_0134(PLW* wk);
void Passive08_0135(PLW* wk);
void Passive08_0136(PLW* wk);
void Passive08_0137(PLW* wk);
void Passive08_0138(PLW* wk);
void Passive08_0139(PLW* wk);
void Passive08_0140(PLW* wk);
void Passive08_0141(PLW* wk);
void Passive08_0142(PLW* wk);
void Passive08_0143(PLW* wk);
void Passive08_0144(PLW* wk);
void Passive08_0145(PLW* wk);
void Passive08_0146(PLW* wk);
void Passive08_0147(PLW* wk);
void Passive08_0148(PLW* wk);
void Passive08_0149(PLW* wk);
void Passive08_0150(PLW* wk);
void Passive08_0151(PLW* wk);
void Passive08_0152(PLW* wk);
void Passive08_0153(PLW* wk);
void Passive08_0154(PLW* wk);
void Passive08_0155(PLW* wk);
void Passive08_0156(PLW* wk);
void Passive08_0157(PLW* wk);
void Passive08_0158(PLW* wk);
void passive08_0159_from_step_6(PLW* wk);
void Passive08_0159(PLW* wk);
void Passive08_0160(PLW* wk);
void Passive08_0161(PLW* wk);
void Passive08_0162(PLW* wk);
void Passive08_0163(PLW* wk);
void Passive08_0164(PLW* wk);
void Passive08_0165(PLW* wk);
void Passive08_0166(PLW* wk);
void Passive08_0167(PLW* wk);
void Passive08_0168(PLW* wk);
void Passive08_0169(PLW* wk);
void Passive08_0170(PLW* wk);
void Passive08_0171(PLW* wk);
void Passive08_0172(PLW* wk);
void Passive08_0173(PLW* wk);
void Passive08_0174(PLW* wk);
void Passive08_0175(PLW* wk);
void Passive08_0176(PLW* wk);
void Passive08_0177(PLW* wk);
void Passive08_0178(PLW* wk);
void Passive08_0179(PLW* wk);
void Passive08_0180(PLW* wk);
void Passive08_0181(PLW* wk);
void Passive08_0182(PLW* wk);
void Passive08_0183(PLW* wk);
void Passive08_0184(PLW* wk);
void Passive08_0185(PLW* wk);
void Passive08_0186(PLW* wk);
void Passive08_0187(PLW* wk);
void Passive08_0188(PLW* wk);
void Passive08_0189(PLW* wk);
void Passive08_0190(PLW* wk);
void Passive08_0191(PLW* wk);
void Passive08_0192(PLW* wk);
void Passive08_0193(PLW* wk);
void Passive08_0194(PLW* wk);
void Passive08_0195(PLW* wk);
void Passive08_0196(PLW* wk);
void Passive08_0197(PLW* wk);
void Passive08_0198(PLW* wk);
void Passive08_0199(PLW* wk);
void Passive08_0200(PLW* wk);
void Passive08_0201(PLW* wk);
void Passive08_0202(PLW* wk);
void Passive08_0203(PLW* wk);
void Passive08_0204(PLW* wk);
void Passive08_0205(PLW* wk);
void Passive08_0206(PLW* wk);
void Passive08_0207(PLW* wk);
void Passive08_0208(PLW* wk);
void Passive08_0209(PLW* wk);
void Passive08_0210(PLW* wk);
void Passive08_0211(PLW* wk);
void Passive08_0212(PLW* wk);
void Passive08_0213(PLW* wk);

#endif
