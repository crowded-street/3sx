#include "port/io/cache.h"
#include "port/paths.h"
#include "port/utils.h"

#include "stb/stb_ds.h"
#include <SDL3/SDL.h>

#define CACHE_VERSION 0
#define VERSION_FILE "version"

static const char* const paths[CACHE_FILE_COUNT] = {
    [CACHE_FILE_SHIN_AKUMA_BG_PALETTE] = "3s/bg15_palette.bin",
    [CACHE_FILE_NG_GILL_BG_PALETTE] = "ng/bg00_palette.bin",
    [CACHE_FILE_NG_GILL_STAGE] = "ng/stage00.ppg",
    [CACHE_FILE_NG_GILL_OBJECTS] = "ng/bg00.bin",
    [CACHE_FILE_NG_ALEX_BG_PALETTE] = "ng/bg01_palette.bin",
    [CACHE_FILE_NG_ALEX_STAGE] = "ng/stage01.ppg",
    [CACHE_FILE_NG_ALEX_OBJECTS] = "ng/bg01.bin",
    [CACHE_FILE_NG_RYU_A_BG_PALETTE] = "ng/bg03_palette.bin",
    [CACHE_FILE_NG_RYU_A_STAGE] = "ng/stage03.ppg",
    [CACHE_FILE_NG_RYU_A_OBJECTS] = "ng/bg03.bin",
    [CACHE_FILE_NG_RYU_B_BG_PALETTE] = "ng/bg04_palette.bin",
    [CACHE_FILE_NG_RYU_B_STAGE] = "ng/stage04.ppg",
    [CACHE_FILE_NG_RYU_B_OBJECTS] = "ng/bg04.bin",
    [CACHE_FILE_NG_YUN_A_BG_PALETTE] = "ng/bg05_palette.bin",
    [CACHE_FILE_NG_YUN_A_STAGE] = "ng/stage05.ppg",
    [CACHE_FILE_NG_YUN_A_OBJECTS] = "ng/bg05.bin",
    [CACHE_FILE_NG_EFC1] = "ng/efc1.bin",
    [CACHE_FILE_NG_YUN_B_BG_PALETTE] = "ng/bg06_palette.bin",
    [CACHE_FILE_NG_YUN_B_STAGE] = "ng/stage06.ppg",
    [CACHE_FILE_NG_YUN_B_OBJECTS] = "ng/bg06.bin",
    [CACHE_FILE_NG_DUDLEY_A_BG_PALETTE] = "ng/bg07_palette.bin",
    [CACHE_FILE_NG_DUDLEY_A_STAGE] = "ng/stage07.ppg",
    [CACHE_FILE_NG_DUDLEY_OBJECTS] = "ng/bg07.bin",
    [CACHE_FILE_NG_DUDLEY_B_BG_PALETTE] = "ng/bg08_palette.bin",
    [CACHE_FILE_NG_DUDLEY_B_STAGE] = "ng/stage08.ppg",
    [CACHE_FILE_NG_NECRO_BG_PALETTE] = "ng/bg09_palette.bin",
    [CACHE_FILE_NG_NECRO_STAGE] = "ng/stage09.ppg",
    [CACHE_FILE_NG_NECRO_OBJECTS] = "ng/bg09.bin",
    [CACHE_FILE_NG_EF94] = "ng/ef94.bin",
    [CACHE_FILE_NG_EF94_PALETTE] = "ng/ef94_palette.bin",
};

typedef struct ReadRequest {
    bool initialized;
    CacheFile file;
    SDL_IOStream* io;
    size_t bytes_to_read;
    size_t bytes_read;
    Uint8* buf;
    CacheReadState state;
} ReadRequest;

static ReadRequest* requests = NULL;
static size_t _read_chunk_size = 0;

static bool get_file_info(CacheFile file, SDL_PathInfo* info) {
    char* path = Cache_GetFullPath(file);
    const bool exists = SDL_GetPathInfo(path, info) && info->type == SDL_PATHTYPE_FILE;
    SDL_free(path);
    return exists;
}

static void remove_tree(const char* path);

static SDL_EnumerationResult remove_entry(void* userdata, const char* dirname, const char* fname) {
    char* path = NULL;
    SDL_asprintf(&path, "%s%s", dirname, fname);
    remove_tree(path);
    SDL_free(path);
    return SDL_ENUM_CONTINUE;
}

/// Removes a file, or a directory with everything in it.
static void remove_tree(const char* path) {
    SDL_PathInfo info;

    if (!SDL_GetPathInfo(path, &info)) {
        return;
    }

    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        SDL_EnumerateDirectory(path, remove_entry, NULL);
    }

    if (!SDL_RemovePath(path)) {
        SDL_Log("Couldn't remove %s: %s", path, SDL_GetError());
    }
}

static void clear_cache() {
    char* path = Paths_GetCachePath(NULL);
    remove_tree(path);
    SDL_free(path);
}

static bool is_cache_current() {
    char* path = Paths_GetCachePath(VERSION_FILE);
    char* text = SDL_LoadFile(path, NULL);
    bool current = false;

    if (text != NULL) {
        char* end = NULL;
        const long version = SDL_strtol(text, &end, 10);
        current = end != text && *end == '\0' && version == CACHE_VERSION;
    }

    SDL_free(text);
    SDL_free(path);
    return current;
}

static void write_version() {
    char* dir = Paths_GetCachePath(NULL);
    char* path = Paths_GetCachePath(VERSION_FILE);
    char text[16];
    SDL_snprintf(text, sizeof(text), "%d", CACHE_VERSION);

    if (!SDL_CreateDirectory(dir) || !SDL_SaveFile(path, text, SDL_strlen(text))) {
        SDL_Log("Couldn't write %s: %s", path, SDL_GetError());
    }

    SDL_free(path);
    SDL_free(dir);
}

void Cache_Init(size_t read_chunk_size) {
    SDL_assert(read_chunk_size > 0);
    _read_chunk_size = read_chunk_size;

    if (!is_cache_current()) {
        clear_cache();
        write_version();
    }
}

void Cache_Finish() {
    for (int i = 0; i < arrlen(requests); i++) {
        if (requests[i].io != NULL) {
            SDL_CloseIO(requests[i].io);
        }
    }

    arrfree(requests);
}

const char* Cache_GetPath(CacheFile file) {
    SDL_assert(file < CACHE_FILE_COUNT);
    SDL_assert(paths[file] != NULL);
    return paths[file];
}

char* Cache_GetFullPath(CacheFile file) {
    return Paths_GetCachePath(Cache_GetPath(file));
}

bool Cache_Exists(CacheFile file) {
    SDL_PathInfo info;
    return get_file_info(file, &info);
}

size_t Cache_GetSize(CacheFile file) {
    SDL_PathInfo info;
    return get_file_info(file, &info) ? info.size : 0;
}

// Cache writing

bool Cache_Write(CacheFile file, const void* data, size_t size) {
    char* path = Cache_GetFullPath(file);
    char* dir = SDL_strdup(path);
    *SDL_strrchr(dir, '/') = '\0';
    char* temp_path = NULL;
    SDL_asprintf(&temp_path, "%s.tmp", path);

    // Files that exist are never rebuilt, so an interrupted write mustn't leave a partial file behind
    const bool success =
        SDL_CreateDirectory(dir) && SDL_SaveFile(temp_path, data, size) && SDL_RenamePath(temp_path, path);

    if (!success) {
        SDL_Log("Couldn't write %s: %s", path, SDL_GetError());
    }

    SDL_free(temp_path);
    SDL_free(dir);
    SDL_free(path);
    return success;
}

// Cache reading

static int find_free_request_slot() {
    for (int i = 0; i < arrlen(requests); i++) {
        if (!requests[i].initialized) {
            return i;
        }
    }

    return arraddnindex(requests, 1);
}

static void read_into_request(ReadRequest* request, size_t max_read) {
    const size_t bytes_to_read = SDL_min(request->bytes_to_read, max_read);
    const size_t bytes_read = SDL_ReadIO(request->io, request->buf + request->bytes_read, bytes_to_read);

    request->bytes_read += bytes_read;
    request->bytes_to_read -= bytes_read;

    if (request->bytes_to_read == 0) {
        request->state = CACHE_READ_STATE_FINISHED;
    } else if (bytes_read < bytes_to_read) {
        SDL_Log("Couldn't read %s: %s", Cache_GetPath(request->file), SDL_GetError());
        request->state = CACHE_READ_STATE_ERROR;
    }
}

void Cache_RunServer() {
    int running_requests = 0;

    for (int i = 0; i < arrlen(requests); i++) {
        if (requests[i].state == CACHE_READ_STATE_READING) {
            running_requests += 1;
        }
    }

    if (running_requests <= 0) {
        return;
    }

    const size_t max_read_per_request = _read_chunk_size / running_requests;

    for (int i = 0; i < arrlen(requests); i++) {
        ReadRequest* request = &requests[i];

        if (request->state == CACHE_READ_STATE_READING) {
            read_into_request(request, max_read_per_request);
        }
    }
}

CacheHandle Cache_Open(CacheFile file) {
    char* path = Cache_GetFullPath(file);
    SDL_IOStream* io = SDL_IOFromFile(path, "rb");

    if (io == NULL) {
        fatal_error("Couldn't open %s: %s", path, SDL_GetError());
    }

    SDL_free(path);

    const int index = find_free_request_slot();
    ReadRequest* request = &requests[index];

    request->file = file;
    request->io = io;
    request->bytes_to_read = SDL_GetIOSize(io);
    request->bytes_read = 0;
    request->buf = NULL;
    request->state = CACHE_READ_STATE_IDLE;
    request->initialized = true;

    return index;
}

void Cache_Read(CacheHandle handle, void* buf) {
    ReadRequest* request = &requests[handle];
    SDL_assert(request->buf == NULL);
    SDL_assert(request->state == CACHE_READ_STATE_IDLE);
    request->buf = buf;
    request->state = CACHE_READ_STATE_READING;
}

void Cache_ReadSync(CacheHandle handle, void* buf) {
    ReadRequest* request = &requests[handle];
    Cache_Read(handle, buf);
    read_into_request(request, request->bytes_to_read);
}

void Cache_Stop(CacheHandle handle) {
    requests[handle].state = CACHE_READ_STATE_IDLE;
}

void Cache_Close(CacheHandle handle) {
    ReadRequest* request = &requests[handle];
    SDL_CloseIO(request->io);
    SDL_zerop(request);
}

CacheReadState Cache_GetState(CacheHandle handle) {
    return requests[handle].state;
}
