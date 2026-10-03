#ifndef PORT_IO_RESOURCE_H
#define PORT_IO_RESOURCE_H

#include "port/io/cache.h"

#include <stdbool.h>
#include <stddef.h>

#define RESOURCE_CACHE_BASE 0xF000
#define RESOURCE_CACHE_FNUM(file) (RESOURCE_CACHE_BASE + (file))

typedef enum ResourceReadState {
    RESOURCE_READ_STATE_IDLE,
    RESOURCE_READ_STATE_READING,
    RESOURCE_READ_STATE_FINISHED,
    RESOURCE_READ_STATE_ERROR,
} ResourceReadState;

typedef int ResourceHandle;

#define RESOURCE_NONE -1

bool Resource_Init(const char* afs_path, size_t read_chunk_size);
void Resource_Finish();
bool Resource_Exists(size_t fnum);
size_t Resource_GetSize(size_t fnum);
void Resource_RunServer();
ResourceHandle Resource_Open(size_t fnum);
void Resource_Read(ResourceHandle handle, void* buf);
void Resource_ReadSync(ResourceHandle handle, void* buf);
void Resource_Stop(ResourceHandle handle);
void Resource_Close(ResourceHandle handle);
ResourceReadState Resource_GetState(ResourceHandle handle);

#endif
