#include "sf33rd/AcrSDK/common/memmgr.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/prilay.h"

#define ALIGN(ptr, len, alignment) ((~(alignment - 1)) & ((uintptr_t)(ptr) + len + alignment - 1))
#define ALIGN_DOWN(ptr, len, alignment) ((~(alignment - 1)) & ((uintptr_t)(ptr) - len))

static u32 plmemPullHandle(MEM_MGR* memmgr);
static void plmemAppendBlockList(MEM_MGR* memmgr, u32 han);
static void plmemDeleteBlockList(MEM_MGR* memmgr, u32 han);

void plmemInit(MEM_MGR* memmgr, const MemInitArgs* a) {
    memmgr->cnt = a->count;
    memmgr->block = a->block;
    memmgr->memsize = a->memsize;
    memmgr->direction = a->direction;
    memmgr->memalign = a->memalign;

    if (a->direction != 0) {
        memmgr->memptr = (u8*)(~(a->memalign - 1) & ((uintptr_t)a->mem_ptr + a->memalign - 1));
    } else {
        memmgr->memptr = (u8*)(~(a->memalign - 1) & ((uintptr_t)a->mem_ptr + a->memsize));
    }

    memmgr->memnow = memmgr->memptr;
    memmgr->used_size = 0;
    memmgr->tmemsize = 0;
    memmgr->blocklist = MEM_NULL_HANDLE;

    plMemset(a->block, 0, a->count * sizeof(MEM_BLOCK));
}

u32 plmemRegister(MEM_MGR* memmgr, s32 len) {
    return plmemRegisterAlign(memmgr, len, memmgr->memalign);
}

u32 plmemRegisterAlign(MEM_MGR* memmgr, s32 len, s32 align) {
    u32 han;

    if (plmemGetFreeSpace(memmgr) <= len + align) {
        return 0;
    }

    if (len == 0) {
        return 0;
    }

    han = plmemPullHandle(memmgr);

    if (han == MEM_NULL_HANDLE) {
        return 0;
    }

    memmgr->block[han].id = 0;
    memmgr->block[han].len = len;
    memmgr->block[han].align = memmgr->memalign;

    if (memmgr->direction != 0) {
        memmgr->block[han].ptr = memmgr->memnow;
        memmgr->memnow = (u8*)(~(align - 1) & ((uintptr_t)&memmgr->block[han].ptr[len] + align - 1));
        // memmgr->memnow = (u8*)ALIGN(memmgr->block[han].ptr, len, align);
    } else {
        memmgr->block[han].ptr = (u8*)(~(align - 1) & (uintptr_t)(memmgr->memnow - len));
        // memmgr->block[han].ptr = (u8 *)ALIGN_DOWN(memmgr->memnow, len, align);
        memmgr->memnow = memmgr->block[han].ptr;
    }

    memmgr->used_size += len;
    plmemAppendBlockList(memmgr, han);

    return han + 1;
}

/* Claim a pulled handle for a block at `ptr` and hand it back as the caller's
 * return value. The four places plmemRegisterS finds room differ only in the
 * address they found. */
static u32 plmem_claim_block(MEM_MGR* memmgr, u32 han, s32 len, u8* ptr) {
    memmgr->block[han].id = 0;
    memmgr->block[han].len = len;
    memmgr->block[han].align = memmgr->memalign;
    memmgr->block[han].ptr = ptr;
    memmgr->used_size += len;
    plmemAppendBlockList(memmgr, han);
    return han + 1;
}

/* Scanning up the block list for the first gap that fits. Returns the address
 * to claim, or NULL where the list ran out - an address the allocator hands
 * back is never NULL. The claim itself stays at the call site, where it ran. */
static u8* plmem_find_gap_upward(MEM_MGR* memmgr, u32 size, MEM_BLOCK* now_block) {
    size_t len2;
    MEM_BLOCK* next_block;
    u8* data_ptr;

    while (now_block->next != MEM_NULL_HANDLE) {
        next_block = &memmgr->block[now_block->next];
        data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
        len2 = next_block->ptr - data_ptr;

        if (size <= len2) {
            return data_ptr;
        }

        now_block = next_block;
    }

    data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
    len2 = memmgr->memnow - data_ptr;

    if (size <= len2) {
        return data_ptr;
    }

    return NULL;
}

/* The downward mirror of plmem_find_gap_upward: the list runs the other way, so
 * the gap is measured from the current block down to the aligned start of the
 * next one, and the address to claim sits size below the current block. Returns
 * NULL where the list ran out, on the same terms. */
static u8* plmem_find_gap_downward(MEM_MGR* memmgr, u32 size, MEM_BLOCK* now_block) {
    size_t len2;
    MEM_BLOCK* next_block;
    u8* data_ptr;

    while (now_block->next != MEM_NULL_HANDLE) {
        next_block = memmgr->block + now_block->next;
        data_ptr = (u8*)ALIGN(next_block->ptr, next_block->len, memmgr->memalign);
        len2 = now_block->ptr - data_ptr;

        if (size <= len2) {
            return now_block->ptr - size;
        }

        now_block = next_block;
    }

    len2 = now_block->ptr - memmgr->memnow;

    if (size <= len2) {
        return now_block->ptr - size;
    }

    return NULL;
}

u32 plmemRegisterS(MEM_MGR* memmgr, s32 len) {
    u32 han;
    u32 size;
    MEM_BLOCK* now_block;
    u8* data_ptr;

    size = ALIGN(NULL, len, memmgr->memalign);
    han = plmemPullHandle(memmgr);

    if (han == MEM_NULL_HANDLE) {
        return 0;
    }

    if (memmgr->blocklist == MEM_NULL_HANDLE) {
        return plmemRegister(memmgr, len);
    }

    now_block = memmgr->block + memmgr->blocklist;

    if (memmgr->direction != 0) {
        data_ptr = plmem_find_gap_upward(memmgr, size, now_block);
    } else {
        data_ptr = plmem_find_gap_downward(memmgr, size, now_block);
    }

    if (data_ptr != NULL) {
        return plmem_claim_block(memmgr, han, len, data_ptr);
    }

    return plmemRegister(memmgr, len);
}

void* plmemTemporaryUse(MEM_MGR* memmgr, s32 len) {
    size_t tmp;

    len = ALIGN(NULL, len, memmgr->memalign);
    tmp = plmemGetFreeSpace(memmgr);

    if (len > tmp) {
        plmemCompact(memmgr);
        len = ALIGN(NULL, len, memmgr->memalign);
        tmp = plmemGetFreeSpace(memmgr);

        if (len > tmp) {
            return NULL;
        }
    }

    if (memmgr->direction != 0) {
        return memmgr->memptr + memmgr->memsize - len;
    }

    return memmgr->memptr - memmgr->memsize;
}

void* plmemRetrieve(MEM_MGR* memmgr, u32 handle) {
    s32 index = handle - 1;

    if ((index >= memmgr->cnt) || (handle == 0)) {
        return NULL;
    }

    return memmgr->block[index].ptr;
}

s32 plmemRelease(MEM_MGR* memmgr, u32 handle) {
    s32 index = handle - 1;

    if (index >= memmgr->cnt) {
        return 0;
    }

    if (memmgr->block[index].len == 0) {
        return 0;
    }

    memmgr->used_size -= memmgr->block[index].len;
    memmgr->block[index].len = 0;
    memmgr->block[index].ptr = NULL;
    plmemDeleteBlockList(memmgr, index);
    return 1;
}

/* Move a block to the address the compaction wants it at, if it is not there
 * already. The four places plmemCompact does this differ only in which block. */
static void plmem_move_block(MEM_BLOCK* block, u8* data_ptr) {
    if (data_ptr != block->ptr) {
        plMemmove(data_ptr, block->ptr, block->len);
        block->ptr = data_ptr;
    }
}

/* The downward pass of the compaction, lifted out so plmemCompact is a choice
 * between two passes rather than the passes themselves. Its upward twin is left
 * where it stands on purpose: extracting both makes them a duplicate pair that
 * costs more than the bump it removes. */
static u8* plmem_compact_downward(MEM_MGR* memmgr, MEM_BLOCK* now_block) {
    MEM_BLOCK* next_block;
    u8* data_ptr;

    data_ptr = (u8*)ALIGN_DOWN(memmgr->memptr, now_block->len, memmgr->memalign);

    plmem_move_block(now_block, data_ptr);

    while (now_block->next != MEM_NULL_HANDLE) {
        next_block = memmgr->block + now_block->next;
        data_ptr = (u8*)ALIGN_DOWN(now_block->ptr, next_block->len, memmgr->memalign);

        plmem_move_block(next_block, data_ptr);

        now_block = next_block;
    }

    return now_block->ptr;
}

void* plmemCompact(MEM_MGR* memmgr) {
    MEM_BLOCK* now_block;
    MEM_BLOCK* next_block;
    u8* data_ptr;

    if (memmgr->blocklist == MEM_NULL_HANDLE) {
        memmgr->memnow = memmgr->memptr;
        return memmgr->memnow;
    }

    now_block = memmgr->block + memmgr->blocklist;

    if (memmgr->direction != 0) {
        data_ptr = (u8*)ALIGN(memmgr->memptr, 0, memmgr->memalign);

        plmem_move_block(now_block, data_ptr);

        while (now_block->next != MEM_NULL_HANDLE) {
            next_block = memmgr->block + now_block->next;
            data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);

            plmem_move_block(next_block, data_ptr);

            now_block = next_block;
        }

        memmgr->memnow = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
    } else {
        memmgr->memnow = plmem_compact_downward(memmgr, now_block);
    }

    return memmgr->memnow;
}

u32 plmemGetSpace(MEM_MGR* memmgr) {
    return memmgr->memsize - memmgr->used_size;
}

size_t plmemGetFreeSpace(MEM_MGR* memmgr) {
    if (memmgr->direction != 0) {
        return memmgr->memptr + memmgr->memsize - memmgr->memnow - memmgr->tmemsize;
    }

    return memmgr->memnow - (memmgr->memptr - memmgr->memsize) - memmgr->tmemsize;
}

u32 plmemPullHandle(MEM_MGR* memmgr) {
    s32 i;

    for (i = 0; i < memmgr->cnt; i++) {
        if (memmgr->block[i].len == 0) {
            plMemset(&memmgr->block[i], 0, sizeof(MEM_BLOCK));
            return i;
        }
    }

    return MEM_NULL_HANDLE;
}

/* Closing the list round the newly inserted block, where a neighbour exists. */
static void plmem_point_prev_block_at(MEM_MGR* memmgr, u32 now_han, u32 han) {
    MEM_BLOCK* now_block;

    if (now_han != MEM_NULL_HANDLE) {
        now_block = &memmgr->block[now_han];
        now_block->next = han;
    }
}

static void plmem_point_next_block_at(MEM_MGR* memmgr, u32 next_han, u32 han) {
    MEM_BLOCK* next_block;

    if (next_han != MEM_NULL_HANDLE) {
        next_block = &memmgr->block[next_han];
        next_block->prev = han;
    }
}

/* The two orders a block list can be kept in, as the scan sees them. */
static s32 now_block_is_below(const MEM_BLOCK* now_block, const MEM_BLOCK* block_ptr) {
    return now_block->ptr < block_ptr->ptr;
}

static s32 now_block_is_above(const MEM_BLOCK* now_block, const MEM_BLOCK* block_ptr) {
    return now_block->ptr > block_ptr->ptr;
}

/* Walk the block list from its head for as long as the order still holds, or
 * until the list runs out. Returns the handle the walk stopped at, and leaves
 * the handle before it in *now_han_out.
 *
 * Both directions ran this identical loop; only the comparison differed, so the
 * comparison comes in as a predicate. */
static u32 plmem_scan_block_list(
    MEM_MGR* memmgr, MEM_BLOCK* block_ptr, u32* now_han_out,
    s32 (*keep_walking)(const MEM_BLOCK* now_block, const MEM_BLOCK* block_ptr)
) {
    MEM_BLOCK* now_block = &memmgr->block[memmgr->blocklist];
    u32 now_han = MEM_NULL_HANDLE;
    u32 next_han = memmgr->blocklist;

    while (keep_walking(now_block, block_ptr)) {
        now_han = next_han;
        next_han = now_block->next;

        if (next_han == MEM_NULL_HANDLE) {
            break;
        }

        now_block = &memmgr->block[next_han];
    }

    *now_han_out = now_han;
    return next_han;
}

/* Where a new block goes when the list runs upward: at the head if it sorts
 * below the head, otherwise wherever the scan stops. Its downward twin is left
 * inline on purpose - see plmemCompact for why lifting both arms costs more
 * than it saves. */
static u32 plmem_insert_point_ascending(MEM_MGR* memmgr, MEM_BLOCK* block_ptr, MEM_BLOCK* now_block, u32* now_han_out) {
    if (now_block->ptr > block_ptr->ptr) {
        *now_han_out = MEM_NULL_HANDLE;
        return memmgr->blocklist;
    }

    return plmem_scan_block_list(memmgr, block_ptr, now_han_out, now_block_is_below);
}

void plmemAppendBlockList(MEM_MGR* memmgr, u32 han) {
    MEM_BLOCK* block_ptr;
    MEM_BLOCK* now_block;
    u32 next_han;
    u32 now_han;

    block_ptr = &memmgr->block[han];

    if (memmgr->blocklist == MEM_NULL_HANDLE) {
        memmgr->blocklist = han;
        block_ptr->prev = MEM_NULL_HANDLE;
        block_ptr->next = MEM_NULL_HANDLE;
        return;
    }

    now_block = &memmgr->block[memmgr->blocklist];
    now_han = MEM_NULL_HANDLE;
    next_han = memmgr->blocklist;

    if (memmgr->direction != 0) {
        next_han = plmem_insert_point_ascending(memmgr, block_ptr, now_block, &now_han);
    } else {
        if (now_block->ptr < block_ptr->ptr) {
            next_han = memmgr->blocklist;
        } else {
            next_han = plmem_scan_block_list(memmgr, block_ptr, &now_han, now_block_is_above);
        }
    }

    block_ptr->prev = now_han;
    block_ptr->next = next_han;

    plmem_point_prev_block_at(memmgr, now_han, han);
    plmem_point_next_block_at(memmgr, next_han, han);
}

void plmemDeleteBlockList(MEM_MGR* memmgr, u32 han) {
    MEM_BLOCK* now_block;
    MEM_BLOCK* parent;
    MEM_BLOCK* child;

    now_block = &memmgr->block[han];
    parent = NULL;
    child = NULL;

    if (now_block->prev != MEM_NULL_HANDLE) {
        parent = &memmgr->block[now_block->prev];
        parent->next = now_block->next;
    } else {
        memmgr->blocklist = now_block->next;

        if (memmgr->blocklist == MEM_NULL_HANDLE) {
            memmgr->memnow = memmgr->memptr;
        }
    }

    if (now_block->next != MEM_NULL_HANDLE) {
        child = &memmgr->block[now_block->next];
        child->prev = now_block->prev;
    }
}
