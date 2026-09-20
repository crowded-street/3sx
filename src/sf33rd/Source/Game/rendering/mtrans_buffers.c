/**
 * @file mtrans_buffers.c
 * The multi-texture buffer pool, the pattern cache lookups over it, and the
 * LZ extraction that fills it. Split out of mtrans.c, which had passed the
 * file size threshold; everything here reaches its state through the
 * MultiTexture it is given, so the cut needed no file-scope state to move.
 */

#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/mtrans_internal.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "structs.h"

#include <SDL3/SDL.h>

static bool is_cached_pattern_state(PatternState* mc, u32 code, u32 palt) {
    return (mc->cs.code == code) && (mc->state == palt);
}

static bool has_free_pattern_slot(s32 i, s32 pattern_count, s32 free_count) {
    return (i != pattern_count) && (free_count != 0);
}

static bool is_first_available_pattern_slot(PatternState* mc, s32 free_index) {
    return (mc->cs.code == -1) && (free_index < 0);
}

/* One of the two pattern caches, as the claim sees it: where it starts, how many
 * slots it has, the time stamp a fresh slot is given, and what the log says when
 * there is no slot to give. */
typedef struct {
    MultiTexture* mt;
    PatternState* cache;
    s32 count;
    s32 time;
    const char* full_message;
} MltbufClaimBank;

/* Claiming the free slot the search found, for either cache. The bank's count
 * and time are read at the call site, where the originals read mt->mltnum and
 * mt->mltcshtime inside the branch; nothing runs in between. */
static s32 claim_mltbuf_slot(const MltbufClaimBank* bank, s32 b, u32 code, u32 palt) {
    if (b >= 0) {
        b = bank->count - b;
        bank->cache[b].time = bank->time;
        bank->cache[b].state = palt;
        bank->cache[b].cs.code = code;
        return b;
    }

    flLogOut(bank->full_message, bank->mt->id);
    while (1) {}
}

static s32 claim_mltbuf16_slot(MultiTexture* mt, s32 b, u32 code, u32 palt) {
    // CG cache is full. 16x16: %d\n
    return claim_mltbuf_slot(
        &(MltbufClaimBank) {
            mt, mt->mltcsh16, mt->mltnum16, mt->mltcshtime16, "ＣＧキャッシュが一杯になりました。１６×１６ : %d\n" },
        b,
        code,
        palt
    );
}

/* One of the two multi-texture pattern caches, with everything the search needs
 * to work it: where the cache starts, how many slots it has, the time stamp a
 * hit refreshes to, and how a miss claims a slot. The two halves read these
 * from mt themselves; holding them here keeps the search at four arguments. */
typedef struct {
    MultiTexture* mt;
    PatternState* cache;
    s32 count;
    s32 time;
    s32 (*claim_slot)(MultiTexture* mt, s32 b, u32 code, u32 palt);
} MltbufBank;

/* The search get_mltbuf16 and get_mltbuf32 share: walk the cache down from its
 * last slot, return on a hit, remember the first free slot on the way, and
 * claim one when the walk runs out.
 *
 * The bank's count and time are read once at the call site where the originals
 * read mt->mltnum and mt->mltcshtime on each pass. Nothing between those reads
 * writes either field - the loop calls only is_cached_pattern_state and
 * is_first_available_pattern_slot, and the claim happens on the way out - so
 * every read still sees the same value it saw before. */
static s32 get_mltbuf(const MltbufBank* bank, u32 code, u32 palt, s32* ret) {
    s32 i;
    s32 b = -1;
    PatternState* mc = bank->cache;

    i = bank->count;

    while (1) {
        if (is_cached_pattern_state(mc, code, palt)) {
            mc->time = bank->time;
            *ret = bank->count - i;
            return 0;
        }

        if (is_first_available_pattern_slot(mc, b)) {
            b = i;
        }

        mc++;
        i -= 1;

        if (i <= 0) {
            *ret = bank->claim_slot(bank->mt, b, code, palt);
            return 1;
        }
    }
}

s32 get_mltbuf16(MultiTexture* mt, u32 code, u32 palt, s32* ret) {
    return get_mltbuf(
        &(MltbufBank) { mt, mt->mltcsh16, mt->mltnum16, mt->mltcshtime16, claim_mltbuf16_slot }, code, palt, ret
    );
}

static s32 claim_mltbuf32_slot(MultiTexture* mt, s32 b, u32 code, u32 palt) {
    // CG cache is full. 32x32 : %d\n
    return claim_mltbuf_slot(
        &(MltbufClaimBank) {
            mt, mt->mltcsh32, mt->mltnum32, mt->mltcshtime32, "ＣＧキャッシュが一杯になりました。３２×３２ : %d\n" },
        b,
        code,
        palt
    );
}

s32 get_mltbuf32(MultiTexture* mt, u32 code, u32 palt, s32* ret) {
    return get_mltbuf(
        &(MltbufBank) { mt, mt->mltcsh32, mt->mltnum32, mt->mltcshtime32, claim_mltbuf32_slot }, code, palt, ret
    );
}

// Take the next free 16x16 slot, record the pattern in it, and note it in the
// collection's map.
static s32 claim_free_x16_slot(const MltbufExtLookup* look, PatternState* mc, s32 i) {
    look->mt->tpf->x16 -= 1;
    look->mt->tpu->x16_used[i] = look->mt->tpf->x16_free[look->mt->tpf->x16];
    look->mt->tpu->x16 += 1;
    mc[look->mt->tpu->x16_used[i]].cs.code = look->code;
    mc[look->mt->tpu->x16_used[i]].state = look->palt;
    *look->ret = look->mt->tpu->x16_used[i];
    mc[look->mt->tpu->x16_used[i]].time = 1;

    if (x16_mapping_set(&look->cp->map, *look->ret)) {
        look->cp->x16 += 1;
    }

    return 1;
}

s32 get_mltbuf16_ext_2(const MltbufExtLookup* look) {
    PatternState* mc = look->mt->mltcsh16;
    s32 i;

    for (i = 0; i < look->mt->tpu->x16; i++) {
        if ((look->code == mc[look->mt->tpu->x16_used[i]].cs.code) &&
            (look->palt == mc[look->mt->tpu->x16_used[i]].state)) {
            *look->ret = look->mt->tpu->x16_used[i];

            if (x16_mapping_set(&look->cp->map, *look->ret)) {
                look->cp->x16 += 1;
                mc[look->mt->tpu->x16_used[i]].time += 1;
            }

            return 0;
        }
    }

    if (has_free_pattern_slot(i, look->mt->mltnum16, look->mt->tpf->x16)) {
        return claim_free_x16_slot(look, mc, i);
    }

    // CG cache is full. x16 EXT2\n
    flLogOut("ＣＧキャッシュが一杯になりました。×１６　ＥＸＴ２\n");
    while (1) {}
}

// The 32x32 twin of claim_free_x16_slot. Note the `time += 1` where the 16x16
// one writes `time = 1`; the asymmetry is in the original and is left as it is.
static s32 claim_free_x32_slot(const MltbufExtLookup* look, PatternState* mc, s32 i) {
    look->mt->tpf->x32 -= 1;
    look->mt->tpu->x32_used[i] = look->mt->tpf->x32_free[look->mt->tpf->x32];
    look->mt->tpu->x32 += 1;
    mc[look->mt->tpu->x32_used[i]].cs.code = look->code;
    mc[look->mt->tpu->x32_used[i]].state = look->palt;
    *look->ret = look->mt->tpu->x32_used[i];
    mc[look->mt->tpu->x32_used[i]].time += 1;

    if (x32_mapping_set(&look->cp->map, *look->ret)) {
        look->cp->x32 += 1;
    }

    return 1;
}

s32 get_mltbuf32_ext_2(const MltbufExtLookup* look) {
    PatternState* mc = look->mt->mltcsh32;
    s32 i;

    for (i = 0; i < look->mt->tpu->x32; i++) {
        if ((look->code == mc[look->mt->tpu->x32_used[i]].cs.code) &&
            (look->palt == mc[look->mt->tpu->x32_used[i]].state)) {
            *look->ret = look->mt->tpu->x32_used[i];

            if (x32_mapping_set(&look->cp->map, *look->ret)) {
                look->cp->x32 += 1;
                mc[look->mt->tpu->x32_used[i]].time += 1;
            }

            return 0;
        }
    }

    if (has_free_pattern_slot(i, look->mt->mltnum32, look->mt->tpf->x32)) {
        return claim_free_x32_slot(look, mc, i);
    }

    flLogOut("ＣＧキャッシュが一杯になりました。×３２　ＥＸＴ２\n");
    while (1) {}
}

/* One of the two extended caches, as the lookup sees it: where it starts, how
 * many slots the pool has in use, which slots those are, and what the log says
 * when the pattern is not among them.
 *
 * The count is a pointer, not a value, because the originals re-read
 * tpu_free->x16 on every pass of the loop. */
typedef struct {
    PatternState* cache;
    const s32* count;
    const u16* used;
    const char* missing_message;
} MltbufExtBank;

static s32 get_mltbuf_ext(const MltbufExtBank* bank, u32 code, u32 palt) {
    s32 i;

    for (i = 0; i < *bank->count; i++) {
        if ((code == bank->cache[bank->used[i]].cs.code) && (palt == bank->cache[bank->used[i]].state)) {
            return bank->used[i];
        }
    }

    flLogOut(bank->missing_message);
    while (1) {}
}

s32 get_mltbuf16_ext(MultiTexture* mt, u32 code, u32 palt) {
    return get_mltbuf_ext(
        &(MltbufExtBank) { mt->mltcsh16, &tpu_free->x16, tpu_free->x16_used, "ＣＧ展開エラー　１６×１６\n" }, code, palt
    );
}

s32 get_mltbuf32_ext(MultiTexture* mt, u32 code, u32 palt) {
    return get_mltbuf_ext(
        &(MltbufExtBank) { mt->mltcsh32, &tpu_free->x32, tpu_free->x32_used, "ＣＧ展開エラー　３２×３２\n" }, code, palt
    );
}

u16 x16_mapping_set(PatternMap* map, s32 code) {
    u16 num;
    u16 flg;

    flg = 0;
    num = code & 0xF;

    if (!((1 << (num)) & (map->x16_map[code / 256][(code % 256) / 16]))) {
        map->x16_map[code / 256][(code % 256) / 16] |= (1 << num);
        flg = 1;
    }

    return flg;
}

u16 x32_mapping_set(PatternMap* map, s32 code) {
    u16 flg = 0;
    u8 num = code & 7;

    if (!((map->x32_map[code / 64][(code % 64) / 8]) & (1 << num))) {
        map->x32_map[code / 64][(code % 64) / 8] |= (1 << num);
        flg = 1;
    }

    return flg;
}

s16 check_patcash_ex_trans(PatternCollection* padr, u32 cg) {
    s16 rnum = -1;
    s16 i;

    for (i = 0; i < padr->kazu; i++) {
        if (padr->adr[i]->cg.code == cg) {
            rnum = i;
            break;
        }
    }

    return rnum;
}

s32 get_free_patcash_index(PatternCollection* padr) {
    s16 i;

    for (i = 0; i < 0x40; i++) {
        if (padr->patt[i].time == 0) {
            return i;
        }
    }

    flLogOut("ＣＧキャッシュバッファが一杯になりました。\n");
    while (1) {}
}

// Both of lz_ext_p6_fx's back-reference cases end in this copy. Only dstptr
// outlives it - tmpptr and tmp are reassigned before they are read again - so
// the run comes back as the advanced write pointer.
static u8* copy_lz_run_8(u8* dstptr, u8* tmpptr, u32 tmp) {
    while (tmp--) {
        *dstptr++ = *tmpptr++;
    }

    return dstptr;
}

void lz_ext_p6_fx(u8* srcptr, u8* dstptr, u32 len) {
    u8* endptr = dstptr + len;
    u8* tmpptr;
    u32 tmp;
    u32 flg;

    while (dstptr < endptr) {
        tmp = *srcptr++;

        switch (tmp & 0xC0) {
        case 0x0:
            *dstptr++ = tmp;
            break;

        case 0x40:
            tmp &= 0x3F;
            tmpptr = (dstptr - (tmp >> 2)) - 1;
            tmp = (tmp & 3) + 2;

            dstptr = copy_lz_run_8(dstptr, tmpptr, tmp);

            break;

        case 0x80:
            tmp = ((tmp & 0x3F) << 8) | *srcptr++;
            tmpptr = (dstptr - (tmp >> 6)) - 1;
            tmp = (tmp & 0x3F) + 2;

            dstptr = copy_lz_run_8(dstptr, tmpptr, tmp);

            break;

        case 0xC0:
            flg = tmp & 0x30;
            tmp = (tmp & 0xF) + 2;

            while (tmp--) {
                *dstptr++ = flg | (*srcptr >> 4);
                *dstptr++ = flg | (*srcptr++ & 0xF);
            }

            break;
        }
    }
}

// The u16 twin of copy_lz_run_8. The two cannot be one function: these pointers
// are u16 and those are u8.
static u16* copy_lz_run_16(u16* dstptr, u16* tmpptr, u32 tmp) {
    while (tmp--) {
        *dstptr++ = *tmpptr++;
    }

    return dstptr;
}

void lz_ext_p6_cx(u8* srcptr, u16* dstptr, u32 len, u16* palptr) {
    u16* endptr = dstptr + len;
    u16* tmpptr;
    u32 tmp;
    u32 flg;

    while (dstptr < endptr) {
        tmp = *srcptr++;

        switch (tmp & 0xC0) {
        case 0x0:
            *dstptr++ = palptr[tmp];
            break;

        case 0x40:
            tmp &= 0x3F;
            tmpptr = (dstptr - (tmp >> 2)) - 1;
            tmp = (tmp & 3) + 2;

            dstptr = copy_lz_run_16(dstptr, tmpptr, tmp);

            break;

        case 0x80:
            tmp = ((tmp & 0x3F) << 8) | *srcptr++;
            tmpptr = (dstptr - (tmp >> 6)) - 1;
            tmp = (tmp & 0x3F) + 2;

            dstptr = copy_lz_run_16(dstptr, tmpptr, tmp);

            break;

        case 0xC0:
            flg = tmp & 0x30;
            tmp = (tmp & 0xF) + 2;

            while (tmp--) {
                *dstptr++ = palptr[flg | (*srcptr >> 4)];
                *dstptr++ = palptr[flg | (*srcptr++ & 0xF)];
            }

            break;
        }
    }
}
