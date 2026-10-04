#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stddef.h>

/// Growable byte array. Zero-initialize it before use and free `data` with `SDL_free`.
typedef struct ByteBuffer {
    Uint8* data;
    size_t size;
    size_t capacity;
} ByteBuffer;

/// Makes room for `additional` more bytes without changing the size.
bool ByteBuffer_Reserve(ByteBuffer* buffer, size_t additional);

bool ByteBuffer_Append(ByteBuffer* buffer, const void* bytes, size_t size);

#endif
