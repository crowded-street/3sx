#include "sf33rd/Source/Game/ui/input_history.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/ui/glyph_renderer.h"

#include <SDL3/SDL.h>

#define INPUT_HISTORY_MAX 17
#define INPUT_COUNT_MAX 99

#define INPUT_HISTORY_OFFSET_TOP 47
#define INPUT_HISTORY_HORIZONTAL_PADDING 2
#define INPUT_HISTORY_RESERVED_WIDTH 51

typedef struct InputHistoryItem {
    u16 lvbt;
    u8 count;
} InputHistoryItem;

typedef struct InputHistory {
    InputHistoryItem buffer[INPUT_HISTORY_MAX];
    u8 end;
    u8 length;
} InputHistory;

typedef struct AttackBitDescription {
    u8 bit;
    u8 strength;
    GlyphPosition glyph;
} AttackBitDescription;

static const GlyphPosition lever_to_glyph[] = {
    [0b0001] = GLYPH_UP,        [0b0010] = GLYPH_DOWN,  [0b0100] = GLYPH_LEFT,     [0b0101] = GLYPH_UP_LEFT,
    [0b0110] = GLYPH_DOWN_LEFT, [0b1000] = GLYPH_RIGHT, [0b1001] = GLYPH_UP_RIGHT, [0b1010] = GLYPH_DOWN_RIGHT,
};

static const AttackBitDescription attack_bits[] = {
    { .bit = 4, .strength = 0, .glyph = GLYPH_PUNCH }, { .bit = 5, .strength = 1, .glyph = GLYPH_PUNCH },
    { .bit = 6, .strength = 2, .glyph = GLYPH_PUNCH }, { .bit = 8, .strength = 0, .glyph = GLYPH_KICK },
    { .bit = 9, .strength = 1, .glyph = GLYPH_KICK },  { .bit = 10, .strength = 2, .glyph = GLYPH_KICK },
};

static const u32 attack_strength_to_color[] = {
    [0] = 0xFF0000FF, // Blue
    [1] = 0xFFFFFF00, // Yellow
    [2] = 0xFFFF0000, // Red
};

static InputHistory input_history[2] = { 0 };

static InputHistoryItem* last_item(InputHistory* history) {
    SDL_assert(history->length > 0);
    return &history->buffer[(history->end + INPUT_HISTORY_MAX - 1) % INPUT_HISTORY_MAX];
}

static u16 cleanup_lvbt(u16 lvbt) {
    if ((lvbt & 0b0001) && (lvbt & 0b0010)) { // Up + Down
        lvbt &= ~0b0011;
    }

    if ((lvbt & 0b0100) && (lvbt & 0b1000)) { // Left + Right
        lvbt &= ~0b1100;
    }

    return lvbt;
}

void InputHistory_Append(u16 lvbt, u8 player) {
    InputHistory* history = &input_history[player];

    if ((history->length > 0) && (last_item(history)->lvbt == lvbt)) {
        last_item(history)->count = SDL_min(last_item(history)->count + 1, INPUT_COUNT_MAX);
    } else if ((history->length == 0) && (lvbt == 0)) {
        // Do nothing
    } else {
        history->end = (history->end + 1) % INPUT_HISTORY_MAX;
        history->length = SDL_min(history->length + 1, INPUT_HISTORY_MAX);
        last_item(history)->lvbt = cleanup_lvbt(lvbt);
        last_item(history)->count = 1;
    }
}

void InputHistory_Render() {
    GlyphPosition pos;
    const float z = PrioBase[2];

    for (int i = 0; i < 2; i++) {
        pos.y = INPUT_HISTORY_OFFSET_TOP;
        InputHistory* history = &input_history[i];

        for (int j = 0; j < INPUT_HISTORY_MAX; j++) {
            if (i == 0) {
                pos.x = INPUT_HISTORY_HORIZONTAL_PADDING;
            } else {
                pos.x = 384 - INPUT_HISTORY_RESERVED_WIDTH - INPUT_HISTORY_HORIZONTAL_PADDING;
            }

            const u8 item_index = (history->end + INPUT_HISTORY_MAX - j - 1) % INPUT_HISTORY_MAX;
            const InputHistoryItem* item = &history->buffer[item_index];

            if (item->count == 0) {
                break;
            }

            if (item->count == 1) {
                pos.x += 16;
            } else if (item->count < 10) {
                pos.x += 8;
                GlyphRenderer_DrawDigit(item->count, pos, 0xFFFFFFFF, z);
                pos.x += 8;
            } else {
                GlyphRenderer_DrawDigit(item->count / 10, pos, 0xFFFFFFFF, z);
                pos.x += 8;
                GlyphRenderer_DrawDigit(item->count % 10, pos, 0xFFFFFFFF, z);
                pos.x += 8;
            }

            pos.x += 1;

            const u16 lever = item->lvbt & 0xF;

            if (lever > 0) {
                GlyphRenderer_DrawGlyph(lever_to_glyph[lever], pos, 0xFFFFFFFF, z);
            }

            pos.x += 9;

            for (int k = 0; k < 6; k++) {
                const AttackBitDescription* attack_bit = &attack_bits[k];

                if (!(item->lvbt & (1 << attack_bit->bit))) {
                    continue;
                }

                GlyphRenderer_DrawGlyph(attack_bit->glyph, pos, attack_strength_to_color[attack_bit->strength], z);
                pos.x += 9;
            }

            pos.y += 9;
        }
    }
}

void InputHistory_Clear() {
    SDL_zeroa(input_history);
}
