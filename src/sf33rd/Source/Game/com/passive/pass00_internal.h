/*
 * Internal declarations for the pass00 pattern scripts.
 *
 * pass00.c holds the dispatcher and Passive00_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS00_INTERNAL_H
#define PASS00_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive00_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void passive00_pattern_approach_walk_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void passive00_pattern_etc_term_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p);
void passive00_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void Passive00_0051(PLW* wk);
void Passive00_0052(PLW* wk);
void Passive00_0053(PLW* wk);
void Passive00_0054(PLW* wk);
void Passive00_0055(PLW* wk);
void Passive00_0056(PLW* wk);
void Passive00_0057(PLW* wk);
void Passive00_0058(PLW* wk);
void Passive00_0059(PLW* wk);
void Passive00_0060(PLW* wk);
void Passive00_0061(PLW* wk);
void Passive00_0062(PLW* wk);
void Passive00_0063(PLW* wk);
void Passive00_0064(PLW* wk);
void Passive00_0065(PLW* wk);
void Passive00_0066(PLW* wk);
void Passive00_0067(PLW* wk);
void Passive00_0068(PLW* wk);
void Passive00_0069(PLW* wk);
void Passive00_0070(PLW* wk);
void Passive00_0071(PLW* wk);
void Passive00_0072(PLW* wk);
void Passive00_0073(PLW* wk);
void Passive00_0074(PLW* wk);
void Passive00_0075(PLW* wk);
void Passive00_0076(PLW* wk);
void Passive00_0077(PLW* wk);
void Passive00_0078(PLW* wk);
void Passive00_0079(PLW* wk);
void Passive00_0080(PLW* wk);
void Passive00_0081(PLW* wk);
void Passive00_0082(PLW* wk);
void Passive00_0083(PLW* wk);
void Passive00_0084(PLW* wk);
void Passive00_0085(PLW* wk);
void Passive00_0086(PLW* wk);
void Passive00_0087(PLW* wk);
void Passive00_0088(PLW* wk);
void Passive00_0089(PLW* wk);
void Passive00_0090(PLW* wk);
void Passive00_0091(PLW* wk);
void Passive00_0092(PLW* wk);
void Passive00_0093(PLW* wk);
void Passive00_0094(PLW* wk);
void Passive00_0095(PLW* wk);
void Passive00_0096(PLW* wk);
void Passive00_0097(PLW* wk);
void Passive00_0098(PLW* wk);
void Passive00_0099(PLW* wk);
void Passive00_0100(PLW* wk);
void Passive00_0101(PLW* wk);
void Passive00_0102(PLW* wk);
void Passive00_0103(PLW* wk);
void Passive00_0104(PLW* wk);
void Passive00_0105(PLW* wk);
void Passive00_0106(PLW* wk);
void Passive00_0107(PLW* wk);
void Passive00_0108(PLW* wk);
void Passive00_0109(PLW* wk);
void Passive00_0110(PLW* wk);
void Passive00_0111(PLW* wk);
void Passive00_0112(PLW* wk);
void Passive00_0113(PLW* wk);
void Passive00_0114(PLW* wk);
void Passive00_0115(PLW* wk);
void Passive00_0116(PLW* wk);
void Passive00_0117(PLW* wk);
void Passive00_0118(PLW* wk);
void Passive00_0119(PLW* wk);
void Passive00_0120(PLW* wk);
void Passive00_0121(PLW* wk);
void Passive00_0122(PLW* wk);
void Passive00_0123(PLW* wk);
void Passive00_0124(PLW* wk);
void Passive00_0125(PLW* wk);
void Passive00_0126(PLW* wk);
void Passive00_0127(PLW* wk);
void Passive00_0128(PLW* wk);
void Passive00_0129(PLW* wk);
void Passive00_0130(PLW* wk);
void Passive00_0131(PLW* wk);
void Passive00_0132(PLW* wk);
void Passive00_0133(PLW* wk);
void Passive00_0134(PLW* wk);
void Passive00_0135(PLW* wk);
void Passive00_0136(PLW* wk);
void Passive00_0137(PLW* wk);
void Passive00_0138(PLW* wk);
void Passive00_0139(PLW* wk);
void Passive00_0140(PLW* wk);
void Passive00_0141(PLW* wk);
void Passive00_0142(PLW* wk);
void Passive00_0143(PLW* wk);
void Passive00_0144(PLW* wk);
void Passive00_0145(PLW* wk);
void Passive00_0146(PLW* wk);
void Passive00_0147(PLW* wk);
void Passive00_0148(PLW* wk);
void Passive00_0149(PLW* wk);
void Passive00_0150(PLW* wk);
void Passive00_0151(PLW* wk);
void Passive00_0152(PLW* wk);
void Passive00_0153(PLW* wk);
void Passive00_0154(PLW* wk);
void Passive00_0155(PLW* wk);
void Passive00_0156(PLW* wk);
void Passive00_0157(PLW* wk);

#endif
