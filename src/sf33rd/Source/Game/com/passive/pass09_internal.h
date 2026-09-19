/*
 * Internal declarations for the pass09 pattern scripts.
 *
 * pass09.c holds the dispatcher and Passive09_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS09_INTERNAL_H
#define PASS09_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive09_pattern_oro_ja_term(PLW* wk, const ORO_Air_Term_Args* a);
void passive09_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void passive09_pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void Passive09_0051(PLW* wk);
void Passive09_0052(PLW* wk);
void Passive09_0053(PLW* wk);
void Passive09_0054(PLW* wk);
void Passive09_0055(PLW* wk);
void Passive09_0056(PLW* wk);
void Passive09_0057(PLW* wk);
void Passive09_0058(PLW* wk);
void Passive09_0059(PLW* wk);
void Passive09_0060(PLW* wk);
void Passive09_0061(PLW* wk);
void Passive09_0062(PLW* wk);
void Passive09_0063(PLW* wk);
void Passive09_0064(PLW* wk);
void Passive09_0065(PLW* wk);
void Passive09_0066(PLW* wk);
void Passive09_0067(PLW* wk);
void Passive09_0068(PLW* wk);
void Passive09_0069(PLW* wk);
void Passive09_0070(PLW* wk);
void Passive09_0071(PLW* wk);
void Passive09_0072(PLW* wk);
void Passive09_0073(PLW* wk);
void Passive09_0074(PLW* wk);
void Passive09_0075(PLW* wk);
void Passive09_0076(PLW* wk);
void Passive09_0077(PLW* wk);
void Passive09_0078(PLW* wk);
void Passive09_0079(PLW* wk);
void Passive09_0080(PLW* wk);
void Passive09_0081(PLW* wk);
void Passive09_0082(PLW* wk);
void Passive09_0083(PLW* wk);
void Passive09_0084(PLW* wk);
void Passive09_0085(PLW* wk);
void Passive09_0086(PLW* wk);
void Passive09_0087(PLW* wk);
void Passive09_0088(PLW* wk);
void Passive09_0089(PLW* wk);
void Passive09_0090(PLW* wk);
void Passive09_0091(PLW* wk);
void Passive09_0092(PLW* wk);
void Passive09_0093(PLW* wk);
void Passive09_0094(PLW* wk);
void Passive09_0095(PLW* wk);
void Passive09_0096(PLW* wk);
void Passive09_0097(PLW* wk);
void Passive09_0098(PLW* wk);
void Passive09_0099(PLW* wk);
void Passive09_0100(PLW* wk);
void Passive09_0101(PLW* wk);
void Passive09_0102(PLW* wk);
void Passive09_0103(PLW* wk);
void Passive09_0104(PLW* wk);
void Passive09_0105(PLW* wk);
void Passive09_0106(PLW* wk);
void Passive09_0107(PLW* wk);
void Passive09_0108(PLW* wk);
void Passive09_0109(PLW* wk);
void Passive09_0110(PLW* wk);
void Passive09_0111(PLW* wk);
void Passive09_0112(PLW* wk);
void Passive09_0113(PLW* wk);
void Passive09_0114(PLW* wk);
void Passive09_0115(PLW* wk);
void Passive09_0116(PLW* wk);
void Passive09_0117(PLW* wk);
void Passive09_0118(PLW* wk);
void Passive09_0119(PLW* wk);
void Passive09_0120(PLW* wk);
void Passive09_0121(PLW* wk);
void Passive09_0122(PLW* wk);
void Passive09_0123(PLW* wk);
void Passive09_0124(PLW* wk);
void Passive09_0125(PLW* wk);
void Passive09_0126(PLW* wk);
void Passive09_0127(PLW* wk);
void Passive09_0128(PLW* wk);
void Passive09_0129(PLW* wk);
void Passive09_0130(PLW* wk);
void Passive09_0131(PLW* wk);

#endif
