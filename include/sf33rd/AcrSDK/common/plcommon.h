#ifndef PLCOMMON_H
#define PLCOMMON_H

#include "types.h"

typedef struct {
    // total size: 0x30
    s32 rl; // offset 0x0, size 0x4
    s32 rs; // offset 0x4, size 0x4
    s32 rm; // offset 0x8, size 0x4
    s32 gl; // offset 0xC, size 0x4
    s32 gs; // offset 0x10, size 0x4
    s32 gm; // offset 0x14, size 0x4
    s32 bl; // offset 0x18, size 0x4
    s32 bs; // offset 0x1C, size 0x4
    s32 bm; // offset 0x20, size 0x4
    s32 al; // offset 0x24, size 0x4
    s32 as; // offset 0x28, size 0x4
    s32 am; // offset 0x2C, size 0x4
} PixelFormat;

typedef struct {
    // total size: 0x48
    u32 desc;                // offset 0x0, size 0x4
    s32 width;               // offset 0x4, size 0x4
    s32 height;              // offset 0x8, size 0x4
    s32 pitch;               // offset 0xC, size 0x4
    void* ptr;               // offset 0x10, size 0x4
    s32 bitdepth;            // offset 0x14, size 0x4
    PixelFormat pixelformat; // offset 0x18, size 0x30
} plContext;

typedef struct {
    // total size: 0x3C
    u32 flag;           // offset 0x0, size 0x4
    u32 desc;           // offset 0x4, size 0x4
    u32 size;           // offset 0x8, size 0x4
    u32 bitdepth;       // offset 0xC, size 0x4
    s16 width;          // offset 0x1A, size 0x2
    s16 height;         // offset 0x1C, size 0x2
    u32 format;         // offset 0x20, size 0x4
    u32 mem_handle;     // offset 0x24, size 0x4
    uintptr_t lock_ptr; // offset 0x2C, size 0x4
    u32 tex_num;        // offset 0x30, size 0x4
    s8 be_flag;         // offset 0x34, size 0x1
    u8 lock_flag;       // offset 0x37, size 0x1
    void* wkVram;       // offset 0x38, size 0x4
} FLTexture;

#endif
