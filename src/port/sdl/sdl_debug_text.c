#include "port/sdl/sdl_debug_text.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

#include <SDL3/SDL.h>

#if DEBUG

static SDL_Renderer* debug_renderer = NULL;

void SDLDebugText_Initialize(SDL_Renderer* renderer) {
    debug_renderer = renderer;
}

void SDLDebugText_Render() {
    if (debug_renderer == NULL) {
        return;
    }

    // Get the debug string buffer
    RenderBuffer* buff_ptr = (RenderBuffer*)flPS2GetSystemBuffAdrs(flDebugStrHan);
    if (buff_ptr == NULL || flDebugStrCtr == 0) {
        return;
    }

    // Calculate scale factor based on current window height
    int render_width, render_height;
    SDL_GetRenderOutputSize(debug_renderer, &render_width, &render_height);
    float scale = (float)render_height / 480.0f;

    // Set blend mode for text
    SDL_SetRenderDrawBlendMode(debug_renderer, SDL_BLENDMODE_BLEND);

    // Apply render scale to scale the font
    SDL_SetRenderScale(debug_renderer, scale, scale);

    // Render all characters
    for (u32 i = 0; i < flDebugStrCtr; i++) {
        RenderBuffer* ch = &buff_ptr[i];
        if (ch->code < 0x20 || ch->code > 0x7F) {
            continue;
        }

        // Extract color components (ARGB format)
        u8 a = (ch->col >> 24) & 0xFF;
        u8 r = (ch->col >> 16) & 0xFF;
        u8 g = (ch->col >> 8) & 0xFF;
        u8 b = ch->col & 0xFF;

        // The PS2 flPrintColor function halved all color values before storing them.
        // We need to double them back to restore the original brightness.
        // (see flPrintColor in flps2debug.c)
        r = (r < 128) ? r * 2 : 255;
        g = (g < 128) ? g * 2 : 255;
        b = (b < 128) ? b * 2 : 255;
        a = (a < 128) ? a * 2 : 255;

        // Create a single character string
        char text[2] = { (char)ch->code, '\0' };

        // Draw black outline in 8 directions
        SDL_SetRenderDrawColor(debug_renderer, 0, 0, 0, 255);
        SDL_RenderDebugText(debug_renderer, ch->x - 1, ch->y - 1, text); // top-left
        SDL_RenderDebugText(debug_renderer, ch->x, ch->y - 1, text);     // top
        SDL_RenderDebugText(debug_renderer, ch->x + 1, ch->y - 1, text); // top-right
        SDL_RenderDebugText(debug_renderer, ch->x - 1, ch->y, text);     // left
        SDL_RenderDebugText(debug_renderer, ch->x + 1, ch->y, text);     // right
        SDL_RenderDebugText(debug_renderer, ch->x - 1, ch->y + 1, text); // bottom-left
        SDL_RenderDebugText(debug_renderer, ch->x, ch->y + 1, text);     // bottom
        SDL_RenderDebugText(debug_renderer, ch->x + 1, ch->y + 1, text); // bottom-right

        // Draw colored character on top
        SDL_SetRenderDrawColor(debug_renderer, r, g, b, a);
        SDL_RenderDebugText(debug_renderer, ch->x, ch->y, text);
    }

    // Reset render scale
    SDL_SetRenderScale(debug_renderer, 1.0f, 1.0f);

    // Reset draw color
    SDL_SetRenderDrawColor(debug_renderer, 255, 255, 255, 255);

    // Clear debug text buffer for next frame
    flDebugStrCtr = 0;
}

void SDLDebugText_Destroy() {
    debug_renderer = NULL;
}

#endif // DEBUG
