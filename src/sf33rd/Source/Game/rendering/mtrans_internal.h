/*
 * Shared between mtrans.c, mtrans_buffers.c and mtrans_tiles.c.
 *
 * The pattern cache lookups and the LZ extraction moved to mtrans_buffers.c
 * when mtrans.c passed the file size threshold. They had been `static` only
 * because everything lived in one file; nothing the decompilation wrote is
 * widened beyond that.
 */

#ifndef MTRANS_INTERNAL_H
#define MTRANS_INTERNAL_H

#include "structs.h"
#include "types.h"

/* One lookup in an extended pattern cache: which cache, which pattern and
   palette, where to put the slot it resolves to, and the collection entry whose
   map records it. These are get_mltbuf16_ext_2's and get_mltbuf32_ext_2's five
   arguments, which are the same list in the same order with the same types. */
typedef struct {
    MultiTexture* mt;
    u32 code;
    u32 palt;
    s32* ret;
    PatternInstance* cp;
} MltbufExtLookup;

// The tile run a store_* pass walks: the texture it draws from, the entry it
// starts at, how many entries are left, and the position and pattern code it
// carries along. These are the first ten arguments of every store_* function,
// in the order they were written.
typedef struct {
    MultiTexture* mt;
    WORK* wk;
    u32* textbl;
    TileMapEntry* trsptr;
    s32 count;
    s32 flip;
    s32 palo;
    f32 x;
    f32 y;
    PatternCode cc;
} TransRun;

// store_trans_rgb_tiles is the ninth store_* pass and the odd one out: its x, y
// and pattern code are locals it sets up itself rather than arguments, so it
// cannot take a TransRun without its caller inventing three values. It gets a
// struct of its own eight arguments instead, in order and in type.
typedef struct {
    MultiTexture* mt;
    WORK* wk;
    u32* textbl;
    TileMapEntry* trsptr;
    s32 count;
    s32 flip;
    s32 palo;
    s32 group;
} RgbTileRun;

/* The nine tile passes, in mtrans_tiles.c. mtrans.c reaches them through the
   TransVariant and ExtTransVariant tables it builds, and through one direct
   call for the true-colour pass. */
void store_trans_tiles(const TransRun* run);
void store_trans_rgb_tiles(const RgbTileRun* run);
void store_trans_cp3_tiles(const TransRun* run);
void store_cached_trans_ext_tiles(const TransRun* run, s32 group);
void store_new_trans_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp);
void store_cached_trans_cp3_ext_tiles(const TransRun* run, s32 group);
void store_new_trans_cp3_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp);
void store_cached_trans_rgb_ext_tiles(const TransRun* run, s32 group);
void store_new_trans_rgb_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp);

/* Where a tile entry moves the run's position. Both halves walk the same tile
   map, so both need them; they are campaign-created helpers, not linkage the
   decompilation chose. */
f32 advance_trans_x(f32 x, s32 flip, TileMapEntry* trsptr);
f32 advance_trans_y(f32 y, s32 flip, TileMapEntry* trsptr);

s16 check_patcash_ex_trans(PatternCollection* padr, u32 cg);
s32 get_free_patcash_index(PatternCollection* padr);
s32 get_mltbuf16(MultiTexture* mt, u32 code, u32 palt, s32* ret);
s32 get_mltbuf16_ext(MultiTexture* mt, u32 code, u32 palt);
s32 get_mltbuf16_ext_2(const MltbufExtLookup* look);
s32 get_mltbuf32(MultiTexture* mt, u32 code, u32 palt, s32* ret);
s32 get_mltbuf32_ext(MultiTexture* mt, u32 code, u32 palt);
s32 get_mltbuf32_ext_2(const MltbufExtLookup* look);
void lz_ext_p6_fx(u8* srcptr, u8* dstptr, u32 len);
void lz_ext_p6_cx(u8* srcptr, u16* dstptr, u32 len, u16* palptr);
u16 x16_mapping_set(PatternMap* map, s32 code);
u16 x32_mapping_set(PatternMap* map, s32 code);

#endif
