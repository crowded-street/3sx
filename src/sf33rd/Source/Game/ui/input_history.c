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
    GlyphPosition glyph;
    GlyphColor color;
} AttackBitDescription;

static const GlyphPosition lever_to_glyph[] = {
    [0b0000] = GLYPH_NEUTRAL, [0b0001] = GLYPH_UP,       [0b0010] = GLYPH_DOWN,
    [0b0100] = GLYPH_LEFT,    [0b0101] = GLYPH_UP_LEFT,  [0b0110] = GLYPH_DOWN_LEFT,
    [0b1000] = GLYPH_RIGHT,   [0b1001] = GLYPH_UP_RIGHT, [0b1010] = GLYPH_DOWN_RIGHT,
};

static const AttackBitDescription attack_bits[] = {
    { .bit = 4, .glyph = GLYPH_PUNCH, .color = GLYPH_COLOR_LIGHT },
    { .bit = 5, .glyph = GLYPH_PUNCH, .color = GLYPH_COLOR_MEDIUM },
    { .bit = 6, .glyph = GLYPH_PUNCH, .color = GLYPH_COLOR_HEAVY },
    { .bit = 8, .glyph = GLYPH_KICK, .color = GLYPH_COLOR_LIGHT },
    { .bit = 9, .glyph = GLYPH_KICK, .color = GLYPH_COLOR_MEDIUM },
    { .bit = 10, .glyph = GLYPH_KICK, .color = GLYPH_COLOR_HEAVY },
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

static void draw_attack_glyphs(const InputHistoryItem* item) {
    for (int k = 0; k < SDL_arraysize(attack_bits); k++) {
        const AttackBitDescription* attack_bit = &attack_bits[k];

        if (!(item->lvbt & (1 << attack_bit->bit))) {
            continue;
        }

        glyph_renderer_state.color = attack_bit->color;
        GlyphRenderer_DrawGlyph(attack_bit->glyph);
        glyph_renderer_state.position.x += 1;
    }
}

void InputHistory_Render() {
    glyph_renderer_state.z = PrioBase[2];

    for (int i = 0; i < 2; i++) {
        glyph_renderer_state.position.y = INPUT_HISTORY_OFFSET_TOP;
        InputHistory* history = &input_history[i];

        for (int j = 0; j < INPUT_HISTORY_MAX; j++) {
            glyph_renderer_state.color = GLYPH_COLOR_WHITE;

            if (i == 0) {
                glyph_renderer_state.position.x = INPUT_HISTORY_HORIZONTAL_PADDING;
            } else {
                glyph_renderer_state.position.x = 384 - INPUT_HISTORY_RESERVED_WIDTH - INPUT_HISTORY_HORIZONTAL_PADDING;
            }

            const u8 item_index = (history->end + INPUT_HISTORY_MAX - j - 1) % INPUT_HISTORY_MAX;
            const InputHistoryItem* item = &history->buffer[item_index];

            if (item->count == 0) {
                break;
            }

            // Digits

            if (item->count == 1) {
                GlyphRenderer_DrawString("  ");
            } else {
                GlyphRenderer_DrawString("%2d", item->count);
            }

            glyph_renderer_state.position.x += 1;

            // Lever

            const u16 lever = item->lvbt & 0xF;
            GlyphRenderer_DrawGlyph(lever_to_glyph[lever]);
            glyph_renderer_state.position.x += 1;

            // Attacks

            draw_attack_glyphs(item);

            glyph_renderer_state.position.y += GLYPH_SIZE + 1;
        }
    }
}

void InputHistory_Clear() {
    SDL_zeroa(input_history);
}
