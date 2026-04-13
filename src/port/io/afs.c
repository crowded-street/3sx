#include "port/io/afs.h"
#include "common.h"

#include "stb/stb_ds.h"
#include <SDL3/SDL.h>

#include <stdio.h>

// Inspired by https://github.com/MaikelChan/AFSLib

#define AFS_MAGIC 0x41465300
#define AFS_ATTRIBUTE_HEADER_SIZE 8
#define AFS_ATTRIBUTE_ENTRY_SIZE 48
#define AFS_MAX_NAME_LENGTH 32

// FIXME: Extract to a common API surface
#if PSP
#define READ_CHUNK_SIZE 16 * 1024
#else
#define READ_CHUNK_SIZE 256 * 1024
#endif

// Uncomment this to enable debug prints
// #define AFS_DEBUG

typedef struct AFSEntry {
    size_t offset;
    size_t size;
    char name[AFS_MAX_NAME_LENGTH];
} AFSEntry;

typedef struct AFS {
    const char* file_path;
    size_t entry_count;
    AFSEntry* entries;
} AFS;

typedef struct ReadRequest {
    bool initialized;
    size_t file_num;
    size_t bytes_to_read;
    size_t bytes_read;
    Uint8* buf;
    AFSReadState state;
} ReadRequest;

static AFS afs = { 0 };
static ReadRequest* requests = NULL;
static SDL_IOStream* stream = NULL;

static bool is_valid_attribute_data(Uint32 attributes_offset, Uint32 attributes_size, Sint64 file_size,
                                    Uint32 entries_end_offset, Uint32 entry_count) {
    if ((attributes_offset == 0) || (attributes_size == 0)) {
        return false;
    }

    if (attributes_size > (file_size - entries_end_offset)) {
        return false;
    }

    if (attributes_size < (entry_count * AFS_ATTRIBUTE_ENTRY_SIZE)) {
        return false;
    }

    if (attributes_offset < entries_end_offset) {
        return false;
    }

    if (attributes_offset > (file_size - attributes_size)) {
        return false;
    }

    return true;
}

static void read_string(SDL_IOStream* src, char* dst) {
    do {
        SDL_ReadS8(src, dst);
        dst++;
    } while (*dst != '\0');
}

static bool init_afs(const char* file_path) {
    afs.file_path = SDL_strdup(file_path);
    SDL_IOStream* io = SDL_IOFromFile(file_path, "rb");

    if (io == NULL) {
        return false;
    }

    // Check magic

    Uint32 magic = 0;
    SDL_ReadU32BE(io, &magic);

    if (magic != AFS_MAGIC) {
        SDL_CloseIO(io);
        return false;
    }

    // Read entries

    SDL_ReadU32LE(io, &afs.entry_count);
    afs.entries = SDL_calloc(afs.entry_count, sizeof(AFSEntry));

    Uint32 entries_start_offset = 0;
    Uint32 entries_end_offset = 0;

    for (int i = 0; i < afs.entry_count; i++) {
        AFSEntry* entry = &afs.entries[i];
        SDL_ReadU32LE(io, &entry->offset);
        SDL_ReadU32LE(io, &entry->size);

        if (entry->offset != 0) {
            if (entries_start_offset == 0) {
                entries_start_offset = entry->offset;
            }

            entries_end_offset = entry->offset + entry->size;
        }
    }

    // Locate attributes

    Uint32 attributes_offset;
    Uint32 attributes_size;
    bool has_attributes = false;

    SDL_ReadU32LE(io, &attributes_offset);
    SDL_ReadU32LE(io, &attributes_size);

    if (is_valid_attribute_data(
            attributes_offset, attributes_size, SDL_GetIOSize(io), entries_end_offset, afs.entry_count)) {
        has_attributes = true;
    } else {
        SDL_SeekIO(io, entries_start_offset - AFS_ATTRIBUTE_HEADER_SIZE, SDL_IO_SEEK_SET);

        SDL_ReadU32LE(io, &attributes_offset);
        SDL_ReadU32LE(io, &attributes_size);

        if (is_valid_attribute_data(
                attributes_offset, attributes_size, SDL_GetIOSize(io), entries_end_offset, afs.entry_count)) {
            has_attributes = true;
        }
    }

    for (int i = 0; i < afs.entry_count; i++) {
        AFSEntry* entry = &afs.entries[i];

        if ((entry->offset != 0) && has_attributes) {
            SDL_SeekIO(io, attributes_offset + i * AFS_ATTRIBUTE_ENTRY_SIZE, SDL_IO_SEEK_SET);
            read_string(io, entry->name);
        } else {
            SDL_zeroa(entry->name);
        }
    }

    stream = io;
    return true;
}

bool AFS_Init(const char* file_path) {
    if (!init_afs(file_path)) {
        return false;
    }

    return true;
}

void AFS_Finish() {
    SDL_CloseIO(stream);
    stream = NULL;
    SDL_free(afs.file_path);
    SDL_free(afs.entries);
    SDL_zero(afs);
    arrfree(requests);
}

size_t AFS_GetFileCount() {
    return afs.entry_count;
}

size_t AFS_GetSize(size_t file_num) {
    if (file_num >= afs.entry_count) {
        return 0;
    }

    return afs.entries[file_num].size;
}

// AFS reading

static int find_free_request_slot() {
    for (int i = 0; i < arrlen(requests); i++) {
        if (!requests[i].initialized) {
            return i;
        }
    }

    return arraddnindex(requests, 1);
}

static void read_into_request(ReadRequest* request, size_t max_read) {
    const Sint64 offset = afs.entries[request->file_num].offset + request->bytes_read;
    SDL_SeekIO(stream, offset, SDL_IO_SEEK_SET);

    const size_t bytes_to_read = SDL_min(request->bytes_to_read, max_read);
    const size_t bytes_read = SDL_ReadIO(stream, request->buf + request->bytes_read, bytes_to_read);

    request->bytes_read += bytes_read;
    request->bytes_to_read -= bytes_read;

    if (request->bytes_to_read == 0) {
        request->state = AFS_READ_STATE_FINISHED;
    }
}

void AFS_RunServer() {
    int running_requests = 0;

    for (int i = 0; i < arrlen(requests); i++) {
        if (requests[i].state == AFS_READ_STATE_READING) {
            running_requests += 1;
        }
    }

    if (running_requests <= 0) {
        return;
    }

    const size_t max_read_per_request = READ_CHUNK_SIZE / running_requests;

    for (int i = 0; i < arrlen(requests); i++) {
        ReadRequest* request = &requests[i];

        if (request->state != AFS_READ_STATE_READING) {
            continue;
        }

        read_into_request(request, max_read_per_request);
    }
}

AFSHandle AFS_Open(size_t file_num) {
    if (file_num >= afs.entry_count) {
        return AFS_NONE;
    }

    const int index = find_free_request_slot();
    ReadRequest* request = &requests[index];

    request->file_num = file_num;
    request->bytes_to_read = afs.entries[file_num].size;
    request->bytes_read = 0;
    request->buf = NULL;
    request->state = AFS_READ_STATE_IDLE;
    request->initialized = true;

#if defined(AFS_DEBUG)
    printf("📂 %d: open (file_num = %d, filename = %s)\n", index, file_num, afs.entries[file_num].name);
#endif

    return index;
}

void AFS_Read(AFSHandle handle, void* buf) {
#if defined(AFS_DEBUG)
    printf("📂 %d: read (sectors = %d, bytes = 0x%X)\n", handle, sectors, sectors * 2048);
#endif

    ReadRequest* request = &requests[handle];
    SDL_assert(request->buf == NULL);
    SDL_assert(request->state == AFS_READ_STATE_IDLE);
    request->buf = buf;
    request->state = AFS_READ_STATE_READING;
}

void AFS_ReadSync(AFSHandle handle, void* buf) {
#if defined(AFS_DEBUG)
    printf("📂 %d: read sync\n", handle);
#endif

    AFS_Read(handle, buf);
    ReadRequest* request = &requests[handle];
    read_into_request(request, request->bytes_to_read);
    SDL_assert(request->state == AFS_READ_STATE_FINISHED);
}

void AFS_Stop(AFSHandle handle) {
#if defined(AFS_DEBUG)
    printf("📂 %d: stop\n", handle);
#endif

    requests[handle].state = AFS_READ_STATE_IDLE;
}

void AFS_Close(AFSHandle handle) {
#if defined(AFS_DEBUG)
    printf("📂 %d: close\n", handle);
#endif

    SDL_zero(requests[handle]);
}

AFSReadState AFS_GetState(AFSHandle handle) {
    ReadRequest* request = &requests[handle];

#if defined(AFS_DEBUG)
    printf("📂 %d: get state (%d)\n", handle, request->state);
#endif

    return request->state;
}
