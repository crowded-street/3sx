#include "port/paths.h"

#include <SDL3/SDL.h>

static const char* pref_path = NULL;

const char* Paths_GetPrefPath() {
    if (pref_path == NULL) {
        pref_path = SDL_GetPrefPath("CrowdedStreet", "3SX");
    }

    return pref_path;
}

const char* Paths_GetBasePath() {
    return SDL_GetBasePath();
}
