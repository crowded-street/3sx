#if STATCHECK

#ifndef RAM_ARCHIVE_H
#define RAM_ARCHIVE_H

#include <SDL3/SDL.h>

typedef struct RamArchive_FrameTableEntry {
    Uint32 offset;
    Uint32 size;
} RamArchive_FrameTableEntry;

typedef struct RamArchive {
    SDL_IOStream* io;
    Uint16 entry_count;
    Uint16 current_frame_index;
    Uint8* current_frame;
    Uint8* frame_scratch;
    RamArchive_FrameTableEntry* entries;
} RamArchive;

bool RamArchive_Init(RamArchive* archive, const char* path);
bool RamArchive_SeekFrame(RamArchive* archive, Uint16 index);
SDL_IOStream* RamArchive_GetCurrentFrame(RamArchive* archive);
SDL_IOStream* RamArchive_GetFrame(RamArchive* archive, Uint16 index);
void RamArchive_Destroy(RamArchive* archive);

#endif

#endif
