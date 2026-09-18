#ifndef MTRANS_SEQS_H
#define MTRANS_SEQS_H

#include "core/render_primitives.h"
#include "structs.h"
#include "types.h"

#define PRIO_BASE_SIZE 128

typedef struct {
    Sprite2* chip;
    u16 sprTotal;
    u16 sprMax;
    s8 up[24];
} SpriteChipSet;

extern SpriteChipSet seqs_w;
extern f32 PrioBaseOriginal[PRIO_BASE_SIZE];

// Defined in mtrans.c, where the transfer passes set it before queueing chips.
extern s32 curr_bright;

s32 seqsStoreChip(f32 x, f32 y, s32 w, s32 h, s32 gix, s32 code, s32 attr, s32 alpha, s32 id);
void appRenewTempPriority(s32 z);
void appRenewTempPriority_1_Chip();

#endif
