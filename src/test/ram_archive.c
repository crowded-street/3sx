#if STATCHECK

#include "test/ram_archive.h"

#include <SDL3/SDL.h>

#define RAM_ARCHIVE_MAGIC "SCRD"
#define RAM_FRAME_SIZE 524288

static void decompress_frame(Uint8* dst, const Uint8* src, Uint32 frame_size) {
    const Uint8* s = src;
    Uint8* d = dst;

    while (s < src + frame_size && d < dst + RAM_FRAME_SIZE) {
        const Uint8 control = *s++;

        if (control < 0x80) {
            const Uint8 length = control + 1;

            for (int i = 0; i < length; i++) {
                *d++ = *s++;
            }
        } else {
            const Uint8 length = (control & 0x7F) + 1;

            for (int i = 0; i < length; i++) {
                *d++ = 0;
            }
        }
    }
}

static void read_frame(RamArchive* archive, Uint16 index, Uint8* dst) {
    const RamArchive_FrameTableEntry* entry = &archive->entries[index];
    Uint8* compressed_frame = SDL_malloc(entry->size);
    SDL_SeekIO(archive->io, entry->offset, SDL_IO_SEEK_SET);
    SDL_ReadIO(archive->io, compressed_frame, entry->size);
    decompress_frame(dst, compressed_frame, entry->size);
    SDL_free(compressed_frame);
}

static void xor_buf(Uint8* restrict dst, const Uint8* restrict src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dst[i] ^= src[i];
    }
}

bool RamArchive_Init(RamArchive* archive, const char* path) {
    SDL_zerop(archive);
    archive->io = SDL_IOFromFile(path, "rb");

    if (archive->io == NULL) {
        SDL_Log("Failed to open RAM archive: %s", SDL_GetError());
        return false;
    }

    char magic[4];
    SDL_ReadIO(archive->io, magic, sizeof(magic));

    if (SDL_strncmp(magic, RAM_ARCHIVE_MAGIC, sizeof(magic)) != 0) {
        SDL_Log("File at path %s is not a RAM archive", path);
        SDL_CloseIO(archive->io);
        archive->io = NULL;
        return false;
    }

    SDL_ReadU16LE(archive->io, &archive->entry_count);
    archive->entries = SDL_calloc(archive->entry_count, sizeof(RamArchive_FrameTableEntry));

    for (int i = 0; i < archive->entry_count; i++) {
        RamArchive_FrameTableEntry* entry = &archive->entries[i];
        SDL_ReadU32LE(archive->io, &entry->offset);
        SDL_ReadU32LE(archive->io, &entry->size);
    }

    archive->current_frame_index = 0;
    archive->current_frame = SDL_malloc(RAM_FRAME_SIZE);
    read_frame(archive, 0, archive->current_frame);

    return true;
}

bool RamArchive_SeekFrame(RamArchive* archive, Uint16 index) {
    if (index >= archive->entry_count) {
        SDL_Log("Frame %d is out of bounds", index);
        return false;
    }

    Uint8* buf = SDL_malloc(RAM_FRAME_SIZE);
    int iterations = index - archive->current_frame_index;
    bool forward = true;

    if (iterations < 0) {
        forward = false;
        iterations = -iterations;
    }

    for (int i = 0; i < iterations; i++) {
        const Uint16 frame_index = forward ? (archive->current_frame_index + i + 1) : (archive->current_frame_index - i);
        read_frame(archive, frame_index, buf);
        xor_buf(archive->current_frame, buf, RAM_FRAME_SIZE);
    }

    archive->current_frame_index = index;
    return true;
}

SDL_IOStream* RamArchive_GetCurrentFrame(RamArchive* archive) {
    return SDL_IOFromConstMem(archive->current_frame, RAM_FRAME_SIZE);
}

SDL_IOStream* RamArchive_GetFrame(RamArchive* archive, Uint16 index) {
    RamArchive_SeekFrame(archive, index);
    return RamArchive_GetCurrentFrame(archive);
}

void RamArchive_Destroy(RamArchive* archive) {
    SDL_CloseIO(archive->io);
    SDL_free(archive->entries);
    SDL_zerop(archive);
}

#endif
