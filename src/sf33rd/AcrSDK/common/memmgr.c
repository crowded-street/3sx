#include "sf33rd/AcrSDK/common/memmgr.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/prilay.h"

#define ALIGN(ptr, len, alignment) ((~(alignment - 1)) & ((uintptr_t)(ptr) + len + alignment - 1))
#define ALIGN_DOWN(ptr, len, alignment) ((~(alignment - 1)) & ((uintptr_t)(ptr) - len))

static u32 plmemPullHandle(MEM_MGR* memmgr);
static void plmemAppendBlockList(MEM_MGR* memmgr, u32 han);
static void plmemDeleteBlockList(MEM_MGR* memmgr, u32 han);

void plmemInit(MEM_MGR* memmgr, MEM_BLOCK* block, s32 count, void* mem_ptr, s32 memsize, s32 memalign, s32 direction) {
    memmgr->cnt = count;
    memmgr->block = block;
    memmgr->memsize = memsize;
    memmgr->direction = direction;
    memmgr->memalign = memalign;

    if (direction != 0) {
        memmgr->memptr = (u8*)(~(memalign - 1) & ((uintptr_t)mem_ptr + memalign - 1));
    } else {
        memmgr->memptr = (u8*)(~(memalign - 1) & ((uintptr_t)mem_ptr + memsize));
    }

    memmgr->memnow = memmgr->memptr;
    memmgr->used_size = 0;
    memmgr->tmemsize = 0;
    memmgr->blocklist = MEM_NULL_HANDLE;

    plMemset(block, 0, count * sizeof(MEM_BLOCK));
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

u32 plmemRegisterS(MEM_MGR* memmgr, s32 len) {
    u32 han;
    size_t len2;
    u32 size;
    MEM_BLOCK* now_block;
    MEM_BLOCK* next_block;
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
        while (now_block->next != MEM_NULL_HANDLE) {
            next_block = &memmgr->block[now_block->next];
            data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
            len2 = next_block->ptr - data_ptr;

            if (size <= len2) {
                memmgr->block[han].id = 0;
                memmgr->block[han].len = len;
                memmgr->block[han].align = memmgr->memalign;
                memmgr->block[han].ptr = data_ptr;
                memmgr->used_size += len;
                plmemAppendBlockList(memmgr, han);
                return han + 1;
            }

            now_block = next_block;
        }

        data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
        len2 = memmgr->memnow - data_ptr;

        if (size <= len2) {
            memmgr->block[han].id = 0;
            memmgr->block[han].len = len;
            memmgr->block[han].align = memmgr->memalign;
            memmgr->block[han].ptr = data_ptr;
            memmgr->used_size += len;
            plmemAppendBlockList(memmgr, han);
            return han + 1;
        }
    } else {
        while (now_block->next != MEM_NULL_HANDLE) {
            next_block = memmgr->block + now_block->next;
            data_ptr = (u8*)ALIGN(next_block->ptr, next_block->len, memmgr->memalign);
            len2 = now_block->ptr - data_ptr;

            if (size <= len2) {
                memmgr->block[han].id = 0;
                memmgr->block[han].len = len;
                memmgr->block[han].align = memmgr->memalign;
                memmgr->block[han].ptr = now_block->ptr - size;
                memmgr->used_size += len;
                plmemAppendBlockList(memmgr, han);
                return han + 1;
            }

            now_block = next_block;
        }

        len2 = now_block->ptr - memmgr->memnow;

        if (size <= len2) {
            memmgr->block[han].id = 0;
            memmgr->block[han].len = len;
            memmgr->block[han].align = memmgr->memalign;
            memmgr->block[han].ptr = now_block->ptr - size;
            memmgr->used_size += len;
            plmemAppendBlockList(memmgr, han);
            return han + 1;
        }
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

        if (data_ptr != now_block->ptr) {
            plMemmove(data_ptr, now_block->ptr, now_block->len);
            now_block->ptr = data_ptr;
        }

        while (now_block->next != MEM_NULL_HANDLE) {
            next_block = memmgr->block + now_block->next;
            data_ptr = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);

            if (data_ptr != next_block->ptr) {
                plMemmove(data_ptr, next_block->ptr, next_block->len);
                next_block->ptr = data_ptr;
            }

            now_block = next_block;
        }

        memmgr->memnow = (u8*)ALIGN(now_block->ptr, now_block->len, memmgr->memalign);
    } else {
        data_ptr = (u8*)ALIGN_DOWN(memmgr->memptr, now_block->len, memmgr->memalign);

        if (data_ptr != now_block->ptr) {
            plMemmove(data_ptr, now_block->ptr, now_block->len);
            now_block->ptr = data_ptr;
        }

        while (now_block->next != MEM_NULL_HANDLE) {
            next_block = memmgr->block + now_block->next;
            data_ptr = (u8*)ALIGN_DOWN(now_block->ptr, next_block->len, memmgr->memalign);

            if (data_ptr != next_block->ptr) {
                plMemmove(data_ptr, next_block->ptr, next_block->len);
                next_block->ptr = data_ptr;
            }

            now_block = next_block;
        }

        memmgr->memnow = now_block->ptr;
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

void plmemAppendBlockList(MEM_MGR* memmgr, u32 han) {
    MEM_BLOCK* block_ptr;
    MEM_BLOCK* next_block;
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
        if (now_block->ptr > block_ptr->ptr) {
            next_han = memmgr->blocklist;
        } else {
            while (now_block->ptr < block_ptr->ptr) {
                now_han = next_han;
                next_han = now_block->next;

                if (next_han == MEM_NULL_HANDLE) {
                    break;
                }

                now_block = &memmgr->block[next_han];
            }
        }
    } else {
        if (now_block->ptr < block_ptr->ptr) {
            next_han = memmgr->blocklist;
        } else {
            while (now_block->ptr > block_ptr->ptr) {
                now_han = next_han;
                next_han = now_block->next;

                if (next_han == MEM_NULL_HANDLE) {
                    break;
                }

                now_block = &memmgr->block[next_han];
            }
        }
    }

    block_ptr->prev = now_han;
    block_ptr->next = next_han;

    if (now_han != MEM_NULL_HANDLE) {
        now_block = &memmgr->block[now_han];
        now_block->next = han;
    }

    if (next_han != MEM_NULL_HANDLE) {
        next_block = &memmgr->block[next_han];
        next_block->prev = han;
    }
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
