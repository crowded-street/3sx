#ifndef PORT_PATHS_H
#define PORT_PATHS_H

#include <SDL3/SDL.h>

/// Get app directory path
///
/// This value shouldn't be freed after use
const char* Paths_GetPrefPath();

const char* Paths_GetBasePath();
SDL_Storage* Paths_OpenUserStorage(SDL_PropertiesID props);

#endif
