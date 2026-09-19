/*
 * Internal declarations for the active14 pattern scripts.
 *
 * active14.c holds the dispatcher and Pattern14_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef ACTIVE14_INTERNAL_H
#define ACTIVE14_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void pattern14_0063_from_step_6(PLW* wk);
void Pattern14_0063(PLW* wk);
void Pattern14_0064(PLW* wk);
void Pattern14_0065(PLW* wk);
void Pattern14_0066(PLW* wk);
void Pattern14_0067(PLW* wk);
void Pattern14_0068(PLW* wk);
void Pattern14_0069(PLW* wk);
void Pattern14_0070(PLW* wk);
void Pattern14_0071(PLW* wk);
void Pattern14_0072(PLW* wk);
void Pattern14_0073(PLW* wk);
void Pattern14_0074(PLW* wk);
void Pattern14_0075(PLW* wk);
void Pattern14_0076(PLW* wk);
void Pattern14_0077(PLW* wk);
void Pattern14_0078(PLW* wk);
void Pattern14_0079(PLW* wk);
void Pattern14_0080(PLW* wk);
void Pattern14_0081(PLW* wk);
void Pattern14_0082(PLW* wk);
void Pattern14_0083(PLW* wk);
void Pattern14_0084(PLW* wk);
void Pattern14_0085(PLW* wk);
void Pattern14_0086(PLW* wk);
void Pattern14_0087(PLW* wk);
void Pattern14_0088(PLW* wk);
void Pattern14_0089(PLW* wk);
void Pattern14_0090(PLW* wk);
void Pattern14_0091(PLW* wk);
void Pattern14_0092(PLW* wk);
void Pattern14_0093(PLW* wk);
void Pattern14_0094(PLW* wk);
void Pattern14_0095(PLW* wk);
void Pattern14_0096(PLW* wk);
void Pattern14_0097(PLW* wk);
void Pattern14_0098(PLW* wk);
void Pattern14_0099(PLW* wk);
void Pattern14_0100(PLW* wk);
void Pattern14_0101(PLW* wk);
void Pattern14_0102(PLW* wk);
void Pattern14_0103(PLW* wk);
void Pattern14_0104(PLW* wk);
void Pattern14_0105(PLW* wk);
void Pattern14_0106(PLW* wk);
void Pattern14_0107(PLW* wk);
void Pattern14_0108(PLW* wk);
void Pattern14_0109(PLW* wk);
void Pattern14_0110(PLW* wk);
void Pattern14_0111(PLW* wk);
void Pattern14_0112(PLW* wk);
void Pattern14_0113(PLW* wk);
void Pattern14_0114(PLW* wk);
void Pattern14_0115(PLW* wk);
void Pattern14_0116(PLW* wk);
void Pattern14_0117(PLW* wk);
void Pattern14_0118(PLW* wk);
void Pattern14_0119(PLW* wk);
void Pattern14_0120(PLW* wk);
void Pattern14_0121(PLW* wk);
void Pattern14_0122(PLW* wk);
void Pattern14_0123(PLW* wk);
void Pattern14_0124(PLW* wk);
void Pattern14_0125(PLW* wk);
void Pattern14_0126(PLW* wk);
void Pattern14_0127(PLW* wk);
void Pattern14_0128(PLW* wk);
void Pattern14_0129(PLW* wk);
void Pattern14_0130(PLW* wk);
void Pattern14_0131(PLW* wk);
void Pattern14_0132(PLW* wk);
void Pattern14_0133(PLW* wk);
void Pattern14_0134(PLW* wk);
void Pattern14_0135(PLW* wk);
void Pattern14_0136(PLW* wk);
void Pattern14_0137(PLW* wk);
void Pattern14_0138(PLW* wk);
void Pattern14_0139(PLW* wk);
void Pattern14_0140(PLW* wk);
void Pattern14_0141(PLW* wk);
void Pattern14_0142(PLW* wk);
void Pattern14_0143(PLW* wk);
void Pattern14_0144(PLW* wk);
void Pattern14_0145(PLW* wk);
void Pattern14_0146(PLW* wk);
void Pattern14_0147(PLW* wk);
void Pattern14_0148(PLW* wk);
void Pattern14_0149(PLW* wk);
void Pattern14_0150(PLW* wk);

#endif
