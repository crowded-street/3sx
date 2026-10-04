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
