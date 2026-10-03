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

char* Paths_GetCachePath(const char* file_path) {
    const char* base = Paths_GetPrefPath();
    char* full_path = NULL;

    if (file_path == NULL) {
        SDL_asprintf(&full_path, "%scache/", base);
    } else {
        SDL_asprintf(&full_path, "%scache/%s", base, file_path);
    }

    return full_path;
}

const char* Paths_GetBasePath() {
    return SDL_GetBasePath();
}

SDL_Storage* Paths_OpenUserStorage(SDL_PropertiesID props) {
    return SDL_OpenUserStorage(ORG, APP, props);
}
