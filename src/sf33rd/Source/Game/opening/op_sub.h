#ifndef OP_SUB_H
#define OP_SUB_H

#include "structs.h"
#include "types.h"

void TexRelease(u32 G_Num);
void TexRelease_OP();
void put_chr2(OPTW* optw);
void opbg_trans(OPBW* opbw, s16 x, s16 y);
void COLOR_COPYn(s16 dst, s16 colcd, s16 n);

#endif
