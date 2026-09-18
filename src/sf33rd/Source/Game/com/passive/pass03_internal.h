/*
 * Internal declarations for the pass03 pattern scripts.
 *
 * pass03.c holds the dispatcher and Passive03_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS03_INTERNAL_H
#define PASS03_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void Passive03_0049(PLW* wk);
void Passive03_0050(PLW* wk);
void Passive03_0051(PLW* wk);
void Passive03_0052(PLW* wk);
void Passive03_0053(PLW* wk);
void Passive03_0054(PLW* wk);
void Passive03_0055(PLW* wk);
void Passive03_0056(PLW* wk);
void Passive03_0057(PLW* wk);
void Passive03_0058(PLW* wk);
void Passive03_0059(PLW* wk);
void Passive03_0060(PLW* wk);
void Passive03_0061(PLW* wk);
void Passive03_0062(PLW* wk);
void Passive03_0063(PLW* wk);
void Passive03_0064(PLW* wk);
void Passive03_0065(PLW* wk);
void Passive03_0066(PLW* wk);
void Passive03_0067(PLW* wk);
void Passive03_0068(PLW* wk);
void Passive03_0069(PLW* wk);
void Passive03_0070(PLW* wk);
void Passive03_0071(PLW* wk);
void Passive03_0072(PLW* wk);
void Passive03_0073(PLW* wk);
void Passive03_0074(PLW* wk);
void Passive03_0075(PLW* wk);
void Passive03_0076(PLW* wk);
void Passive03_0077(PLW* wk);
void Passive03_0078(PLW* wk);
void Passive03_0079(PLW* wk);
void Passive03_0080(PLW* wk);
void Passive03_0081(PLW* wk);
void Passive03_0082(PLW* wk);
void Passive03_0083(PLW* wk);
void Passive03_0084(PLW* wk);
void Passive03_0085(PLW* wk);
void Passive03_0086(PLW* wk);
void Passive03_0087(PLW* wk);
void Passive03_0088(PLW* wk);
void Passive03_0089(PLW* wk);
void Passive03_0090(PLW* wk);
void Passive03_0091(PLW* wk);
void Passive03_0092(PLW* wk);
void Passive03_0093(PLW* wk);
void Passive03_0094(PLW* wk);
void Passive03_0095(PLW* wk);
void Passive03_0096(PLW* wk);
void Passive03_0097(PLW* wk);
void Passive03_0098(PLW* wk);
void Passive03_0099(PLW* wk);
void Passive03_0100(PLW* wk);
void Passive03_0101(PLW* wk);
void Passive03_0102(PLW* wk);
void Passive03_0103(PLW* wk);
void Passive03_0104(PLW* wk);
void Passive03_0105(PLW* wk);
void Passive03_0106(PLW* wk);
void Passive03_0107(PLW* wk);
void Passive03_0108(PLW* wk);
void Passive03_0109(PLW* wk);
void Passive03_0110(PLW* wk);
void Passive03_0111(PLW* wk);
void Passive03_0112(PLW* wk);
void Passive03_0113(PLW* wk);
void Passive03_0114(PLW* wk);
void Passive03_0115(PLW* wk);
void Passive03_0116(PLW* wk);
void Passive03_0117(PLW* wk);
void Passive03_0118(PLW* wk);
void Passive03_0119(PLW* wk);
void Passive03_0120(PLW* wk);
void Passive03_0121(PLW* wk);
void Passive03_0122(PLW* wk);
void Passive03_0123(PLW* wk);
void Passive03_0124(PLW* wk);
void Passive03_0125(PLW* wk);
void Passive03_0126(PLW* wk);
void Passive03_0127(PLW* wk);
void Passive03_0128(PLW* wk);
void Passive03_0129(PLW* wk);
void Passive03_0130(PLW* wk);
void Passive03_0131(PLW* wk);
void Passive03_0132(PLW* wk);
void Passive03_0133(PLW* wk);
void Passive03_0134(PLW* wk);
void Passive03_0135(PLW* wk);

#endif
