#include "port/sdl/sdl_message_renderer.h"

#include <SDL3/SDL.h>

SDL_Texture* message_canvas = NULL;

static const int canvas_width = 512;
static const int canvas_height = 448;

static SDL_Renderer* _renderer = NULL;
static SDL_Texture* knjsub_texture = NULL;
static SDL_Palette* knjsub_palette = NULL;

static const SDL_Color knjsub_palette_colors[4] = {
    { .r = 255, .g = 255, .b = 255, .a = 0 },
    { .r = 255, .g = 255, .b = 255, .a = 0 },
    { .r = 255, .g = 255, .b = 255, .a = 0 },
    { .r = 255, .g = 255, .b = 255, .a = 255 },
};

void SDLMessageRenderer_Initialize(SDL_Renderer* renderer) {
    _renderer = renderer;

    // Initialize canvas
    message_canvas =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, canvas_width, canvas_height);
    SDL_SetTextureScaleMode(message_canvas, SDL_SCALEMODE_NEAREST);

    // Initialize knjsub palette
    knjsub_palette = SDL_CreatePalette(4);
    SDL_SetPaletteColors(knjsub_palette, knjsub_palette_colors, 0, 4);
}

void SDLMessageRenderer_BeginFrame() {
    // Clear canvas
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_SetRenderTarget(_renderer, message_canvas);
    SDL_RenderClear(_renderer);
}

void SDLMessageRenderer_CreateTexture(int width, int height, void* pixels, int format) {
    if (knjsub_texture != NULL) {
        SDL_DestroyTexture(knjsub_texture);
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_INDEX4LSB, pixels, width / 2);
    SDL_SetSurfacePalette(surface, knjsub_palette);
    knjsub_texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_DestroySurface(surface);

    SDL_SetTextureScaleMode(knjsub_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(knjsub_texture, SDL_BLENDMODE_BLEND);
}

static int adjust_coordinate(int coordinate, bool is_x, bool is_uv) {
    coordinate >>= 4;

    if (!is_uv) {
        const int display_size = is_x ? canvas_width : canvas_height;
        coordinate -= (4096 - display_size) / 2;
    }

    return coordinate;
}

static Uint8 scale_color_value(Uint8 value) {
    int temp = value;
    temp *= 2;

    if (temp > 0xFF) {
        temp = 0xFF;
    }

    return (Uint8)temp;
}

void SDLMessageRenderer_DrawTexture(int x0, int y0, int x1, int y1, int u0, int v0, int u1, int v1,
                                    unsigned int color) {
    x0 = adjust_coordinate(x0, true, false);
    y0 = adjust_coordinate(y0, false, false);
    x1 = adjust_coordinate(x1, true, false);
    y1 = adjust_coordinate(y1, false, false);

    u0 = adjust_coordinate(u0, true, true);
    v0 = adjust_coordinate(v0, false, true);
    u1 = adjust_coordinate(u1, true, true);
    v1 = adjust_coordinate(v1, false, true);

    SDL_FRect src_rect;
    src_rect.x = u0;
    src_rect.y = v0;
    src_rect.w = u1 - u0;
    src_rect.h = v1 - v0;

    SDL_FRect dst_rect;
    dst_rect.x = x0;
    dst_rect.y = y0;
    dst_rect.w = x1 - x0;
    dst_rect.h = y1 - y0;

    const Uint8 r = scale_color_value(color & 0xFF);
    const Uint8 g = scale_color_value((color >> 8) & 0xFF);
    const Uint8 b = scale_color_value((color >> 16) & 0xFF);
    const Uint8 a = scale_color_value(color >> 24);

    SDL_SetTextureColorMod(knjsub_texture, r, g, b);
    SDL_SetTextureAlphaMod(knjsub_texture, a);

    SDL_SetRenderTarget(_renderer, message_canvas);
    SDL_RenderTexture(_renderer, knjsub_texture, &src_rect, &dst_rect);
}
