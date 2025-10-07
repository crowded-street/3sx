#include "sf33rd/Source/Compress/Lz77/Lz77Dec.h"
#include "common.h"

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

                if (loop == 0) {
                    loop = 0x80;
                }

                dic = dst - offset;

                if (step) {
                    for (j = 0; j < loop; j++) {
                        *dst++ = *dic + step;
                        dic++;
                    }
                } else {
                    for (j = 0; j < loop; j++) {
                        *dst++ = *dic++;
                    }
                }

                size -= loop;
            } else {
                switch (offset & 0x3F) {
                case 1:
                    loop = *src++;

                    if (loop == 0) {
                        loop = 0x100;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = *src++;
                    }

                    size -= loop;
                    break;

                case 2:
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    if (loop == 0) {
                        loop = 0x10000;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = *src++;
                    }

                    size -= loop;
                    break;

                case 3:
                    num = *src++;
                    loop = *src++;

                    if (loop == 0) {
                        loop = 0x100;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = num;
                    }

                    size -= loop;
                    break;

                case 4:
                    num = *src++;
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    if (loop == 0) {
                        loop = 0x10000;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = num;
                    }

                    size -= loop;
                    break;

                case 5:
                    num = *src++;
                    step = *src++;
                    loop = *src++;

                    if (loop == 0) {
                        loop = 0x100;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = num;
                        num += step;
                    }

                    size -= loop;
                    break;

                case 6:
                    num = *src++;
                    step = *src++;
                    loop = (src[0] << 8) | src[1];
                    src += 2;

                    if (loop == 0) {
                        loop = 0x10000;
                    }

                    for (j = 0; j < loop; j++) {
                        *dst++ = num;
                        num += step;
                    }

                    size -= loop;
                    break;
                }
            }
        } else {
            offset = (offset << 8) | *src++;
            loop = offset & 0xF;

            if (loop == 0) {
                loop = 0x10;
            }

            offset = (offset >> 4) & 0x7FF;

            if (offset == 0) {
                offset = 0x800;
            }

            dic = dst - offset;

            for (j = 0; j < loop; j++) {
                *dst++ = *dic++;
            }

            size -= loop;
        }
    }

    return size == 0;
}
