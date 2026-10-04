#ifndef PORT_IO_CACHE_H
#define PORT_IO_CACHE_H

#include <stdbool.h>
#include <stddef.h>

// Add a path to `paths` in cache.c for every file

typedef enum CacheFile {
    CACHE_FILE_SHIN_AKUMA_BG_PALETTE,
    CACHE_FILE_NG_ALEX_BG_PALETTE,
    CACHE_FILE_NG_ALEX_STAGE,
    CACHE_FILE_COUNT,
} CacheFile;

typedef enum CacheReadState {
    CACHE_READ_STATE_IDLE,
    CACHE_READ_STATE_READING,
    CACHE_READ_STATE_FINISHED,
    CACHE_READ_STATE_ERROR,
} CacheReadState;

typedef int CacheHandle;

void Cache_Init(size_t read_chunk_size);
void Cache_Finish();

/// @return Path of the file relative to the cache directory.
const char* Cache_GetPath(CacheFile file);

/// @return Absolute path of the file. Free it with `SDL_free`.
char* Cache_GetFullPath(CacheFile file);

bool Cache_Exists(CacheFile file);
size_t Cache_GetSize(CacheFile file);
bool Cache_Write(CacheFile file, const void* data, size_t size);
void Cache_RunServer();
CacheHandle Cache_Open(CacheFile file);
void Cache_Read(CacheHandle handle, void* buf);
void Cache_ReadSync(CacheHandle handle, void* buf);
void Cache_Stop(CacheHandle handle);
void Cache_Close(CacheHandle handle);
CacheReadState Cache_GetState(CacheHandle handle);

#endif
