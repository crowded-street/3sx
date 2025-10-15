#ifndef EFF02_H
#define EFF02_H

#include "structs.h"
#include "types.h"

typedef struct {
    // total size: 0xC
    u16 se;    // offset 0x0, size 0x2
    u8 hits;   // offset 0x2, size 0x1
    u8 deff;   // offset 0x3, size 0x1
    u8 kezu;   // offset 0x4, size 0x1
    u8 fsin;   // offset 0x5, size 0x1
    u8 status; // offset 0x6, size 0x1
    u8 quake;  // offset 0x7, size 0x1
    u8 dir;    // offset 0x8, size 0x1
    u8 col;    // offset 0x9, size 0x1
    u8 myhix;  // offset 0xA, size 0x1
    u8 emhix;  // offset 0xB, size 0x1
} HMDT;

typedef struct {
    // total size: 0x6
    u16 chix; // offset 0x0, size 0x2
    s16 hx;   // offset 0x2, size 0x2
    s16 hy;   // offset 0x4, size 0x2
} EXPLEM;

extern const s16 hit_mark_dir_table[16];
extern const HMDT hmdt[];
extern const s16 hcct[];
extern const s16 gqdt[][2];
extern const EXPLEM explem[];
extern const EXPLEM explem2[][20];
extern const s16 hit_mark_hosei_table[][2];

void effect_02_move(WORK_Other* ewk);
s32 effect_02_init(WORK* wk, s8 dmgp, s8 mkst, s8 dmrl);

#endif
