#ifndef FLPS2DEBUG_H
#define FLPS2DEBUG_H

#include "structs.h"
#include "types.h"

/// @brief Initialize debug string buffer
///
/// Allocates and initializes the buffer used by flPrintL for debug text rendering.
void flPS2DebugInit();
void flPS2SystemError(s32 error_level, const char* format, ...);

#endif // FLPS2DEBUG_H
