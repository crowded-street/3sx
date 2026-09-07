#include "core/renderer.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/ui/glyph_renderer.h"

#include <SDL3/SDL.h>
#define GLYPH_COLOR_STRIDE 4

static Uint32 glyph_texture = 0;
static const Uint32 string_kerning = 8;

// hacky way to quickly map the punctuation to sprite atlas positions
static char* punc_map = ".,!:()><";
static GlyphPosition puncPositions[10] = { (GlyphPosition) { 26, 1 }, (GlyphPosition) { 27, 1 },
                                           (GlyphPosition) { 33, 0 }, (GlyphPosition) { 30, 0 },
                                           (GlyphPosition) { 26, 0 }, (GlyphPosition) { 27, 0 },
                                           (GlyphPosition) { 28, 2 }, (GlyphPosition) { 30, 2 } };

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

void GlyphRenderer_DrawChar(char c, GlyphPosition screen_pos, GlyphColor color, float z) {
    if (SDL_isalpha(c)) {
        if (SDL_isupper(c)) {
            const int pos = c - 'A';
            GlyphRenderer_DrawGlyph((GlyphPosition) { pos, 0 }, screen_pos, color, z);
        } else if (SDL_islower(c)) {
            int pos = c - 'a';
            GlyphRenderer_DrawGlyph((GlyphPosition) { pos, 1 }, screen_pos, color, z);
        }
    } else if (SDL_isdigit(c)) {
        GlyphRenderer_DrawDigit((int)(c - '0'), screen_pos, color, z);
    } else if (strchr(punc_map, c)) {
        GlyphPosition puncPos = puncPositions[(int)(strchr(punc_map, c) - punc_map)];
        GlyphRenderer_DrawGlyph(puncPos, screen_pos, color, z);
    } else if (SDL_isspace(c) || c == '_') {
        return;
    } else {
        // draw an X if the character doesn't exist in the spritesheet
        GlyphRenderer_DrawGlyph((GlyphPosition) { 23, 0 }, screen_pos, GLYPH_COLOR_HEAVY, z);
    }
}

void GlyphRenderer_DrawString(GlyphPosition screen_pos, GlyphColor color, float z, const char* format, ...) {
    for (int i = 0; i < strlen(format); i++) {
        GlyphRenderer_DrawChar(format[i], screen_pos, color, z);
        screen_pos.x += string_kerning;
    }
}
