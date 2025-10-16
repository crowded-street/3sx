#include "common.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "structs.h"

struct internal_state {
    s32 dummy;
};

#include "zlib.h"

typedef struct {
    struct z_stream_s info;
    s32 state;
    _MEMMAN_OBJ mobj;
} ZLIB;

ZLIB zlib;

void* zlib_Malloc(void*, u32, u32);
void zlib_Free(void*, void*);

void zlib_Initialize(void* tempAdrs, s32 tempSize) {
    if (tempAdrs == NULL) {
        while (1) {}
    }

    mmHeapInitialize(&zlib.mobj, tempAdrs, tempSize, ALIGN_UP(sizeof(_MEMMAN_CELL), 16), "- for zlib -");

    zlib.info.zalloc = zlib_Malloc;
    zlib.info.zfree = zlib_Free;
    zlib.info.opaque = NULL;
}

void* zlib_Malloc(void* opaque, u32 items, u32 size) {
    return mmAlloc(&zlib.mobj, size * items, 0);
}

void zlib_Free(void* opaque, void* adrs) {
    mmFree(&zlib.mobj, (u8*)adrs);
}

ssize_t zlib_Decompress(void* srcBuff, s32 srcSize, void* dstBuff, s32 dstSize) {
    zlib.info.next_in = srcBuff;
    zlib.info.avail_in = srcSize;
    zlib.info.next_out = dstBuff;
    zlib.info.avail_out = dstSize;
    zlib.state = 0;

    if (inflateInit_(&zlib.info, ZLIB_VERSION, sizeof(z_stream)) != 0) {
        return 0;
    }

    while (1) {
        zlib.state = inflate(&zlib.info, 0);

        if (zlib.state == 1) {
            break;
        }

        if (zlib.state == 0) {
            continue;
        } else {
            return 0;
        }
    }

    if (inflateEnd(&zlib.info) != 0) {
        return 0;
    }

    return zlib.info.total_out;
}
