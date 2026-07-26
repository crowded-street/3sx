#include "core/xbox_buttons.h"
#include "core/render_primitives.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"

#include <SDL3/SDL.h>

#define XBOX_TEXTURE_WIDTH 64.0f
#define XBOX_TEXTURE_HEIGHT 128.0f

static const int xbox_atlas[][2] = {
    [BUTTON_ICON_WEST] = { 2, 1 },           [BUTTON_ICON_NORTH] = { 3, 1 },
    [BUTTON_ICON_RIGHT_SHOULDER] = { 1, 3 }, [BUTTON_ICON_LEFT_SHOULDER] = { 0, 3 },
    [BUTTON_ICON_SOUTH] = { 0, 1 },          [BUTTON_ICON_EAST] = { 1, 1 },
    [BUTTON_ICON_RIGHT_TRIGGER] = { 1, 4 },  [BUTTON_ICON_LEFT_TRIGGER] = { 0, 4 },
};

static u32 xbox_texture = 0;

bool XboxButtons_Init() {
    const char* base_path = SDL_GetBasePath();
    char* full_path = NULL;
    SDL_asprintf(&full_path, "%s/assets/xbox_buttons.bmp", base_path);
    xbox_texture = flCreateTextureFromFile(full_path, 0);
    SDL_free(full_path);
    return xbox_texture != 0;
}

void XboxButtons_SetTextureParams(Sprite* sprite, ButtonIcon icon) {
    sprite->tex_code = xbox_texture;

    const int atlas_min_x = xbox_atlas[icon][0] * 16;
    const int atlas_max_x = atlas_min_x + 16;
    const int atlas_min_y = xbox_atlas[icon][1] * 16;
    const int atlas_max_y = atlas_min_y + 16;
    sprite->t[0].s = atlas_min_x / XBOX_TEXTURE_WIDTH;
    sprite->t[3].s = atlas_max_x / XBOX_TEXTURE_WIDTH;
    sprite->t[0].t = atlas_min_y / XBOX_TEXTURE_HEIGHT;
    sprite->t[3].t = atlas_max_y / XBOX_TEXTURE_HEIGHT;
}
