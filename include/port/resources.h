#ifndef PORT_RESOURCES_H
#define PORT_RESOURCES_H

#include <stdbool.h>

/// @brief Get path to a file in resources folder.
/// @param file_path Relative path to a file in resources, or `NULL` for path to the root of resources folder.
char* Resources_GetPath(const char* file_path);

bool Resources_CheckIfPresent();

/// @brief Run resource copying flow. Repeated calls of this function progress the flow.
/// @return `true` if resources have been copied, `false` otherwise.
bool Resources_RunResourceCopyingFlow();

#endif
