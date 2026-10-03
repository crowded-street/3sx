#ifndef PORT_PATHS_H
#define PORT_PATHS_H

#include <SDL3/SDL.h>

/// Get app directory path
///
/// This value shouldn't be freed after use
const char* Paths_GetPrefPath();

/// Get path to a file in the cache folder, which holds files generated from other resources
/// @param file_path Relative path to a file in the cache, or `NULL` for path to the root of the cache folder.
char* Paths_GetCachePath(const char* file_path);

const char* Paths_GetBasePath();
SDL_Storage* Paths_OpenUserStorage(SDL_PropertiesID props);

#endif
