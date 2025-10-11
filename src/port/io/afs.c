#include "port/io/afs.h"
#include <SDL3/SDL.h>
#include <stdio.h>

// Inspired by https://github.com/MaikelChan/AFSLib

#define AFS_MAGIC 0x41465300
#define AFS_ATTRIBUTE_HEADER_SIZE 8
#define AFS_ATTRIBUTE_ENTRY_SIZE 48
#define AFS_MAX_NAME_LENGTH 32

#define AFS_MAX_READ_REQUESTS 100

typedef struct AFSEntry {
    unsigned int offset;
    unsigned int size;
    char name[AFS_MAX_NAME_LENGTH];
} AFSEntry;

typedef struct AFS {
    char* file_path;
    unsigned int entry_count;
    AFSEntry* entries;
} AFS;

typedef struct ReadRequest {
    bool initialized;
    int index;
    int file_num;
    int sector;
    AFSReadState state;
    SDL_AsyncIO* asyncio;
} ReadRequest;

static AFS afs = { 0 };
static SDL_AsyncIOQueue* asyncio_queue = NULL;
static ReadRequest requests[AFS_MAX_READ_REQUESTS] = { { 0 } };

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
    char c;

    do {
        SDL_ReadIO(src, &c, 1);
        *dst++ = c;
    } while (c != '\0');
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
    afs.entries = SDL_malloc(sizeof(AFSEntry) * afs.entry_count);

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
            read_string(io, &entry->name);
        } else {
            SDL_memset(&entry->name, 0, sizeof(entry->name));
        }
    }

    SDL_CloseIO(io);
    return true;
}

static bool init_asyncio(const char* file_path) {
    asyncio_queue = SDL_CreateAsyncIOQueue();
    return asyncio_queue != NULL;
}

bool AFS_Init(const char* file_path) {
    if (!init_afs(file_path)) {
        return false;
    }

    return init_asyncio(file_path);
}

void AFS_Finish() {
    SDL_free(afs.file_path);
    SDL_free(afs.entries);
    SDL_zero(afs);
    SDL_zeroa(requests);
    SDL_DestroyAsyncIOQueue(asyncio_queue);
}

unsigned int AFS_GetFileCount() {
    return afs.entry_count;
}

unsigned int AFS_GetSize(int file_num) {
    if ((file_num < 0) || (file_num >= afs.entry_count)) {
        return 0;
    }

    return afs.entries[file_num].size;
}

// AFS reading

void AFS_RunServer() {
    SDL_AsyncIOOutcome outcome;

    while (SDL_GetAsyncIOResult(asyncio_queue, &outcome)) {
        ReadRequest* request = (ReadRequest*)outcome.userdata;

        switch (outcome.type) {
        case SDL_ASYNCIO_TASK_READ:
            switch (outcome.result) {
            case SDL_ASYNCIO_COMPLETE:
                request->state = AFS_READ_STATE_FINISHED;
                break;

            case SDL_ASYNCIO_CANCELED:
                request->state = AFS_READ_STATE_IDLE;
                break;

            case SDL_ASYNCIO_FAILURE:
                request->state = AFS_READ_STATE_ERROR;
                break;
            }

            break;

        case SDL_ASYNCIO_TASK_CLOSE:
            request->state = AFS_READ_STATE_IDLE;
            break;

        case SDL_ASYNCIO_TASK_WRITE:
            // Do nothing
            break;
        }

        request->asyncio = NULL;
    }
}

AFSHandle AFS_Open(int file_num) {
    for (int i = 0; i < SDL_arraysize(requests); i++) {
        ReadRequest* request = &requests[i];

        if (request->initialized) {
            continue;
        }

        request->file_num = file_num;
        request->sector = 0;
        request->index = i;
        request->state = AFS_READ_STATE_IDLE;
        request->initialized = true;
        return i;
    }

    return AFS_NONE;
}

void AFS_Read(AFSHandle handle, int sectors, void* buf) {
    ReadRequest* request = &requests[handle];
    const Uint64 offset = afs.entries[request->file_num].offset + request->sector * 2048;

    request->state = AFS_READ_STATE_READING;
    request->asyncio = SDL_AsyncIOFromFile(afs.file_path, "r");

    if (request->asyncio == NULL) {
        printf("SDL_AsyncIOFromFile error: %s\n", SDL_GetError());
        request->state = AFS_READ_STATE_ERROR;
        return;
    }

    const bool success = SDL_ReadAsyncIO(request->asyncio, buf, offset, sectors * 2048, asyncio_queue, request);

    if (!success) {
        printf("SDL_ReadAsyncIO error: %s\n", SDL_GetError());
        request->state = AFS_READ_STATE_ERROR;
        return;
    }

    request->sector += sectors;
}

void AFS_Stop(AFSHandle handle) {
    ReadRequest* request = &requests[handle];

    if (request->asyncio != NULL) {
        SDL_CloseAsyncIO(request->asyncio, false, asyncio_queue, request);
        request->asyncio = NULL;
    }
}

void AFS_Close(AFSHandle handle) {
    ReadRequest* request = &requests[handle];
    AFS_Stop(handle);
    SDL_zerop(request);
}

AFSReadState AFS_GetState(AFSHandle handle) {
    ReadRequest* request = &requests[handle];
    return request->state;
}

unsigned int AFS_GetSectorCount(AFSHandle handle) {
    ReadRequest* request = &requests[handle];
    const unsigned int size = afs.entries[request->file_num].size;
    return (size + 2048 - 1) / 2048;
}
