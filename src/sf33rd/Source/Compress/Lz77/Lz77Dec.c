#include "sf33rd/Source/Compress/Lz77/Lz77Dec.h"
#include "common.h"

/* A loop count of zero means the whole range its field can address. */
static s32 whole_loop_if_zero(s32 loop, s32 whole) {
    if (loop == 0) {
        loop = whole;
    }

    return loop;
}

/* The three runs this decoder writes: one byte repeated, one byte stepping, and
 * a copy from earlier in the output. Each returns the write pointer where it
 * stopped, which is the one value it produced. */
static u8* fill_bytes(u8* dst, u8 num, s32 loop) {
    s32 j;

    for (j = 0; j < loop; j++) {
        *dst++ = num;
    }

    return dst;
}

static u8* fill_ramp(u8* dst, u8 num, u8 step, s32 loop) {
    s32 j;

    for (j = 0; j < loop; j++) {
        *dst++ = num;
        num += step;
    }

    return dst;
}

static u8* copy_from_dictionary(u8* dst, const u8* dic, s32 loop) {
    s32 j;

    for (j = 0; j < loop; j++) {
        *dst++ = *dic++;
    }

    return dst;
}

s32 decLZ77withSizeCheck(u8* src, u8* dst, s32 size) {
    s32 j;
    s32 loop;
    u8* dic;
    u8 num;
    u8 step;
    u16 offset;

    while (size > 0) {
        offset = *src++;

        if (offset & 0x80) {
            if (offset & 0x40) {
                offset = ((offset << 8) | *src++) & 0x3FFF;

                if (offset == 0) {
                    offset = 0x4000;
                }

                loop = *src++;

                if (loop & 0x80) {
                    step = *src++;
                } else {
                    step = 0;
                }

                loop &= 0x7F;

                loop = whole_loop_if_zero(loop, 0x80);

                dic = dst - offset;

                if (step) {
                    for (j = 0; j < loop; j++) {
                        *dst++ = *dic + step;
                        dic++;
                    }
                } else {
                    dst = copy_from_dictionary(dst, dic, loop);
                }

                size -= loop;
            } else {
                switch (offset & 0x3F) {
                case 1:
                    loop = *src++;

                    loop = whole_loop_if_zero(loop, 0x100);

                    for (j = 0; j < loop; j++) {
                        *dst++ = *src++;
                    }

                    size -= loop;
                    break;

                case 2:
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    loop = whole_loop_if_zero(loop, 0x10000);

                    for (j = 0; j < loop; j++) {
                        *dst++ = *src++;
                    }

                    size -= loop;
                    break;

                case 3:
                    num = *src++;
                    loop = *src++;

                    loop = whole_loop_if_zero(loop, 0x100);

                    dst = fill_bytes(dst, num, loop);

                    size -= loop;
                    break;

                case 4:
                    num = *src++;
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    loop = whole_loop_if_zero(loop, 0x10000);

                    dst = fill_bytes(dst, num, loop);

                    size -= loop;
                    break;

                case 5:
                    num = *src++;
                    step = *src++;
                    loop = *src++;

                    loop = whole_loop_if_zero(loop, 0x100);

                    dst = fill_ramp(dst, num, step, loop);

                    size -= loop;
                    break;

                case 6:
                    num = *src++;
                    step = *src++;
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    loop = whole_loop_if_zero(loop, 0x10000);

                    dst = fill_ramp(dst, num, step, loop);

                    size -= loop;
                    break;
                }
            }
        } else {
            offset = (offset << 8) | *src++;
            loop = offset & 0xF;

            loop = whole_loop_if_zero(loop, 0x10);

            offset = (offset >> 4) & 0x7FF;

            if (offset == 0) {
                offset = 0x800;
            }

            dic = dst - offset;

            dst = copy_from_dictionary(dst, dic, loop);

            size -= loop;
        }
    }

    return size == 0;
}
