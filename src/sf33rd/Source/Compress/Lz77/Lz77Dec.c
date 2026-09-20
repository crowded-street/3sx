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

static u8* copy_from_dictionary_stepped(u8* dst, const u8* dic, u8 step, s32 loop) {
    s32 j;

    for (j = 0; j < loop; j++) {
        *dst++ = *dic + step;
        dic++;
    }

    return dst;
}

/* A straight literal run: loop bytes copied out of the stream. The two literal
 * opcodes wrote this loop out twice; it advances src the way fill_bytes and
 * fill_ramp return dst. */
static u8* lz77_copy_literals(u8** srcp, u8* dst, s32 loop) {
    u8* src = *srcp;
    s32 j;

    for (j = 0; j < loop; j++) {
        *dst++ = *src++;
    }

    *srcp = src;

    return dst;
}

/* The literal and fill opcodes: the six ways a run is written out rather than
 * copied from the dictionary. Returns how many output bytes the opcode
 * produced, which is what the caller takes off the remaining size. An opcode
 * this switch does not name produces none and leaves size alone, exactly as
 * before. */
static s32 lz77_write_run(u8** srcp, u8** dstp, u16 offset) {
    u8* src = *srcp;
    u8* dst = *dstp;
    s32 loop = 0;
    u8 num;
    u8 step;

    switch (offset & 0x3F) {
    case 1:
        loop = *src++;

        loop = whole_loop_if_zero(loop, 0x100);

        dst = lz77_copy_literals(&src, dst, loop);

        break;

    case 2:
        loop = (src[0] << 8) | src[1];
        src += 2;

        loop = whole_loop_if_zero(loop, 0x10000);

        dst = lz77_copy_literals(&src, dst, loop);

        break;

    case 3:
        num = *src++;
        loop = *src++;

        loop = whole_loop_if_zero(loop, 0x100);

        dst = fill_bytes(dst, num, loop);

        break;

    case 4:
        num = *src++;
        loop = (src[0] << 8) | src[1];
        src += 2;

        loop = whole_loop_if_zero(loop, 0x10000);

        dst = fill_bytes(dst, num, loop);

        break;

    case 5:
        num = *src++;
        step = *src++;
        loop = *src++;

        loop = whole_loop_if_zero(loop, 0x100);

        dst = fill_ramp(dst, num, step, loop);

        break;

    case 6:
        num = *src++;
        step = *src++;
        loop = (src[0] << 8) | src[1];
        src += 2;

        loop = whole_loop_if_zero(loop, 0x10000);

        dst = fill_ramp(dst, num, step, loop);

        break;
    }

    *srcp = src;
    *dstp = dst;

    return loop;
}

/* The long dictionary opcode: a fourteen-bit back-reference, optionally stepped.
 * Returns how many output bytes it produced, on the same terms as
 * lz77_write_run. */
static s32 lz77_copy_long_run(u8** srcp, u8** dstp, u16 offset) {
    u8* src = *srcp;
    u8* dst = *dstp;
    u8* dic;
    s32 loop;
    u8 step;

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
        dst = copy_from_dictionary_stepped(dst, dic, step, loop);
    } else {
        dst = copy_from_dictionary(dst, dic, loop);
    }

    *srcp = src;
    *dstp = dst;

    return loop;
}

/* The short dictionary opcode: an eleven-bit back-reference with a four-bit
 * length, both stored one less than they mean. Returns how many output bytes it
 * produced, on the same terms as the other two. */
static s32 lz77_copy_short_run(u8** srcp, u8** dstp, u16 offset) {
    u8* src = *srcp;
    u8* dst = *dstp;
    u8* dic;
    s32 loop;

    offset = (offset << 8) | *src++;
    loop = offset & 0xF;

    loop = whole_loop_if_zero(loop, 0x10);

    offset = (offset >> 4) & 0x7FF;

    if (offset == 0) {
        offset = 0x800;
    }

    dic = dst - offset;

    dst = copy_from_dictionary(dst, dic, loop);

    *srcp = src;
    *dstp = dst;

    return loop;
}

s32 decLZ77withSizeCheck(u8* src, u8* dst, s32 size) {
    u16 offset;

    while (size > 0) {
        offset = *src++;

        if (offset & 0x80) {
            if (offset & 0x40) {
                size -= lz77_copy_long_run(&src, &dst, offset);
            } else {
                size -= lz77_write_run(&src, &dst, offset);
            }
        } else {
            size -= lz77_copy_short_run(&src, &dst, offset);
        }
    }

    return size == 0;
}
