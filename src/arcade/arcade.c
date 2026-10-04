#include "arcade/arcade.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_char_data.h"
#include "arcade/arcade_stage.h"
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

static bool rom_exists(RomGame game) {
    char* path = Resources_GetPath(Rom_GetZipName(game));
    const bool exists = SDL_GetPathInfo(path, NULL);
    SDL_free(path);
    return exists;
}

void Arcade_Init() {
    ArcadeBalance_Init();

    bool needed[ROM_GAME_COUNT] = { false };
    needed[ROM_GAME_SFIII] = rom_exists(ROM_GAME_SFIII);
    needed[ROM_GAME_SFIII3] = true;

    for (int game = 0; game < ROM_GAME_COUNT; game++) {
        if (needed[game]) {
            roms[game] = load_rom(game);
        }
    }

    const Rom* sfiii3 = roms[ROM_GAME_SFIII3];

    if (sfiii3 != NULL && ArcadeBalance_IsEnabled()) {
        ArcadeCharData_Init(sfiii3);
    }

    ArcadeStage_Init((const Rom* const*)roms);

    // Only the stage converter reads New Generation's ROM
    Rom_Destroy(roms[ROM_GAME_SFIII]);
    roms[ROM_GAME_SFIII] = NULL;

#if ARCADE_ROM_TEXTURES
    if (sfiii3 != NULL) {
        ArcadeTexture_Init(sfiii3);
    }
#endif
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
