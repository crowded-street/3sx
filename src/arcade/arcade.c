#include "arcade/arcade.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_char_data.h"
#include "arcade/rom/rom.h"
#include "port/resources.h"

#if ARCADE_ROM_TEXTURES
#include "arcade/arcade_texture.h"
#endif

#include <SDL3/SDL.h>

#include <stdbool.h>

static Rom* roms[ROM_GAME_COUNT] = { 0 };

static Rom* load_rom(RomGame game) {
    char* path = Resources_GetPath(Rom_GetZipName(game));
    Rom* rom = Rom_Create(game, path);

    if (rom == NULL) {
        SDL_Log("Couldn't load arcade ROM %s: %s", Rom_GetZipName(game), SDL_GetError());
    }

    SDL_free(path);
    return rom;
}

void Arcade_Init() {
    ArcadeBalance_Init();

    bool needed[ROM_GAME_COUNT] = { 0 };
    needed[ROM_GAME_SFIII3] = ArcadeBalance_IsEnabled();

    for (int game = 0; game < ROM_GAME_COUNT; game++) {
        if (needed[game]) {
            roms[game] = load_rom(game);
        }
    }

    if (roms[ROM_GAME_SFIII3] != NULL) {
        ArcadeCharData_Init(roms[ROM_GAME_SFIII3]);
    }
}

void Arcade_Finish() {
#if ARCADE_ROM_TEXTURES
    ArcadeTexture_Finish();
#endif

    for (int game = 0; game < ROM_GAME_COUNT; game++) {
        Rom_Destroy(roms[game]);
        roms[game] = NULL;
    }
}
