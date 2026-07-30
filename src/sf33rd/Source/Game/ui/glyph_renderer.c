#include "sf33rd/Source/Game/ui/glyph_renderer.h"
#include "core/renderer.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <SDL3/SDL.h>

#define GLYPH_COLOR_STRIDE 4

static Uint32 glyph_texture = 0;

static FLTexture* get_texture() {
    return &flTexture[glyph_texture - 1];
}

bool GlyphRenderer_Init() {
    const char* base_path = SDL_GetBasePath();
    char* full_path = NULL;
    SDL_asprintf(&full_path, "%s/assets/glyphs.bmp", base_path);
    glyph_texture = flCreateTextureFromFile(full_path, 0);
    SDL_free(full_path);
    return glyph_texture != 0;
}

void GlyphRenderer_DrawGlyph(GlyphPosition atlas_pos, GlyphPosition screen_pos, GlyphColor color, float z) {
    atlas_pos.y += GLYPH_COLOR_STRIDE * color;

    FLTexture* texture = get_texture();
    Sprite sprite = { 0 };

    sprite.v[0].z = sprite.v[3].z = z;
    sprite.v[0].x = screen_pos.x;
    sprite.v[0].y = screen_pos.y;
    sprite.v[3].x = screen_pos.x + 8;
    sprite.v[3].y = screen_pos.y + 8;

    sprite.t[0].s = atlas_pos.x * 8 / (float)texture->width;
    sprite.t[0].t = atlas_pos.y * 8 / (float)texture->height;
    sprite.t[3].s = (atlas_pos.x + 1) * 8 / (float)texture->width;
    sprite.t[3].t = (atlas_pos.y + 1) * 8 / (float)texture->height;

    sprite.tex_code = glyph_texture;
    Renderer_SetTexture(glyph_texture);
    Renderer_DrawSprite(&sprite, 0xFFFFFFFF);
}

void GlyphRenderer_DrawDigit(Uint8 digit, GlyphPosition screen_pos, GlyphColor color, float z) {
    if (digit > 9) {
        SDL_assert(false);
        return;
    }

    GlyphRenderer_DrawGlyph((GlyphPosition) { digit, 2 }, screen_pos, color, z);
}
