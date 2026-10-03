#include "port/io/resource.h"
#include "port/io/afs.h"
#include "port/io/cache.h"
#include "port/utils.h"

#include <SDL3/SDL.h>

/// Set on handles that belong to the cache backend. Backends number their handles from 0,
/// so this tells them apart while keeping every valid handle positive.
#define CACHE_HANDLE_FLAG 0x40000000

static bool is_cache_fnum(size_t fnum) {
    return fnum >= RESOURCE_CACHE_BASE;
}

static bool to_cache_file(size_t fnum, CacheFile* file) {
    if (!is_cache_fnum(fnum) || (fnum - RESOURCE_CACHE_BASE >= CACHE_FILE_COUNT)) {
        return false;
    }

    *file = fnum - RESOURCE_CACHE_BASE;
    return true;
}

static bool is_cache_handle(ResourceHandle handle) {
    return (handle & CACHE_HANDLE_FLAG) != 0;
}

static CacheHandle to_cache_handle(ResourceHandle handle) {
    return handle & ~CACHE_HANDLE_FLAG;
}

static ResourceReadState state_from_afs(AFSReadState state) {
    switch (state) {
    case AFS_READ_STATE_IDLE:
        return RESOURCE_READ_STATE_IDLE;

    case AFS_READ_STATE_READING:
        return RESOURCE_READ_STATE_READING;

    case AFS_READ_STATE_FINISHED:
        return RESOURCE_READ_STATE_FINISHED;

    case AFS_READ_STATE_ERROR:
        return RESOURCE_READ_STATE_ERROR;

    default:
        fatal_error("Unhandled AFS state: %d", state);
    }
}

static ResourceReadState state_from_cache(CacheReadState state) {
    switch (state) {
    case CACHE_READ_STATE_IDLE:
        return RESOURCE_READ_STATE_IDLE;

    case CACHE_READ_STATE_READING:
        return RESOURCE_READ_STATE_READING;

    case CACHE_READ_STATE_FINISHED:
        return RESOURCE_READ_STATE_FINISHED;

    case CACHE_READ_STATE_ERROR:
        return RESOURCE_READ_STATE_ERROR;

    default:
        fatal_error("Unhandled cache state: %d", state);
    }
}

bool Resource_Init(const char* afs_path, size_t read_chunk_size) {
    if (!AFS_Init(afs_path, read_chunk_size)) {
        return false;
    }

    SDL_assert(AFS_GetFileCount() <= RESOURCE_CACHE_BASE);
    Cache_Init(read_chunk_size);
    return true;
}

void Resource_Finish() {
    Cache_Finish();
    AFS_Finish();
}

bool Resource_Exists(size_t fnum) {
    CacheFile file;

    if (is_cache_fnum(fnum)) {
        return to_cache_file(fnum, &file) && Cache_Exists(file);
    }

    return fnum < AFS_GetFileCount();
}

size_t Resource_GetSize(size_t fnum) {
    CacheFile file;

    if (is_cache_fnum(fnum)) {
        return to_cache_file(fnum, &file) ? Cache_GetSize(file) : 0;
    }

    return AFS_GetSize(fnum);
}

void Resource_RunServer() {
    AFS_RunServer();
    Cache_RunServer();
}

ResourceHandle Resource_Open(size_t fnum) {
    CacheFile file;

    if (is_cache_fnum(fnum)) {
        return to_cache_file(fnum, &file) ? (Cache_Open(file) | CACHE_HANDLE_FLAG) : RESOURCE_NONE;
    }

    const AFSHandle handle = AFS_Open(fnum);
    SDL_assert((handle & CACHE_HANDLE_FLAG) == 0);
    return (handle == AFS_NONE) ? RESOURCE_NONE : handle;
}

void Resource_Read(ResourceHandle handle, void* buf) {
    if (is_cache_handle(handle)) {
        Cache_Read(to_cache_handle(handle), buf);
    } else {
        AFS_Read(handle, buf);
    }
}

void Resource_ReadSync(ResourceHandle handle, void* buf) {
    if (is_cache_handle(handle)) {
        Cache_ReadSync(to_cache_handle(handle), buf);
    } else {
        AFS_ReadSync(handle, buf);
    }
}

void Resource_Stop(ResourceHandle handle) {
    if (is_cache_handle(handle)) {
        Cache_Stop(to_cache_handle(handle));
    } else {
        AFS_Stop(handle);
    }
}

void Resource_Close(ResourceHandle handle) {
    if (is_cache_handle(handle)) {
        Cache_Close(to_cache_handle(handle));
    } else {
        AFS_Close(handle);
    }
}

ResourceReadState Resource_GetState(ResourceHandle handle) {
    if (is_cache_handle(handle)) {
        return state_from_cache(Cache_GetState(to_cache_handle(handle)));
    }

    return state_from_afs(AFS_GetState(handle));
}
