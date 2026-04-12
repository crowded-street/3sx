#include "scanline_renderer.h"

#include "port/config/config.h"

static SDL_Renderer* renderer = NULL;
static SDL_Texture* scanline_overlay_texture = NULL;
static int scanline_percentage = 0;
static int cached_scanline_rect_h = -1;
static int cached_scanline_percentage = -1;

static void init_scanlines() {
    const int raw_scanlines = Config_GetInt(CFG_KEY_SCANLINES);

    if (raw_scanlines < 0) {
        return;
    }

    scanline_percentage = SDL_clamp(raw_scanlines, 0, 100);
}

static bool scanline_cache_matches(int rect_h) {
    return (scanline_overlay_texture != NULL) && (cached_scanline_rect_h == rect_h) &&
           (cached_scanline_percentage == scanline_percentage);
}

static bool update_scanline_overlay_texture(const SDL_FRect* game_rect) {
    const int rect_h = game_rect->h;

    if (scanline_cache_matches(rect_h)) {
        return true;
    }

    SDL_DestroyTexture(scanline_overlay_texture);

    scanline_overlay_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, 1, rect_h);

    if (scanline_overlay_texture == NULL) {
        SDL_Log("Couldn't create scanline overlay texture: %s", SDL_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(scanline_overlay_texture, SDL_BLENDMODE_BLEND);

    SDL_Texture* old_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, scanline_overlay_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    const float strength = scanline_percentage / 100.0f;
    const float row_height = game_rect->h / 224.0f;

    // Precompute alpha per screen row to avoid doing expensive calculations for every pixel.
    Uint8* alpha_table = SDL_malloc(rect_h);

    for (int y = 0; y < rect_h; y++) {
        const float t = SDL_fmodf((y + 0.5f) / row_height, 1.0f);
        const float dy = t - 0.5f;
        const float y2 = dy * dy;
        const float y4 = y2 * y2;
        const float scan_weight = SDL_clamp(1.45f - 6.0f * (y2 - 2.05f * y4), 0.0f, 1.45f);
        const float darkness = (1.45f - scan_weight) / 1.45f * strength;
        alpha_table[y] = darkness * 255.0f + 0.5f;
    }

    SDL_FRect* rects = SDL_malloc(rect_h * sizeof(SDL_FRect));

    for (int a = 1; a < 256; a++) {
        int count = 0;

        for (int y = 0; y < rect_h; y++) {
            if (alpha_table[y] == a) {
                rects[count] = (SDL_FRect) { 0, y, 1, 1 };
                count += 1;
            }
        }

        if (count > 0) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, a);
            SDL_RenderFillRects(renderer, rects, count);
        }
    }

    SDL_free(rects);
    SDL_free(alpha_table);

    SDL_SetRenderTarget(renderer, old_target);

    cached_scanline_rect_h = rect_h;
    cached_scanline_percentage = scanline_percentage;

    return true;
}

void ScanlineRenderer_Init(SDL_Renderer* sdl_renderer) {
    renderer = sdl_renderer;
    init_scanlines();
}

void ScanlineRenderer_Destroy() {
    SDL_DestroyTexture(scanline_overlay_texture);
    scanline_overlay_texture = NULL;
    renderer = NULL;
}

void ScanlineRenderer_Render(const SDL_FRect* game_rect) {
    if ((renderer == NULL) || (scanline_percentage == 0)) {
        return;
    }

    if (!update_scanline_overlay_texture(game_rect)) {
        return;
    }

    SDL_RenderTexture(renderer, scanline_overlay_texture, NULL, game_rect);
}
