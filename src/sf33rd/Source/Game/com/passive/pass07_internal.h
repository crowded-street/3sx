/*
 * Internal declarations for the pass07 pattern scripts.
 *
 * pass07.c holds the dispatcher and Passive07_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS07_INTERNAL_H
#define PASS07_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive07_pattern_keep_away_wait_get_up(PLW* wk, u16 lever_data, s16 option);
void passive07_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive07_pattern_wait_get_up_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void Passive07_0055(PLW* wk);
void Passive07_0056(PLW* wk);
void Passive07_0057(PLW* wk);
void Passive07_0058(PLW* wk);
void Passive07_0059(PLW* wk);
void Passive07_0060(PLW* wk);
void Passive07_0061(PLW* wk);
void Passive07_0062(PLW* wk);
void Passive07_0063(PLW* wk);
void Passive07_0064(PLW* wk);
void Passive07_0065(PLW* wk);
void Passive07_0066(PLW* wk);
void Passive07_0067(PLW* wk);
void Passive07_0068(PLW* wk);
void Passive07_0069(PLW* wk);
void Passive07_0070(PLW* wk);
void Passive07_0071(PLW* wk);
void Passive07_0072(PLW* wk);
void Passive07_0073(PLW* wk);
void Passive07_0074(PLW* wk);
void Passive07_0075(PLW* wk);
void Passive07_0076(PLW* wk);
void Passive07_0077(PLW* wk);
void Passive07_0078(PLW* wk);
void Passive07_0079(PLW* wk);
void Passive07_0080(PLW* wk);
void Passive07_0081(PLW* wk);
void Passive07_0082(PLW* wk);
void Passive07_0083(PLW* wk);
void Passive07_0084(PLW* wk);
void Passive07_0085(PLW* wk);
void Passive07_0086(PLW* wk);
void Passive07_0087(PLW* wk);
void Passive07_0088(PLW* wk);
void Passive07_0089(PLW* wk);
void Passive07_0090(PLW* wk);
void Passive07_0091(PLW* wk);
void Passive07_0092(PLW* wk);
void Passive07_0093(PLW* wk);
void Passive07_0094(PLW* wk);
void Passive07_0095(PLW* wk);
void Passive07_0096(PLW* wk);
void Passive07_0097(PLW* wk);
void Passive07_0098(PLW* wk);
void Passive07_0099(PLW* wk);
void Passive07_0100(PLW* wk);
void Passive07_0101(PLW* wk);
void Passive07_0102(PLW* wk);
void Passive07_0103(PLW* wk);
void Passive07_0104(PLW* wk);
void Passive07_0105(PLW* wk);
void Passive07_0106(PLW* wk);
void Passive07_0107(PLW* wk);
void Passive07_0108(PLW* wk);
void Passive07_0109(PLW* wk);
void Passive07_0110(PLW* wk);
void Passive07_0111(PLW* wk);
void Passive07_0112(PLW* wk);
void Passive07_0113(PLW* wk);
void Passive07_0114(PLW* wk);
void Passive07_0115(PLW* wk);
void Passive07_0116(PLW* wk);
void Passive07_0117(PLW* wk);
void Passive07_0118(PLW* wk);
void Passive07_0119(PLW* wk);
void Passive07_0120(PLW* wk);
void Passive07_0121(PLW* wk);
void Passive07_0122(PLW* wk);
void Passive07_0123(PLW* wk);
void Passive07_0124(PLW* wk);
void Passive07_0125(PLW* wk);
void Passive07_0126(PLW* wk);
void Passive07_0127(PLW* wk);
void Passive07_0128(PLW* wk);
void Passive07_0129(PLW* wk);
void Passive07_0130(PLW* wk);
void Passive07_0131(PLW* wk);
void Passive07_0132(PLW* wk);
void Passive07_0133(PLW* wk);
void Passive07_0134(PLW* wk);
void Passive07_0135(PLW* wk);
void Passive07_0136(PLW* wk);
void Passive07_0137(PLW* wk);
void Passive07_0138(PLW* wk);
void Passive07_0139(PLW* wk);
void Passive07_0140(PLW* wk);
void Passive07_0141(PLW* wk);
void Passive07_0142(PLW* wk);
void Passive07_0143(PLW* wk);
void Passive07_0144(PLW* wk);
void Passive07_0145(PLW* wk);
void Passive07_0146(PLW* wk);
void Passive07_0147(PLW* wk);
void Passive07_0148(PLW* wk);
void Passive07_0149(PLW* wk);
void Passive07_0150(PLW* wk);
void Passive07_0151(PLW* wk);
void Passive07_0152(PLW* wk);
void Passive07_0153(PLW* wk);
void Passive07_0154(PLW* wk);
void Passive07_0155(PLW* wk);
void Passive07_0156(PLW* wk);
void Passive07_0157(PLW* wk);
void Passive07_0158(PLW* wk);
void Passive07_0159(PLW* wk);
void Passive07_0160(PLW* wk);
void Passive07_0161(PLW* wk);
void Passive07_0162(PLW* wk);
void Passive07_0163(PLW* wk);
void Passive07_0164(PLW* wk);
void Passive07_0165(PLW* wk);
void Passive07_0166(PLW* wk);
void Passive07_0167(PLW* wk);
void Passive07_0168(PLW* wk);
void Passive07_0169(PLW* wk);
void Passive07_0170(PLW* wk);
void Passive07_0171(PLW* wk);
void Passive07_0172(PLW* wk);
void Passive07_0173(PLW* wk);
void Passive07_0174(PLW* wk);
void Passive07_0175(PLW* wk);
void Passive07_0176(PLW* wk);
void Passive07_0177(PLW* wk);
void Passive07_0178(PLW* wk);
void Passive07_0179(PLW* wk);
void Passive07_0180(PLW* wk);
void Passive07_0181(PLW* wk);
void Passive07_0182(PLW* wk);
void Passive07_0183(PLW* wk);
void Passive07_0184(PLW* wk);
void Passive07_0185(PLW* wk);
void Passive07_0186(PLW* wk);
void Passive07_0187(PLW* wk);
void Passive07_0188(PLW* wk);
void Passive07_0189(PLW* wk);
void Passive07_0190(PLW* wk);
void Passive07_0191(PLW* wk);
void Passive07_0192(PLW* wk);
void Passive07_0193(PLW* wk);
void Passive07_0194(PLW* wk);
void Passive07_0195(PLW* wk);
void Passive07_0196(PLW* wk);
void Passive07_0197(PLW* wk);
void Passive07_0198(PLW* wk);
void Passive07_0199(PLW* wk);
void Passive07_0200(PLW* wk);
void Passive07_0201(PLW* wk);
void Passive07_0202(PLW* wk);
void Passive07_0203(PLW* wk);
void Passive07_0204(PLW* wk);
void Passive07_0205(PLW* wk);
void Passive07_0206(PLW* wk);
void Passive07_0207(PLW* wk);
void Passive07_0208(PLW* wk);
void Passive07_0209(PLW* wk);
void Passive07_0210(PLW* wk);
void Passive07_0211(PLW* wk);
void Passive07_0212(PLW* wk);
void Passive07_0213(PLW* wk);

#endif
