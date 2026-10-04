#include "arcade/byte_buffer.h"

bool ByteBuffer_Reserve(ByteBuffer* buffer, size_t additional) {
    if (additional > SIZE_MAX - buffer->size) {
        return false;
    }

    const size_t required = buffer->size + additional;

    if (required <= buffer->capacity) {
        return true;
    }

    size_t capacity = buffer->capacity ? buffer->capacity : 4096;

    while (capacity < required) {
        if (capacity > SIZE_MAX / 2) {
            capacity = required;
            break;
        }

        capacity *= 2;
    }

    void* resized = SDL_realloc(buffer->data, capacity);

    if (resized == NULL) {
        return false;
    }

    buffer->data = resized;
    buffer->capacity = capacity;
    return true;
}

bool ByteBuffer_Append(ByteBuffer* buffer, const void* bytes, size_t size) {
    if (!ByteBuffer_Reserve(buffer, size)) {
        return false;
    }

    SDL_memcpy(buffer->data + buffer->size, bytes, size);
    buffer->size += size;
    return true;
}
