#include "arcade/rom/cps3_dma.h"

static void emit_byte(Uint8 value, Uint8* dst, size_t length, size_t* written, Uint8* previous) {
    if (value & 0x40) {
        size_t run = (value & 0x3f) + 1;

        if (run > length - *written) {
            run = length - *written;
        }

        SDL_memset(dst + *written, *previous, run);
        *written += run;
    } else if (*written < length) {
        dst[(*written)++] = value;
        *previous = value;
    }
}

bool Cps3_DecodeDma(RomRegion graphics, size_t source, size_t dictionary, Uint8* dst, size_t length) {
    size_t written = 0;
    Uint8 previous = 0;

    while (written < length) {
        if (source >= graphics.size) {
            return false;
        }

        Uint8 control = graphics.data[source ^ 1];
        source += 1;

        if (control & 0x80) {
            size_t entry = dictionary + (control & 0x7f) * 2;

            if (entry + 1 >= graphics.size) {
                return false;
            }

            emit_byte(graphics.data[entry ^ 1], dst, length, &written, &previous);
            emit_byte(graphics.data[(entry + 1) ^ 1], dst, length, &written, &previous);
        } else {
            emit_byte(control, dst, length, &written, &previous);
        }
    }

    return true;
}

/// Run state of the 8bpp format: a byte that repeats the previous one is followed by a run length.
typedef struct Dma8Run {
    int last;
    int before_last;
} Dma8Run;

static void emit_byte8(Uint8 value, Uint8* dst, size_t length, size_t* written, Dma8Run* run) {
    if (run->last == run->before_last) {
        size_t count = (value + 1) & 0xFF;

        if (count > length - *written) {
            count = length - *written;
        }

        SDL_memset(dst + *written, run->last, count);
        *written += count;
        run->before_last = -1;
    } else {
        run->before_last = run->last;
        run->last = value;

        if (*written < length) {
            dst[(*written)++] = value;
        }
    }
}

bool Cps3_DecodeDma8(RomRegion graphics, size_t source, size_t dictionary, Uint8* dst, size_t length) {
    size_t written = 0;
    Dma8Run run = { .last = -2, .before_last = -1 };

    while (written < length) {
        if (source >= graphics.size) {
            return false;
        }

        Uint8 control = graphics.data[source ^ 1];
        source += 1;

        // Each bit of the control byte says whether the next byte is a dictionary index or a literal
        for (int i = 0; i < 8 && written < length; i++, control <<= 1) {
            if (source >= graphics.size) {
                return false;
            }

            const Uint8 value = graphics.data[source ^ 1];
            source += 1;

            if (control & 0x80) {
                const size_t entry = dictionary + (value & 0x7f) * 2;

                if (entry + 1 >= graphics.size) {
                    return false;
                }

                emit_byte8(graphics.data[entry ^ 1], dst, length, &written, &run);
                emit_byte8(graphics.data[(entry + 1) ^ 1], dst, length, &written, &run);
            } else {
                emit_byte8(value, dst, length, &written, &run);
            }
        }
    }

    return true;
}
