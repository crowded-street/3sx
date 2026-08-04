#include "port/paths.h"

#include <SDL3/SDL.h>

#define ORG "CrowdedStreet"
#define APP "3SX"

static const char* pref_path = NULL;

const char* Paths_GetPrefPath() {
    if (pref_path == NULL) {
        pref_path = SDL_GetPrefPath(ORG, APP);
    }

    return pref_path;
}

const char* Paths_GetBasePath() {
    return SDL_GetBasePath();
}

SDL_Storage* Paths_OpenUserStorage(SDL_PropertiesID props) {
    return SDL_OpenUserStorage(ORG, APP, props);
}
