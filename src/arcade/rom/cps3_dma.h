#ifndef CPS3_DMA_H
#define CPS3_DMA_H

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stddef.h>

/// Decompresses CPS3 character DMA data.
/// @param graphics Graphics block in the layout of `Rom_GetGraphics`.
/// @param source Offset of the compressed data within `graphics`.
/// @param dictionary Offset of the 128-entry byte pair dictionary within `graphics`.
/// @return `false` if the data runs past the end of `graphics`.
bool Cps3_DecodeDma(
    const Uint8* graphics, size_t graphics_size, size_t source, size_t dictionary, Uint8* dst, size_t length
);

#endif
