/*
 * Shared between mtrans.c and mtrans_buffers.c.
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
